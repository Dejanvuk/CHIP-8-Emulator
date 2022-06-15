#include <stdlib.h>

#include "interpreter.h"

static uint8_t generateRandomNumber() {
    return (rand() % (255 + 1 - 0)) + 0;
}

void processNextInstruction(CHIP8* pChip8, DISPLAY* pDisplay) {
    // fetch the next opcode
    uint16_t opcode = (pChip8->memory[pChip8->cpu.PC] << 8) | (pChip8->memory[pChip8->cpu.PC+1]);
    pChip8->cpu.PC += 2; // easier to do it here then after PC increasing instructions

    // decode and process
    switch (opcode & 0xF000) {
        case 0x0000:
            switch (opcode & 0x00FF) {
                // 00E0 Clear the display.
                case 0x00E0:
                    clearDisplay(pDisplay);
                    break;
                // 00EE Returns from a subroutine.
                case 0x00EE:
                    pChip8->cpu.SP--;
                    pChip8->cpu.PC = pChip8->stack[pChip8->cpu.SP];
                    break;
                /* 
                0NNN Calls machine code routine (RCA 1802 for COSMAC VIP) at address NNN. 
                only used on the old computers on which Chip-8 was originally implemented. 
                It is ignored by modern interpreters. 
                */
                case 0x00FF:
                    break;
            }
            break;
        // 1NNN Jumps to address NNN.
        case 0x1000:
            pChip8->cpu.PC = opcode & 0x0FFF;
            break;
        // 2NNN Calls subroutine at NNN.
        case 0x2000:
            pChip8->stack[pChip8->cpu.SP] = pChip8->cpu.PC;
            pChip8->cpu.PC = opcode & 0x0FFF;
            pChip8->cpu.SP++;
            break;  
        // 3XNN Skips the next instruction if VX equals NN. (Usually the next instruction is a jump to skip a code block);
        case 0x3000:
            if(pChip8->cpu.V[(opcode & 0x0F00) >> 8] == (opcode && 0x00FF)) 
                pChip8->cpu.PC += 2;
            break;
        // 4XNN Skips the next instruction if VX does not equal NN. (Usually the next instruction is a jump to skip a code block);
        case 0x4000:
            if(pChip8->cpu.V[(opcode & 0x0F00) >> 8] != (opcode && 0x00FF)) 
                pChip8->cpu.PC += 2;
            break;
        // 5XY0 Skips the next instruction if VX equals VY. (Usually the next instruction is a jump to skip a code block);
        case 0x5000:
            if(pChip8->cpu.V[(opcode & 0x0F00) >> 8] == pChip8->cpu.V[(opcode & 0x0F0) >> 4]) 
                pChip8->cpu.PC += 2;
            break;
        // 6XNN Sets VX to NN.
        case 0x6000:
            pChip8->cpu.V[(opcode & 0x0F00) >> 8] = (opcode && 0x00FF);
            break;
        // 7XNN Adds NN to VX. (Carry flag is not changed);
        case 0x7000:
            pChip8->cpu.V[(opcode & 0x0F00) >> 8] += (opcode && 0x00FF);
            break;  
        case 0x8000:

            break;
        // 9XY0 Skips the next instruction if VX does not equal VY. (Usually the next instruction is a jump to skip a code block);
        case 0x9000:
            if(pChip8->cpu.V[(opcode & 0x0F00) >> 8] != pChip8->cpu.V[(opcode & 0x0F0) >> 4]) 
                pChip8->cpu.PC += 2;
            break;  
        // ANNN Sets I to the address NNN.      
        case 0xA000:
            pChip8->cpu.I = opcode & 0x0FFF;
            break;
        // BNNN Jumps to the address NNN plus V0.
        case 0xB000:
            pChip8->cpu.PC = (opcode & 0x0FFF) + pChip8->cpu.V[0x0];
            break;
        // CXNN Sets VX to the result of a bitwise and operation on a random number (Typically: 0 to 255) and NN.
        case 0xC000:
            pChip8->cpu.V[(opcode & 0x0F00)] = generateRandomNumber() & (opcode & 0x00FF);
            break;  
        case 0xD000:
            break;
        case 0xE000:
            break;
        case 0xF000:
            break;
        default:
            printf("Error: unknown opcode! %x\n", opcode);
            break;
    }
}