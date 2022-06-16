#ifndef FONTS_H
#define FONTS_H

#include "chip8.h"

#define FONT_SIZE 0x50
#define FONT_WIDTH 5 // These sprites are 5 bytes long, or 8x5 pixels

const extern uint8_t fonts[FONT_SIZE];

void loadFontset(CHIP8*);

#endif