/*
 * main.c
 *
 * Created on:         2024-12-03 20:04:51
 *     Author:         carlitos (benzon.salazar@gmail.com)
 *
 * Last Modified by:   carlitos
 * Last Modified time: 2024-12-03 21:03:15
 */

#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "ultrasonic_sensor.h"

void app_main(void) {
  xTaskCreate(ultrasonic, "ultrasonic", configMINIMAL_STACK_SIZE * 3, NULL, 5, NULL);
}
