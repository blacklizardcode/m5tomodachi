#ifndef CHARACTER_HPP

#define CHARACTER_HPP

#include "stdint.h"

#define STAGE_EGG 0
#define STAGE_BABY 1
#define STAGE_CHILD 2
#define STAGE_TEEN 3
#define STAGE_ADULT 4

typedef struct M5Tomodachi {
    uint8_t hunger;
    uint8_t happiness;
    uint8_t energy;
    uint8_t health;
    uint64_t age;
    uint8_t stage;
    uint8_t care_mistakes;
} M5Tomodachi;

int init_pet();
int tick_pet();

#endif