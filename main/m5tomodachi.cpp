#include <stdio.h>

// esp-idf imports
#include "M5Unified.hpp"
#include "esp_littlefs.h"
#include "esp_log.h"
#include "freertos/task.h"

// local imports
#include "character.hpp"
#include "ui.hpp"

void onStart();
void tick_pet_task(void *pvParameters);

extern "C" void app_main(void)
{   
    onStart();
    M5.Display.clear();
}

void onStart() {
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

    esp_err_t ret = esp_vfs_littlefs_register(&conf);
    if (ret != ESP_OK) {
        ESP_LOGE("littlefs", "Failed to mount LittleFS partition 'fs': %s", esp_err_to_name(ret));
        return;
    }

    // init the pet
    init_pet();

    xTaskCreate(tick_pet_task, "pet_tick", 4096, NULL, 5, NULL);
}

// tick the pet every 1 minute (1000ms * 60)
void tick_pet_task(void *pvParameters) {
    while (1) {
        tick_pet();
        vTaskDelay(pdMS_TO_TICKS(1000 * 60));
    }
    
}