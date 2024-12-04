/*
 * ultrasonic_sensor.h
 *
 * Created on:         2024-12-03 20:56:58
 *     Author:         carlitos (benzon.salazar@gmail.com)
 *
 * Last Modified by:   carrliitos
 * Last Modified time: 2024-12-04 05:01:02
 */

#ifndef MAIN_ULTRASONIC_SENSOR_H_
#define MAIN_ULTRASONIC_SENSOR_H_

#include "driver/gpio.h"

#define ECHO_PIN         GPIO_NUM_5
#define TRIG_PIN         GPIO_NUM_4
#define MAX_DISTANCE_CM  500 // 5m max

void write_to_csv(const char *filename, float distance);
void ultrasonic(void *pvParameters);

#endif /* MAIN_ULTRASONIC_SENSOR_H_ */