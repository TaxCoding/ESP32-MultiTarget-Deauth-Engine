/**
 * @file wifi_controller.c
 * @author TaxCoding (raja.naufal1605@student.unri.ac.id)
 * @date 2026
 * @copyright Copyright (c) 2026
 * 
 * @brief Management of Wi-Fi stack initialization, APSTA mode, MAC spoofing, and AP configurations.
 */

#include "wifi_controller.h"
#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "esp_wifi.h"
#include "esp_netif.h"
#include "esp_event.h"

static const char* TAG = "wifi_controller";
static bool wifi_init = false;
static uint8_t original_mac_ap[6];

/**
 * @brief Generic event handler for Wi-Fi events.
 */
static void wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data) {}

/**
 * @brief Initialize the Wi-Fi network interface stack in APSTA mode.
 */
static void wifi_init_apsta() {
    esp_netif_init();
    esp_netif_create_default_wifi_ap();
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&wifi_init_config));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL));

    esp_wifi_get_mac(WIFI_IF_AP, original_mac_ap);
    ESP_ERROR_CHECK(esp_wifi_start());
    wifi_init = true;
}

/**
 * @brief Start the Access Point interface with specified configurations.
 * 
 * @param wifi_config Pointer to the wifi_config_t configuration structure.
 */
void wifictl_ap_start(wifi_config_t *wifi_config) {
    if (!wifi_init) {
        wifi_init_apsta();
    }
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_AP, wifi_config));
    ESP_LOGI(TAG, "AP started with SSID=%s", wifi_config->ap.ssid);
}

/**
 * @brief Stop the Access Point interface.
 */
void wifictl_ap_stop() {
    wifi_config_t wifi_config = { .ap = { .max_connection = 0 } };
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_AP, &wifi_config));
}

/**
 * @brief Set a custom MAC address for the AP interface (MAC spoofing).
 * 
 * @param mac_ap Pointer to the 6-byte MAC address array.
 */
void wifictl_set_ap_mac(const uint8_t *mac_ap) {
    if (!wifi_init) {
        wifi_init_apsta();
    }
    ESP_ERROR_CHECK(esp_wifi_set_mac(WIFI_IF_AP, mac_ap));
}

/**
 * @brief Retrieve the current MAC address of the AP interface.
 * 
 * @param mac_ap Pointer to the 6-byte destination array.
 */
void wifictl_get_ap_mac(uint8_t *mac_ap) {
    esp_wifi_get_mac(WIFI_IF_AP, mac_ap);
}