#ifndef SPRITESHEET_HPP

#define SPRITESHEET_HPP

#include "utils/utils.hpp"
#include "M5Unified.hpp"

extern const vector2 NEUTRAL_HAPPY;
extern const vector2 NEUTRAL_HAPPY_BLINK;
extern const vector2 NEUTRAL_HAPPY_WINK;
extern const vector2 ANGRY;
extern const vector2 HAPPY;
extern const vector2 SUPRISED;
extern const vector2 SAD_EYES_CLOSED;
extern const vector2 SAD;
extern const vector2 UWU;
extern const vector2 DEAD;


extern const unsigned char epd_bitmap_ [] __attribute__((section(".rodata")));
void drawSprite(M5Canvas &canvas, vector2 sprite);

#endif