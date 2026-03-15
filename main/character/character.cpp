// std imports
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

// esp-idf imports
#include "esp_littlefs.h"
#include "esp_log.h"

// local imports
#include "character.hpp"
#include "utils/utils.hpp"
#include "settings/settings.hpp"

M5Tomodachi pet;

static uint8_t stage_from_age(uint64_t age) {
    if (age > 8640) {
        return STAGE_ADULT;
    }
    if (age > 4320) {
        return STAGE_TEEN;
    }
    if (age > 2160) {
        return STAGE_CHILD;
    }
    if (age > 720) {
        return STAGE_BABY;
    }
    return STAGE_EGG;
}

esp_err_t init_pet() {
    FILE *save_file = fopen("/filesystem/character.txt", "rb");
    bool has_saved_character = (save_file != nullptr);
    if (save_file) {
        fclose(save_file);
    }

    M5Tomodachi filePet = read_parse_character_file();

    if (!has_saved_character) {
        pet.hunger = 100;
        pet.happiness = 100;
        pet.energy = 100;
        pet.health = 100;
        pet.age = 0;
        pet.stage = STAGE_EGG;
        pet.care_mistakes = 0;
        ESP_LOGI(TAG, "No saved character file found, using defaults");
        return ESP_OK;
    } else {
        pet.hunger = filePet.hunger;
        pet.happiness = filePet.happiness;
        pet.energy = filePet.energy;
        pet.health = filePet.health;
        pet.age = filePet.age;
        pet.stage = stage_from_age(pet.age);
        pet.care_mistakes = filePet.care_mistakes;
        ESP_LOGI(TAG, "saved character file found, using those values");
    }

   
    ESP_LOGI(TAG, "Tomodachi initialized");
    return ESP_OK;
}

int tick_pet() {
    pet.age += 1;

    pet.hunger = (pet.hunger >= 2) ? pet.hunger - 2 : 0;
    pet.happiness = (pet.happiness >= 1) ? pet.happiness - 1 : 0;
    pet.energy = (pet.energy >= 1) ? pet.energy - 1 : 0;

    pet.stage = stage_from_age(pet.age);

    write_parse_character_file(pet);
    ESP_LOGI(TAG, "Tomodachi ticked");
    ESP_LOGI(TAG, "hunger: %d, happiness: %d, energy: %d, health: %d, age: %" PRIu64 ", stage: %d, care_mistakes: %d",
             pet.hunger, pet.happiness, pet.energy, pet.health, pet.age, pet.stage, pet.care_mistakes);
    return 0;
}