/*
 * key_loader.h
 *
 * Created on:         2024-12-04 00:49:50
 *     Author:         carrliitos (benzon.salazar@gmail.com)
 *
 * Last Modified by:   carrliitos
 * Last Modified time: 2024-12-04 00:51:45
 */

#ifndef MAIN_KEY_LOADER_H_
#define MAIN_KEY_LOADER_H_

#include "esp_err.h"

/**
 * Initialize the filesystem (SPIFFS).
 */
esp_err_t init_filesystem();

/**
 * Load the private key from a JSON file.
 * @return Pointer to the private key string, or NULL if loading failed.
 */
const char* load_private_key();

#endif /* MAIN_KEY_LOADER_H_ */