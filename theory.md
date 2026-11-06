# Theoretical Background & Systems Analysis

This document outlines the theoretical foundations, architectural decisions, and physical constraints governing the ESP32 multi-target deauthentication engine.

## 1. IEEE 802.11 Layer 2 Injections
Standard Wi-Fi communication relies on a structured protocol stack (OSI layers). However, management frames such as Deauthentication (`0xc0`) operate at Layer 2 (Data Link Layer). 
- By injecting raw frames directly into the air via `esp_wifi_80211_tx`, the device bypasses association requirements.
- The internal driver sanity checks are overridden at the linker level (`ieee80211_raw_frame_sanity_check`) to permit arbitrary frame structures.

## 2. FreeRTOS Dual-Core Optimization (Core Pinning)
The ESP32 features a dual-core Tensilica Xtensa architecture (PRO_CPU / Core 0 and APP_CPU / Core 1).
- **Core 0** is dedicated by ESP-IDF to handle background Wi-Fi driver tasks and TCP/IP stack operations.
- **Core 1** hosts our `hyper_attack_task` via `xTaskCreatePinnedToCore`. This hardware-level isolation prevents CPU contention and context-switching jitter, ensuring consistent packet burst timing.

## 3. Flow Control & Preventing TX Buffer Exhaustion (`ESP_ERR_NO_MEM`)
High-frequency transmission tests often trigger `ESP_ERR_NO_MEM` (Error `0x101`).
- **The Cause:** Pushing frames faster than the physical radio transceiver can modulate and transmit them saturates the internal driver TX queue.
- **The Solution:** Replacing FreeRTOS tick-based delays (`vTaskDelay`) with ROM-based microsecond delays (`ets_delay_us`) allows precise burst-density throttling, ensuring buffer recovery without kernel panics.

## 4. RF Physics & Spatial Limitations
Software optimization is bound by physical laws.
- **Free Space Path Loss:** RF signal strength degrades exponentially over distance according to the inverse-square law.
- **Link Asymmetry:** A software-level attack is constrained by the physical transmission range of the ESP32's onboard antenna. Targets outside the effective RF propagation radius cannot receive the injected frames, regardless of software transmission rates.