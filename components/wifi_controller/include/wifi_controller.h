/**
 * @file wifi_controller.h
 * @author TaxCoding (raja.naufal1605@student.unri.ac.id)
 * @date 2026
 * @copyright Copyright (c) 2026
 * 
 * @brief Header file for Wi-Fi controller management, AP configuration, and MAC handling.
 */

#ifndef WIFI_CONTROLLER_H
#define WIFI_CONTROLLER_H

#include "esp_wifi_types.h"
#include "esp_err.h"

/**
 * @brief Maximum number of Access Points stored during scanning.
 */
#define CONFIG_SCAN_MAX_AP 20

/**
 * @brief Container structure for scanned Access Point records.
 */
typedef struct {
    uint16_t count;
    wifi_ap_record_t records[CONFIG_SCAN_MAX_AP];
} wifictl_ap_records_t;

/**
 * @brief Start the Access Point interface with given configuration.
 * 
 * @param wifi_config Pointer to the wifi_config_t structure.
 */
void wifictl_ap_start(wifi_config_t *wifi_config);

/**
 * @brief Stop the Access Point interface.
 */
void wifictl_ap_stop(void);

/**
 * @brief Set a custom MAC address for the AP interface.
 * 
 * @param mac_ap Pointer to the 6-byte MAC address array.
 */
void wifictl_set_ap_mac(const uint8_t *mac_ap);

/**
 * @brief Get the current MAC address of the AP interface.
 * 
 * @param mac_ap Pointer to the 6-byte destination array.
 */
void wifictl_get_ap_mac(uint8_t *mac_ap);

/**
 * @brief Restore the original MAC address of the AP interface.
 */
void wifictl_restore_ap_mac(void);

#endif // WIFI_CONTROLLER_H