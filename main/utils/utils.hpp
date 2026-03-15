#ifndef UTILS_HPP

#define UTILS_HPP

#include "character/character.hpp" 

#define MAX_MAP_KEY 64
#define MAX_MAP_VALUE 256
#define MAX_MAP_ENTRIES 256

// Configuration


typedef struct map {
    char key[MAX_MAP_KEY];
    char value[MAX_MAP_VALUE];
} map;

void write_parse_character_file(const M5Tomodachi& character);
M5Tomodachi read_parse_character_file();

// End of UTILS_HPP
#endif