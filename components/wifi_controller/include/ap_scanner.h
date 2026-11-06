/**
 * @file ap_scanner.h
 * @author TaxCoding (raja.naufal1605@student.unri.ac.id)
 * @date 2026
 * @copyright Copyright (c) 2026
 * 
 * @brief Header file for Wi-Fi active scanning and AP record management.
 */

#ifndef AP_SCANNER_H
#define AP_SCANNER_H

#include "wifi_controller.h"

/**
 * @brief Perform an active scan for nearby Access Points.
 */
void wifictl_scan_nearby_aps(void);

/**
 * @brief Retrieve the global structure containing all scanned AP records.
 * 
 * @return const wifictl_ap_records_t* Pointer to the AP records container.
 */
const wifictl_ap_records_t *wifictl_get_ap_records(void);

/**
 * @brief Retrieve a specific AP record by index from the scanned list.
 * 
 * @param index Array index of the target AP record.
 * @return const wifi_ap_record_t* Pointer to the AP record, or NULL if out of bounds.
 */
const wifi_ap_record_t *wifictl_get_ap_record(unsigned index);

#endif // AP_SCANNER_H