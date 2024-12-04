/*
 * ultrasonic_sensor.c
 *
 * Created on:         2024-12-03 20:56:44
 *     Author:         carlitos (benzon.salazar@gmail.com)
 *
 * Last Modified by:   carrliitos
 * Last Modified time: 2024-12-04 02:18:49
 */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "ultrasonic.h" // From esp-idf-lib
#include "esp_log.h"
#include "esp_http_client.h" 
#include "cJSON.h"
#include "mbedtls/base64.h"
#include "mbedtls/md.h"
#include "mbedtls/pk.h"
#include "mbedtls/error.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"

#include "key_loader.h"

#include "ultrasonic_sensor.h"

#define TAG           "Sensor"
#define SENSOR_TAG    "HC-SR04"
#define PUBSUB_URL    "https://pubsub.googleapis.com/v1/projects/esp32-ultrasonic-sensor-443705/topics/esp32-ultrasonic-topic:publish"

#define MAX_PAYLOAD   256

static char jwt_token[1024];

/**
 * Generate a JWT for Google Pub/Sub authentication
 */
static esp_err_t generate_jwt(char *jwt, size_t jwt_len) {
 // Load Private Key

  esp_err_t private_key_err = init_filesystem();
  if (private_key_err == ESP_OK) {
    ESP_LOGI(TAG, "Filesystem initialized. Attempting to load private key.");
    
    PRIVATE_KEY = load_private_key();  // No warning here
    if (PRIVATE_KEY != NULL) {
      ESP_LOGI(TAG, "Successfully loaded private key.");
    } else {
      ESP_LOGE(TAG, "Failed to load private key.");
    }
  } else {
    ESP_LOGE(TAG, "Failed to initialize filesystem: %s", esp_err_to_name(private_key_err));
  }

  const char *header = "{\"alg\":\"RS256\",\"typ\":\"JWT\"}";
  const char *payload_template = "{\"iss\":\"esp32-ultrasonic-sensor-sa@esp32-ultrasonic-sensor-443705.iam.gserviceaccount.com\",\"scope\":\"https://www.googleapis.com/auth/pubsub\",\"aud\":\"https://oauth2.googleapis.com/token\",\"exp\":%lu,\"iat\":%lu}";
  
  time_t now = time(NULL);
  char payload[512];
  snprintf(payload, sizeof(payload), payload_template, now + 3600, now);

  // Base64 encode header and payload
  char header_base64[256], payload_base64[256];
  size_t out_len;
  mbedtls_base64_encode((unsigned char *)header_base64, sizeof(header_base64), &out_len, (unsigned char *)header, strlen(header));
  mbedtls_base64_encode((unsigned char *)payload_base64, sizeof(payload_base64), &out_len, (unsigned char *)payload, strlen(payload));

  if (strlen(header_base64) == 0 || strlen(payload_base64) == 0) {
    ESP_LOGE(TAG, "Base64 strings are empty");
    return ESP_FAIL;
  }

  // Create signing input
  char signing_input[512];
  int snprintf_result = snprintf(signing_input, sizeof(signing_input), "%s.%s", header_base64, payload_base64);
  if (snprintf_result < 0 || snprintf_result >= sizeof(signing_input)) {
    ESP_LOGE(TAG, "Failed to create signing input. Buffer too small or snprintf failed.");
    return ESP_FAIL;
  }
  ESP_LOGI(TAG, "Signing Input: %s", signing_input);

  // Initialize RNG context
  mbedtls_entropy_context entropy;
  mbedtls_ctr_drbg_context ctr_drbg;
  mbedtls_entropy_init(&entropy);
  mbedtls_ctr_drbg_init(&ctr_drbg);

  const char *pers = "jwt_sign";
  int ret = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, (const unsigned char *)pers, strlen(pers));
  if (ret != 0) {
    ESP_LOGE(TAG, "Failed to seed RNG: %d", ret);
    return ESP_FAIL;
  }

  // Sign the input
  unsigned char signature[256];
  mbedtls_pk_context pk_context;
  mbedtls_pk_init(&pk_context);
  mbedtls_pk_parse_key(&pk_context, (const unsigned char *)PRIVATE_KEY, strlen(PRIVATE_KEY) + 1, NULL, 0, NULL, NULL);

  // Perform signing
  size_t sig_len;
  ret = mbedtls_pk_sign(&pk_context, MBEDTLS_MD_SHA256, (unsigned char *)signing_input, strlen(signing_input),
                        signature, sizeof(signature), &sig_len, mbedtls_ctr_drbg_random, &ctr_drbg);
  if (ret != 0) {
    ESP_LOGE(TAG, "Failed to sign JWT: %d", ret);
    mbedtls_ctr_drbg_free(&ctr_drbg);
    mbedtls_entropy_free(&entropy);
    return ESP_FAIL;
  }

  ESP_LOGI(TAG, "JWT signed successfully, signature length: %zu", sig_len);

  // Clean up
  mbedtls_ctr_drbg_free(&ctr_drbg);
  mbedtls_entropy_free(&entropy);
  mbedtls_pk_free(&pk_context);

  // Base64 encode signature
  char signature_base64[512];
  mbedtls_base64_encode((unsigned char *)signature_base64, sizeof(signature_base64), &out_len, signature, sig_len);

  // Create final JWT
  snprintf(jwt, jwt_len, "%s.%s.%s", header_base64, payload_base64, signature_base64);
  return ESP_OK;
}

/**
 * Send distance data to Google Pub/Sub
 */
static void send_to_pubsub(float distance) {
  char payload[MAX_PAYLOAD];
  snprintf(payload, MAX_PAYLOAD, "{\"messages\":[{\"data\":\"%.2f\"}]}", distance);

  esp_http_client_config_t config = {
    .url = PUBSUB_URL,
    .method = HTTP_METHOD_POST,
  };

  esp_http_client_handle_t client = esp_http_client_init(&config);
  esp_http_client_set_header(client, "Authorization", jwt_token);
  esp_http_client_set_header(client, "Content-Type", "application/json");

  esp_http_client_set_post_field(client, payload, strlen(payload));

  esp_err_t err = esp_http_client_perform(client);
  if (err == ESP_OK) {
    ESP_LOGI(TAG, "POST successful, Status = %d", esp_http_client_get_status_code(client));
  } else {
    ESP_LOGE(TAG, "HTTP POST failed: %s", esp_err_to_name(err));
  }

  esp_http_client_cleanup(client);
}

void ultrasonic(void *pvParameters) {
  ultrasonic_sensor_t sensor = {
    .trigger_pin = TRIG_PIN,
    .echo_pin    = ECHO_PIN
  };

  ultrasonic_init(&sensor);

  // Generate JWT
  if (generate_jwt(jwt_token, sizeof(jwt_token)) != ESP_OK) {
    ESP_LOGE(TAG, "Failed to generate JWT");
    vTaskDelete(NULL);
    return;
  }

  while (true) {
    float distance;
    esp_err_t res = ultrasonic_measure(&sensor, MAX_DISTANCE_CM, &distance);

    if (res != ESP_OK) {
      ESP_LOGE(TAG, "Error %d: ", res);
    } else {
      ESP_LOGI(SENSOR_TAG, "Distance: %.2f m", distance);
      send_to_pubsub(distance);
    }

    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

