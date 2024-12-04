/*
 * ultrasonic_sensor.c
 *
 * Created on:         2024-12-03 20:56:44
 *     Author:         carlitos (benzon.salazar@gmail.com)
 *
 * Last Modified by:   carlitos
 * Last Modified time: 2024-12-03 21:30:11
 */

#include <stdio.h>
#include <stdbool.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "ultrasonic.h" // From esp-idf-lib
#include "esp_log.h"

#include "ultrasonic_sensor.h"

#define TAG           "Sensor"
#define SENSOR_TAG    "HC-SR04"

void ultrasonic(void *pvParameters) {
  ultrasonic_sensor_t sensor = {
    .trigger_pin = TRIG_PIN,
    .echo_pin    = ECHO_PIN
  };

  ultrasonic_init(&sensor);

  while (true){
    float distance;
    esp_err_t res = ultrasonic_measure(&sensor, MAX_DISTANCE_CM, &distance);

    if (res != ESP_OK) {
      ESP_LOGE(TAG, "Error %d: ", res);
      switch (res) {
        case ESP_ERR_ULTRASONIC_PING:
          ESP_LOGE(TAG, "Cannot ping (device is in invalid state)");
          break;
        case ESP_ERR_ULTRASONIC_PING_TIMEOUT:
          ESP_LOGE(TAG, "Ping timeout (no device found)");
          break;
        case ESP_ERR_ULTRASONIC_ECHO_TIMEOUT:
          ESP_LOGE(TAG, "Echo timeout (i.e. distance too big)");
          break;
        default:
          printf("%s\n", esp_err_to_name(res));
      }
    }
    else
      ESP_LOGI(SENSOR_TAG, "Distance: %0.04f m", distance);

    vTaskDelay(pdMS_TO_TICKS(500));
  }
}
