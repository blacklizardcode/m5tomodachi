#include <stdio.h>
#include <string.h>

// esp-idf imports
#include "M5Unified.hpp"
#include "esp_littlefs.h"
#include "esp_log.h"
#include "freertos/task.h"
#include "esp_wifi.h"
#include "esp_netif.h"
#include "nvs_flash.h"

// local imports
#include "character/character.hpp"
#include "ui/ui.hpp"
#include "utils/utils.hpp"
#include "utils/utils_webserver.hpp"
#include "tasks/tomodachi_tasks.hpp"
#include "settings/settings.hpp"
#include "ui/spritesheet/spritesheet.hpp"


esp_err_t onStart();
esp_err_t init_wifi();
void tick_pet_task(void *pvParameters);

extern "C" void app_main(void)
{   
    auto err = onStart();
    if (err != 0 ) {
        return;
    }
}

esp_err_t onStart() {
    esp_err_t err;

    // Init the nvs flash for wifi and persitent storage
    err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    if (err != ESP_OK) {
        return err;
    }

    // init the network stack
    //? might move this to the wifi init function
    err = esp_netif_init();
    if (err != ESP_OK) {
        return err;
    }

    // create the default event loop
    err = esp_event_loop_create_default();
    if (err != ESP_OK && err != ESP_ERR_INVALID_STATE) {
        return err;
    }

    // init the filesystem
    esp_vfs_littlefs_conf_t conf = {
        .base_path = "/filesystem",
        .partition_label = "fs",
        .partition = nullptr,
        .format_if_mount_failed = true,
        .read_only = false,
        .dont_mount = false,
        .grow_on_mount = false,
    };

    err = esp_vfs_littlefs_register(&conf);
    if (err != ESP_OK) {
        return err;
    }

    err = init_wifi();
    if (err != 0) {
        return err;
    }

    // TODO: Turn into a ui option 
    settings_wifi_mode = WIFI_MODE_STA;
    strlcpy(settings_wifi_ssid, "Tommy", sizeof(settings_wifi_ssid));
    strlcpy(settings_wifi_pass, "nrbGt7jvuzFp", sizeof(settings_wifi_pass));
    err = settings_start_wifi();
    if (err != ESP_OK) {
        return err;
    }

    // TODO: Turn into a ui option 
    settings_start_webserver();

    // init the pet
    err = init_pet();
    if (err != ESP_OK) {
        return err;
    }

    xTaskCreate(tick_pet_task, "pet_tick", 20480, NULL, 5, NULL);
    return 0;
}

esp_err_t init_wifi() {
    
    return ESP_OK;
}

