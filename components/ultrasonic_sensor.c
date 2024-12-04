/*
 * ultrasonic_sensor.c
 *
 * Created on:         2024-12-03 20:56:44
 *     Author:         carlitos (benzon.salazar@gmail.com)
 *
 * Last Modified by:   carrliitos
 * Last Modified time: 2024-12-04 05:02:02
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

void write_to_csv(const char *filename, float distance) {
  FILE *file = fopen(filename, "a");  // Open in append mode
  if (file != NULL) {
    fprintf(file, "%.2f\n", distance);  // Write distance to the file
    fclose(file);
  } else {
    printf("Failed to open file for writing\n");
  }
}

void ultrasonic(void *pvParameters) {
  ultrasonic_sensor_t sensor = {
    .trigger_pin = TRIG_PIN,
    .echo_pin    = ECHO_PIN
  };

  ultrasonic_init(&sensor);

  const char *csv_file = "./data/ultrasonic_data.csv";
  while (true) {
    float distance;
    esp_err_t res = ultrasonic_measure(&sensor, MAX_DISTANCE_CM, &distance);

    if (res != ESP_OK) {
      ESP_LOGE(TAG, "Error %d: ", res);
    } else {
      write_to_csv(csv_file, distance);
      ESP_LOGI(SENSOR_TAG, "Distance: %.2f m", distance);
    }

    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

