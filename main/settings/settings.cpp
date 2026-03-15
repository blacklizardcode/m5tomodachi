// std imports
#include "string.h"

// esp idf imports
#include "esp_netif.h"
#include "esp_wifi.h"

// local imports
#include "settings/settings.hpp"
#include "utils/utils_webserver.hpp"


wifi_mode_t settings_wifi_mode;
char settings_wifi_ssid[33];
char settings_wifi_pass[64];
static TaskHandle_t s_webserver_task_handle = nullptr;

esp_err_t settings_start_webserver();
esp_err_t settings_stop_webserver();
esp_err_t settings_start_wifi();
esp_err_t settings_stop_wifi();


esp_err_t settings_start_wifi() {
    constexpr size_t kSsidMaxLen = 32;
    constexpr size_t kPassphraseMaxLen = 63;

    wifi_init_config_t conf = WIFI_INIT_CONFIG_DEFAULT();
    esp_err_t err = esp_wifi_init(&conf);
    if (err != ESP_OK) {
        return err;
    }

    wifi_config_t wifi_config = {0};

    if (settings_wifi_mode == WIFI_MODE_STA) {
        size_t ssid_len = strnlen(settings_wifi_ssid, sizeof(settings_wifi_ssid));
        size_t pass_len = strnlen(settings_wifi_pass, sizeof(settings_wifi_pass));

        if (ssid_len == 0 || ssid_len > kSsidMaxLen || pass_len > kPassphraseMaxLen) {
            return ESP_ERR_INVALID_ARG;
        }

        memcpy(wifi_config.sta.ssid, settings_wifi_ssid, ssid_len);
        memcpy(wifi_config.sta.password, settings_wifi_pass, pass_len);

        esp_netif_create_default_wifi_sta();
        err = esp_wifi_set_mode(WIFI_MODE_STA);
        if (err != ESP_OK) {
            return err;
        }

        err = esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
        if (err != ESP_OK) {
            return err;
        }
    }
    else if (settings_wifi_mode == WIFI_MODE_AP) {
        size_t ssid_len = strnlen(settings_wifi_ssid, sizeof(settings_wifi_ssid));
        size_t pass_len = strnlen(settings_wifi_pass, sizeof(settings_wifi_pass));

        if (ssid_len == 0 || ssid_len > kSsidMaxLen || pass_len > kPassphraseMaxLen) {
            return ESP_ERR_INVALID_ARG;
        }

        memcpy(wifi_config.ap.ssid, settings_wifi_ssid, ssid_len);
        memcpy(wifi_config.ap.password, settings_wifi_pass, pass_len);

        wifi_config.ap.ssid_len = ssid_len;
        wifi_config.ap.max_connection = 4;
        wifi_config.ap.authmode = WIFI_AUTH_WPA_WPA2_PSK;

        if (pass_len == 0) {
            wifi_config.ap.authmode = WIFI_AUTH_OPEN;
        }

        esp_netif_create_default_wifi_ap();
        err = esp_wifi_set_mode(WIFI_MODE_AP);
        if (err != ESP_OK) {
            return err;
        }

        err = esp_wifi_set_config(WIFI_IF_AP, &wifi_config);
        if (err != ESP_OK) {
            return err;
        }
    }
    else {
        return ESP_ERR_INVALID_ARG;
    }

    err = esp_wifi_start();
    if (err != ESP_OK) {
        return err;
    }

    if (settings_wifi_mode == WIFI_MODE_STA) {
        err = esp_wifi_connect();
        if (err != ESP_OK) {
            return err;
        }
    }

    return ESP_OK;
}

esp_err_t settings_stop_wifi() {
    wifi_mode_t mode;

    if (esp_wifi_get_mode(&mode) == ESP_OK) {
        if (mode & WIFI_MODE_STA) {
            esp_wifi_disconnect();
        }
    }

    esp_wifi_stop();

    esp_wifi_deinit();

    return ESP_OK;
}

esp_err_t settings_start_webserver() {
    if (s_webserver_task_handle != nullptr) {
        return ESP_OK;
    }

    BaseType_t created = xTaskCreate(
        webserver_task,
        "webserver",
        20480,
        NULL,
        10,
        &s_webserver_task_handle
    );

    return (created == pdPASS) ? ESP_OK : ESP_FAIL;
}

esp_err_t settings_stop_webserver() {
    if (s_webserver_task_handle == nullptr) {
        return ESP_ERR_INVALID_STATE;
    }

    vTaskDelete(s_webserver_task_handle);
    s_webserver_task_handle = nullptr;
    return ESP_OK;
}