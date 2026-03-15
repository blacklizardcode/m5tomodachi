// stdlibs
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <cstdlib>

// esp-idf libs
#include "esp_log.h"
#include "esp_http_server.h"

// local libs
#include "utils/utils.hpp"
#include "character/character.hpp"
#include "settings/settings.hpp"

void xor_cipher(char *data, size_t data_len, const char *key);


M5Tomodachi read_parse_character_file() {
    M5Tomodachi character = {};
    FILE *fptr = fopen("/filesystem/character.txt", "rb");
    if (!fptr) {
        return character;
    }

    char fileContents[512];
    size_t bytes_read = fread(fileContents, 1, sizeof(fileContents) - 1, fptr);
    if (bytes_read == 0) {
        fclose(fptr);
        return character;
    }
    fclose(fptr);
    xor_cipher(fileContents, bytes_read, XOR_KEY);
    fileContents[bytes_read] = '\0';

    char buffer[512];
    strncpy(buffer, fileContents, sizeof(buffer));
    buffer[sizeof(buffer) - 1] = '\0';  // safety null-terminate

    char *line = strtok(buffer, "\n");
    while (line != NULL) {
        while (*line == ' ') line++;
        char *equal_sign = strchr(line, '=');
        if (equal_sign) {
            *equal_sign = '\0';  // split the string at '='
            char *key = line;
            char *value = equal_sign + 1;
            char *end = key + strlen(key) - 1;
            while (end > key && (*end == ' ')) *end-- = '\0';
            while (*value == ' ') value++;

            if (strcmp(key, "hunger") == 0) character.hunger = (uint8_t)strtoul(value, NULL, 10);
            else if (strcmp(key, "happiness") == 0) character.happiness = (uint8_t)strtoul(value, NULL, 10);
            else if (strcmp(key, "energy") == 0) character.energy = (uint8_t)strtoul(value, NULL, 10);
            else if (strcmp(key, "health") == 0) character.health = (uint8_t)strtoul(value, NULL, 10);
            else if (strcmp(key, "age") == 0) {
                errno = 0;
                character.age = strtoull(value, NULL, 10);
                if (errno != 0) {
                    character.age = 0;
                }
            } else if (strcmp(key, "stage") == 0) character.stage = (uint8_t)strtoul(value, NULL, 10);
            else if (strcmp(key, "care_mistakes") == 0) character.care_mistakes = (uint8_t)strtoul(value, NULL, 10);
        }
        line = strtok(NULL, "\n");
    }
    return character;
}

void write_parse_character_file(const M5Tomodachi& character) {
    char buffer[256];
    int plain_len = snprintf(buffer, sizeof(buffer),
        "hunger=%u\n"
        "happiness=%u\n"
        "energy=%u\n"
        "health=%u\n"
        "age=%llu\n"
        "stage=%u\n"
        "care_mistakes=%u\n",
        character.hunger,
        character.happiness,
        character.energy,
        character.health,
        character.age,
        character.stage,
        character.care_mistakes
    );

    if (plain_len <= 0) {
        return;
    }

    size_t data_len = (size_t)plain_len;
    if (data_len >= sizeof(buffer)) {
        data_len = sizeof(buffer) - 1;
    }

    xor_cipher(buffer, data_len, XOR_KEY);

    FILE *fptr = fopen("/filesystem/character.txt", "wb");
    if (!fptr) {
        return;
    }
    fwrite(buffer, 1, data_len, fptr);
    fclose(fptr);
}

void xor_cipher(char *data, size_t data_len, const char *key) {
    size_t key_len = strlen(key);
    for (size_t i = 0; i < data_len; ++i) {
        data[i] ^= key[i % key_len];  // XOR with key byte
    }
}
