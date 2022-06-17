#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "chip8.h"
#include "display.h"
#include "keyboard.h"
#include "fonts.h"

/**
 * @brief // fetch, decode and execute the next instruction
 * 
 */
void processNextInstruction(CHIP8*, DISPLAY*);
void display(CHIP8*, DISPLAY*, uint8_t, uint8_t, uint8_t);
void clearDisplay(DISPLAY*);

#endif