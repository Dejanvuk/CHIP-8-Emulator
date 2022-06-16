#ifndef CHIP8_H
#define CHIP8_H

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdint.h>

#include "keyboard.h"

#define REGISTER_SIZE 16 // 16 general purpose registers, VF is used as flag.
#define MEMORY_MAP_SIZE 4096 // 4KB (4,096 bytes) of RAM.
#define STACK_SIZE 16

#define DATA_SPACE_START 0x200 // Start of the CHip 8's data space
#define DATA_SPACE_END 0xFFF // End of the Cip 8's data space

typedef struct cpu {
    /* Registers */
    uint8_t V[REGISTER_SIZE]; // V[0xF] used as flag.
    uint16_t I; // address register.
    uint16_t PC; // program counter, holds the address of the instruction to be executed.
    uint8_t SP; // stack pointer, points to the topmost level of the stack
} CPU;

typedef struct chip8
{
    CPU cpu;
    uint8_t memory[MEMORY_MAP_SIZE]; // RAM
    uint16_t stack[STACK_SIZE];
    uint8_t keys[KEYS_COUNT];
} CHIP8;

int readRom(char*, CHIP8*);

CHIP8* initializeChip8(void);

#endif