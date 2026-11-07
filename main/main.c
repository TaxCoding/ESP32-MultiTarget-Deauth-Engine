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
#include "button.h"

/**
 * @brief Main application entry point. Handles system bootstrap, NVS initialization, 
 *        AP scanning, target registration, and hands over control to the button FSM controller.
 */
void app_main(void) {
    ESP_LOGI("main", "System initialization...");

    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    attack_dos_init();
    wifictl_scan_nearby_aps();
    
    const wifictl_ap_records_t *records = wifictl_get_ap_records();
    if (records->count == 0) {
        ESP_LOGW("main", "No Access Points found!");
        return;
    }

    int limit = records->count < 5 ? records->count : 5;
    for (int i = 0; i < limit; i++) {
        attack_dos_add_target(&records->records[i]);
    }

    button_ctrl_init();
    
    ESP_LOGI("main", "Initialization complete. System is IDLE. Press BOOT button to start modes.");
}