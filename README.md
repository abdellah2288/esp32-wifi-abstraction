# ESP32 WiFi Abstraction Layer

This repository contains a simple WiFi abstraction layer for ESP32 microcontrollers written in C, and it is licensed under GPL v2. The abstraction layer provides functionality for initializing WiFi in either station or access point mode, scanning for available access points, and handling WiFi events. The code is designed to be easy to integrate into ESP32 projects, providing a clean interface for managing WiFi connectivity.
Features

  - Station and Access Point Modes: Initialize WiFi in either station or access point mode using the init_wifi function.

  - WiFi Event Handling: The library includes an event handler for WiFi events, focusing on connect/disconnect events in station mode. You can easily extend the event handling to accommodate other event types.

  - Access Point Configuration: Configure custom parameters for the access point when operating in access point mode, such as SSID, password, channel, and more.

  - Scanning for Access Points: Use the wifi_scan function to scan for available access points. The results are returned in a structured format.

## Prerequisites

Before using this WiFi abstraction layer, ensure you have the ESP-IDF development environment set up on your system. You can find instructions on setting up the ESP-IDF framework [here](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/).
## Usage

- Include the necessary headers in your ESP-IDF project:

```
#include "ep_wifi.h"
```
- Define the necessary macros for custom access point configuration, such as SSID, password, channel, etc.

- Initialize the WiFi and IP event loop:
```
init_wifi_event_loop(void);
```
- Initialize non-volatile storage:

```
    initialize_nvs(void);
```
- Use the init_wifi function to initialize WiFi in either station or access point mode.

- Handle WiFi events using the provided event handler wifi_event_handler.

- Scan for available access points using the wifi_scan function.

## Contributing

Contributions are welcome! If you find any issues or have improvements to suggest, please open an issue or submit a pull request.
License

This project is licensed under the GNU General Public License v2.0.
