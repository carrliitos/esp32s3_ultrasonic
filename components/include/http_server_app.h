/*
 * http_server_app.h
 *
 * Created on:         2024-12-04 02:58:08
 *     Author:         carrliitos (benzon.salazar@gmail.com)
 *
 * Last Modified by:   carrliitos
 * Last Modified time: 2024-12-04 04:33:52
 */

#ifndef MAIN_HTTP_SERVER_APP_H_
#define MAIN_HTTP_SERVER_APP_H_

#include "esp_http_server.h"

#define BASIC_AUTH_USERNAME "ESP32"
#define BASIC_AUTH_PASSWORD "ESP32Webserver"

/** 
* Function to start the HTTP server
*/
httpd_handle_t http_server_start(void);

/**
* Function to stop the HTTP server
*/
void http_server_stop(httpd_handle_t server);

/* HTTP-related functions moved from wifi_app.c */
esp_err_t send_data_to_web_server(float distance);

#endif /* MAIN_HTTP_SERVER_APP_H_ */