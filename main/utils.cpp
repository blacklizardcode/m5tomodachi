
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <cstdlib>

#include "utils.hpp"
#include "character.hpp"

void xor_cipher(char *data, const char *key);


M5Tomodachi read_parse_character_file() {
    M5Tomodachi character = {};
    FILE *fptr = fopen("/filesystem/character.txt", "r");
    if (!fptr) {
        return character;
    }
    char fileContents[100];
    if (!fgets(fileContents, sizeof(fileContents), fptr)) {
        fclose(fptr);
        return character;
    }
    fclose(fptr);
    xor_cipher(fileContents, XOR_KEY);

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

            if (strcmp(key, "hunger") == 0) character.hunger = (uint8_t)atoi(value);
            else if (strcmp(key, "happiness") == 0) character.happiness = (uint8_t)atoi(value);
            else if (strcmp(key, "energy") == 0) character.energy = (uint8_t)atoi(value);
            else if (strcmp(key, "health") == 0) character.health = (uint8_t)atoi(value);
            else if (strcmp(key, "age") == 0) character.age = (uint8_t)atoi(value);
            else if (strcmp(key, "stage") == 0) character.stage = (uint8_t)atoi(value);
            else if (strcmp(key, "care_mistakes") == 0) character.care_mistakes = (uint8_t)atoi(value);
        }
        line = strtok(NULL, "\n");
    }
    return character;
}

void write_parse_character_file(const M5Tomodachi& character) {
    char buffer[256];
    snprintf(buffer, sizeof(buffer),
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

    xor_cipher(buffer, XOR_KEY);

    FILE *fptr = fopen("/filesystem/character.txt", "w");
    if (!fptr) {
        return;
    }
    fputs(buffer, fptr);
    fclose(fptr);
}

void xor_cipher(char *data, const char *key) {
    size_t key_len = strlen(key);
    for (size_t i = 0; data[i] != '\0'; ++i) {
        data[i] ^= key[i % key_len];  // XOR with key byte
    }
}