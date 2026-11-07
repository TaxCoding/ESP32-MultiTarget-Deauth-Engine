# ESP32 Multi-Target Wi-Fi Deauth Engine

Low-level firmware implementation for high-performance multi-target Wi-Fi testing on **ESP32-D0WD-V3 (revision 3)** using ESP-IDF v4.1.3.

## Features
- **Raw Frame Injection:** Bypasses TCP/IP stack via `wsl_bypasser` and `esp_wifi_80211_tx`.
- **Core Isolation:** Utilizes FreeRTOS `xTaskCreatePinnedToCore` to isolate attack tasks on Core 1.
- **Multi-Target Round-Robin:** Dynamic channel hopping and time-division multiplexing across scanned APs.
- **Buffer Flow Control:** Microsecond-level precision (`ets_delay_us`) to prevent TX queue overflow.
- **Hardware FSM Control:** Interactive state machine managed via the onboard BOOT button and D2 LED indicator.

## Hardware Controls & Operational Modes
- **BOOT Button (GPIO 0):** Cycles through operational modes sequentially.
- **LED D2 (GPIO 2):** Real-time visual indicator for system states and transitions.
  - **Idle (Mode 0):** Initial boot state. LED is **OFF** and all attack tasks are completely halted.
  - **Mode 1:** Blinks **1x**, LED stays **ON**, executes attacks on **1 target**.
  - **Mode 2:** Blinks **2x**, LED stays **ON**, executes attacks on **up to 3 targets**.
  - **Mode 3:** Blinks **3x**, LED stays **ON**, executes attacks on **up to 5 targets**. *(Pressing the button again flashes **5 rapid blinks** and safely returns the system to Idle).*

## Project Structure
- `components/attack_dos/` - Core multi-target attack engine and task scheduler.
- `components/ap_scanner/` - Active scanning module for BSSID/SSID discovery.
- `components/wifi_controller/` - Stack initialization, APSTA mode, and MAC handling.
- `components/wsl_bypasser/` - Raw frame transmitter and sanity check overrides.
- `components/button_ctrl/` - FSM button polling, debounce logic, and D2 LED state manager.
- `main/` - Application entry point (`app_main`).

## Getting Started
### Prerequisites
- ESP-IDF v4.1.3 toolchain configured.
- ESP32-D0WD-V3 development board (4MB Flash).

### Build & Flash
    idf.py fullclean
    idf.py -p COM8 flash monitor

### Flash Only (Using esptool.py)
    esptool.py -p COM8 -b 115200 --after hard_reset write_flash \
        --flash_mode dio --flash_freq 40m --flash_size detect \
        0x8000 build/partition_table/partition-table.bin \
        0x1000 build/bootloader/bootloader.bin \
        0x10000 build/belajarKomponenDOS.bin

## Theoretical Background
For an in-depth analysis of the underlying RF physics, FreeRTOS scheduling, and memory management, read the [Theory Document](theory.md).