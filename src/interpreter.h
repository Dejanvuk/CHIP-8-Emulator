#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "chip8.h"
#include "display.h"
#include "keyboard.h"

/**
 * @brief // fetch, decode and execute the next instruction
 * 
 */
void processNextInstruction(CHIP8*, DISPLAY*);

#endif