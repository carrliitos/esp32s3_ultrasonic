/*
 * ultrasonic_sensor.h
 *
 * Created on:         2024-12-03 20:56:58
 *     Author:         carlitos (benzon.salazar@gmail.com)
 *
 * Last Modified by:   carlitos
 * Last Modified time: 2024-12-03 21:22:27
 */

#ifndef MAIN_ULTRASONIC_SENSOR_H_
#define MAIN_ULTRASONIC_SENSOR_H_

#include "driver/gpio.h"

#define ECHO_PIN         GPIO_NUM_5
#define TRIG_PIN         GPIO_NUM_4
#define MAX_DISTANCE_CM  500 // 5m max

void ultrasonic(void *pvParameters);

#endif /* MAIN_ULTRASONIC_SENSOR_H_ */