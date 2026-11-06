/**
 * @file ap_scanner.c
 * @author TaxCoding (raja.naufal1605@student.unri.ac.id)
 * @date 2026
 * @copyright Copyright (c) 2026
 * 
 * @brief Implementation of active Wi-Fi scanning and Access Point record management.
 */

#include "ap_scanner.h"
#include "esp_log.h"
#include "esp_wifi.h"

static const char* TAG = "ap_scanner";
static wifictl_ap_records_t ap_records;

/**
 * @brief Initialize the Wi-Fi stack in APSTA mode if uninitialized and perform an active scan for nearby Access Points.
 */
void wifictl_scan_nearby_aps() {
    ESP_LOGI(TAG, "Scanning nearby Access Points...");

    ap_records.count = CONFIG_SCAN_MAX_AP;
    wifi_scan_config_t scan_config = {
        .ssid = NULL,
        .bssid = NULL,
        .channel = 0,
        .scan_type = WIFI_SCAN_TYPE_ACTIVE
    };
    
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_err_t err = esp_wifi_init(&cfg);
    if (err == ESP_ERR_WIFI_INIT_STATE) {
        // Driver already initialized, skip initialization
    } else {
        ESP_ERROR_CHECK(err);
        ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));
        ESP_ERROR_CHECK(esp_wifi_start());
    }

    ESP_ERROR_CHECK(esp_wifi_scan_start(&scan_config, true));
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&ap_records.count, ap_records.records));
    ESP_LOGI(TAG, "Found %u APs.", ap_records.count);
}

/**
 * @brief Retrieve the global structure containing all scanned AP records.
 * 
 * @return const wifictl_ap_records_t* Pointer to the AP records container.
 */
const wifictl_ap_records_t *wifictl_get_ap_records() {
    return &ap_records;
}

/**
 * @brief Retrieve a specific AP record by index from the scanned list.
 * 
 * @param index Array index of the target AP record.
 * @return const wifi_ap_record_t* Pointer to the AP record, or NULL if out of bounds.
 */
const wifi_ap_record_t *wifictl_get_ap_record(unsigned index) {
    if (index >= ap_records.count) {
        ESP_LOGE(TAG, "Index out of bounds! Available: %u, Requested: %u", ap_records.count, index);
        return NULL;
    }
    return &ap_records.records[index];
}