/**
 * @file wsl_bypasser.h
 * @author TaxCoding (raja.naufal1605@student.unri.ac.id)
 * @date 2026
 * @copyright Copyright (c) 2026
 * 
 * @brief Header file for raw 802.11 frame injection and deauth bypass operations.
 */

#ifndef WSL_BYPASSER_H
#define WSL_BYPASSER_H

#include <stdint.h>
#include "esp_wifi_types.h"

/**
 * @brief Transmit a raw 802.11 frame via the Wi-Fi interface.
 * 
 * @param frame_buffer Pointer to the raw frame byte array.
 * @param size Size of the frame buffer in bytes.
 */
void wsl_bypasser_send_raw_frame(const uint8_t *frame_buffer, int size);

/**
 * @brief Construct and transmit an IEEE 802.11 deauthentication frame for a target AP.
 * 
 * @param ap_record Pointer to the target wifi_ap_record_t structure.
 */
void wsl_bypasser_send_deauth_frame(const wifi_ap_record_t *ap_record);

#endif // WSL_BYPASSER_H