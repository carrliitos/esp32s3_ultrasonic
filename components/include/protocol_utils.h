/*
 * protocol_utils.h
 *
 * Created on:         2024-12-04 04:39:16
 *     Author:         carrliitos (benzon.salazar@gmail.com)
 *
 * Last Modified by:   carrliitos
 * Last Modified time: 2024-12-04 04:44:39
 */

#ifndef MAIN_PROTOCOL_UTILS_H_
#define MAIN_PROTOCOL_UTILS_H_

#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Encode an URI
 *
 * @param dest       a destination memory location
 * @param src        the source string
 * @param len        the length of the source string
 * @return uint32_t  the count of escaped characters
 *
 * @note Please allocate the destination buffer keeping in mind that encoding a
 *       special character will take up 3 bytes (for '%' and two hex digits).
 *       In the worst-case scenario, the destination buffer will have to be 3 times
 *       that of the source string.
 */
uint32_t uri_encode(char *dest, const char *src, size_t len);

/**
 * @brief Decode an URI
 *
 * @param dest  a destination memory location
 * @param src   the source string
 * @param len   the length of the source string
 *
 * @note Please allocate the destination buffer keeping in mind that a decoded
 *       special character will take up 2 less bytes than its encoded form.
 *       In the worst-case scenario, the destination buffer will have to be
 *       the same size that of the source string.
 */
void uri_decode(char *dest, const char *src, size_t len);

#ifdef __cplusplus
}
#endif

#endif /* MAIN_PROTOCOL_UTILS_H_ */