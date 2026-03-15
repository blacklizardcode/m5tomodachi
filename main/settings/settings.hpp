#ifndef SETTINGS_HPP

#define SETTINGS_HPP

#include "esp_wifi.h"

// global settings values
extern wifi_mode_t settings_wifi_mode;
extern char settings_wifi_ssid[33];
extern char settings_wifi_pass[64];

// definitions
#define XOR_KEY "1TObUQXshE"
#define TAG "Tomodachi"

// functions
esp_err_t settings_start_webserver();
esp_err_t settings_stop_webserver();
esp_err_t settings_start_wifi();
esp_err_t settings_stop_wifi();

#endif