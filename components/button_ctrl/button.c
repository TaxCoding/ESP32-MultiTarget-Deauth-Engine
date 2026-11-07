/**
 * @file button.c
 * @author TaxCoding (raja.naufal1605@student.unri.ac.id)
 * @date 2026
 * @copyright Copyright (c) 2026
 * 
 * @brief Implementation of button polling, LED FSM, and mode transitions controlling Attack Engine.
 */

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "button.h"
#include "attack_dos.h"

/** @brief GPIO pin assigned to the BOOT button. */
#define BUTTON_GPIO     GPIO_NUM_0

/** @brief GPIO pin assigned to the onboard D2 blue LED. */
#define LED_D2_GPIO     GPIO_NUM_2

/** @brief Logging tag for the button controller module. */
static const char *TAG = "BUTTON_CTRL";

/** @brief Stores the current operational state machine mode. */
static system_mode_t current_mode = MODE_IDLE;

/**
 * @brief Blinks the onboard D2 LED a specified number of times with a given delay.
 * 
 * @param times Number of blink cycles.
 * @param delay_ms Delay duration in milliseconds for high and low states.
 */
static void led_blink(int times, int delay_ms) {
    for (int i = 0; i < times; i++) {
        gpio_set_level(LED_D2_GPIO, 1);
        vTaskDelay(pdMS_TO_TICKS(delay_ms));
        gpio_set_level(LED_D2_GPIO, 0);
        vTaskDelay(pdMS_TO_TICKS(delay_ms));
    }
}

/**
 * @brief Executes actions corresponding to the requested system mode, controlling both the LED indicator and the attack engine.
 * 
 * @param mode Target operational mode to transition into (@ref system_mode_t).
 */
static void execute_mode_action(system_mode_t mode) {
    switch (mode) {
        case MODE_IDLE:
            ESP_LOGI(TAG, "Sistem IDLE: Menghentikan semua serangan & mematikan LED.");
            attack_dos_stop();
            gpio_set_level(LED_D2_GPIO, 0);
            break;
            
        case MODE_1:
            ESP_LOGI(TAG, "Masuk ke MODE 1: Menyerang 1 Target.");
            led_blink(1, 200);
            gpio_set_level(LED_D2_GPIO, 1);
            attack_dos_set_mode_limit(1);
            attack_dos_start_multi();
            break;
            
        case MODE_2:
            ESP_LOGI(TAG, "Masuk ke MODE 2: Menyerang hingga 3 Target.");
            attack_dos_stop();
            gpio_set_level(LED_D2_GPIO, 0);
            vTaskDelay(pdMS_TO_TICKS(300));
            led_blink(2, 200);
            gpio_set_level(LED_D2_GPIO, 1);
            attack_dos_set_mode_limit(3);
            attack_dos_start_multi();
            break;
            
        case MODE_3:
            ESP_LOGI(TAG, "Masuk ke MODE 3: Menyerang Semua Target (Max 5).");
            attack_dos_stop();
            gpio_set_level(LED_D2_GPIO, 0);
            vTaskDelay(pdMS_TO_TICKS(300));
            led_blink(3, 200);
            gpio_set_level(LED_D2_GPIO, 1);
            attack_dos_set_mode_limit(5);
            attack_dos_start_multi();
            break;
            
        default:
            break;
    }
}

/**
 * @brief FreeRTOS background task for polling the BOOT button, managing debounce intervals, and driving mode transitions.
 * 
 * @param pvParameter Task parameters pointer (unused).
 */
static void button_monitor_task(void *pvParameter) {
    bool last_button_state = true;

    while (1) {
        bool current_button_state = gpio_get_level(BUTTON_GPIO);

        if (last_button_state && !current_button_state) {
            vTaskDelay(pdMS_TO_TICKS(50));
            if (gpio_get_level(BUTTON_GPIO) == 0) {
                
                if (current_mode == MODE_3) {
                    ESP_LOGI(TAG, "Keluar dari mode aktif, kembali ke IDLE.");
                    gpio_set_level(LED_D2_GPIO, 0);
                    vTaskDelay(pdMS_TO_TICKS(200));
                    led_blink(5, 80);
                    current_mode = MODE_IDLE;
                    execute_mode_action(current_mode);
                } else {
                    current_mode = (system_mode_t)(current_mode + 1);
                    execute_mode_action(current_mode);
                }

                while (gpio_get_level(BUTTON_GPIO) == 0) {
                    vTaskDelay(pdMS_TO_TICKS(10));
                }
            }
        }
        last_button_state = current_button_state;
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

/**
 * @brief Initializes GPIO configurations for the BOOT button and LED D2, and spawns the background monitor task.
 */
void button_ctrl_init(void) {
    gpio_config_t led_conf = {
        .pin_bit_mask = (1ULL << LED_D2_GPIO),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&led_conf);
    gpio_set_level(LED_D2_GPIO, 0);

    gpio_config_t btn_conf = {
        .pin_bit_mask = (1ULL << BUTTON_GPIO),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&btn_conf);

    xTaskCreatePinnedToCore(
        button_monitor_task, 
        "button_monitor_task", 
        2048, 
        NULL, 
        5, 
        NULL, 
        0
    );

    ESP_LOGI(TAG, "Button & LED controller initialized successfully.");
}

/**
 * @brief Retrieves the current active system operational mode.
 * 
 * @return system_mode_t The active mode enum value (@ref system_mode_t).
 */
system_mode_t button_ctrl_get_current_mode(void) {
    return current_mode;
}