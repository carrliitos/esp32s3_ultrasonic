/*
 * key_loader.c
 *
 * Created on:         2024-12-04 00:50:00
 *     Author:         carrliitos (benzon.salazar@gmail.com)
 *
 * Last Modified by:   carrliitos
 * Last Modified time: 2024-12-04 01:18:39
 */

#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "cJSON.h"
#include "esp_spiffs.h"

#define TAG "KeyLoader"
#define JSON_FILE_PATH "/spiffs_image/private_key.json"

static char private_key[2048] = {0};

/**
 * Initialize the SPIFFS file system
 */
esp_err_t init_filesystem() {
  esp_vfs_spiffs_conf_t conf = {
    .base_path = "/spiffs_image",
    .partition_label = NULL,
    .max_files = 5,
    .format_if_mount_failed = true
  };
  esp_err_t ret = esp_vfs_spiffs_register(&conf);
  if (ret != ESP_OK) {
    ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
    return ret;
  }
  return ESP_OK;
}

/**
 * Load the private key from the JSON file
 */
const char* load_private_key() {
  FILE *file = fopen(JSON_FILE_PATH, "r");
  if (file == NULL) {
    ESP_LOGE(TAG, "Failed to open file: %s", JSON_FILE_PATH);
    return NULL;
  }

  char buffer[4096];
  size_t len = fread(buffer, 1, sizeof(buffer) - 1, file);
  fclose(file);

  if (len == 0) {
    ESP_LOGE(TAG, "Failed to read file: %s", JSON_FILE_PATH);
    return NULL;
  }

  buffer[len] = '\0'; // Null-terminate the buffer
  cJSON *json = cJSON_Parse(buffer);
  if (json == NULL) {
    ESP_LOGE(TAG, "Failed to parse JSON");
    return NULL;
  }

  cJSON *key_item = cJSON_GetObjectItem(json, "private_key");
  if (key_item == NULL || !cJSON_IsString(key_item)) {
    ESP_LOGE(TAG, "Private key not found in JSON");
    cJSON_Delete(json);
    return NULL;
  }

  strncpy(private_key, key_item->valuestring, sizeof(private_key) - 1);
  cJSON_Delete(json);

  ESP_LOGI(TAG, "Private key loaded successfully");
  return private_key;
}
