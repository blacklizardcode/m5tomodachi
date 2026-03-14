// std imports
#include <stdint.h>
#include <stdio.h>

// esp-idf imports
#include "esp_littlefs.h"
#include "esp_log.h"

// local imports
#include "character.hpp"
#include "utils.hpp"

M5Tomodachi pet;

int init_pet() {
    M5Tomodachi filePet = read_parse_character_file();

    pet.hunger = (filePet.hunger == NULL) ? 0 : filePet.hunger;
    pet.happiness = (filePet.happiness == NULL) ? 0 : filePet.happiness;
    pet.energy = (filePet.energy == NULL) ? 0 : filePet.energy;
    pet.health = (filePet.health == NULL) ? 0 : filePet.health;
    pet.age = (filePet.age == NULL) ? 0 : filePet.age;
    pet.stage = (filePet.stage == NULL) ? STAGE_EGG : filePet.stage;
    pet.care_mistakes = (filePet.care_mistakes == NULL) ? 0 : filePet.care_mistakes;
    ESP_LOGI(TAG, "Tomodachi initialized");
    return 0;
}

int tick_pet() {
    pet.age += 1;

    pet.hunger -= 2;
    pet.happiness -= 1;
    pet.energy -= 1;

    // Stage transitions based on age in minutes
    // 0 - 720 min: Egg (0.5 day)
    if (pet.age > 720 && pet.age <= 2160) {
        pet.stage = STAGE_BABY; // 720 - 2160 min: Baby (1 day)
    } else if (pet.age > 2160 && pet.age <= 4320) {
        pet.stage = STAGE_CHILD; // 2160 - 4320 min: Child (1.5 days)
    } else if (pet.age > 4320 && pet.age <= 8640) {
        pet.stage = STAGE_TEEN; // 4320 - 8640 min: Teen (3 days)
    } else if (pet.age > 8640) {
        pet.stage = STAGE_ADULT; // 8640+ min: Adult (6 days)
    }
    write_parse_character_file(pet);
    ESP_LOGI(TAG, "Tomodachi ticked");
    return 0;
}