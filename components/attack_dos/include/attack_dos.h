/**
 * @file attack_dos.h
 * @author TaxCoding (raja.naufal1605@student.unri.ac.id)
 * @date 2026
 * @copyright Copyright (c) 2026
 * 
 * @brief Header file for the Wi-Fi DoS/Deauth attack module.
 */

#ifndef ATTACK_DOS_H
#define ATTACK_DOS_H

#include "esp_wifi_types.h"
#include "wifi_controller.h"

/**
 * @brief Configuration structure for single-target attack parameters.
 */
typedef struct {
    int method;
    uint8_t target_channel;
    char target_ssid[33];
    uint8_t target_bssid[6];
} attack_config_t;

/**
 * @brief Initialize the attack module.
 */
void attack_dos_init(void);

/**
 * @brief Start single-target attack execution.
 * 
 * @param attack_config Pointer to the attack configuration structure.
 */
void attack_dos_start(attack_config_t *attack_config);

/**
 * @brief Halt active attack execution and stop Wi-Fi operations.
 */
void attack_dos_stop(void);

/**
 * @brief Register a target AP record into the multi-target tracking array.
 * 
 * @param record Pointer to the target wifi_ap_record_t structure.
 */
void attack_dos_add_target(const wifi_ap_record_t *record);

/**
 * @brief Launch the multi-target round-robin attack engine.
 */
void attack_dos_start_multi(void);

/**
 * @brief Set the maximum number of active targets based on the current system operational mode.
 * 
 * @param limit Maximum number of targets to attack (1 to MAX_TARGETS).
 */
void attack_dos_set_mode_limit(int limit);

#endif // ATTACK_DOS_H