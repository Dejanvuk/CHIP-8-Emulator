#ifndef CHIP8_H
#define CHIP8_H

#include <SDL.h>
#include <stdio.h>
#include <stdint.h>

#include "core.h"

int readRom(char*, CHIP8*);

CHIP8* initializeChip8(void);

#endif