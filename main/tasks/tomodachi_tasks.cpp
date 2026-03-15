
#include "character/character.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// tick the pet every 1 minute (1000ms * 60)
void tick_pet_task(void *pvParameters) {
    while (1) {
        tick_pet();
        vTaskDelay(pdMS_TO_TICKS(1000 * 60));
    }
}