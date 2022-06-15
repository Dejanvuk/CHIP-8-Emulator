#include "interpreter.h"

void processNextInstruction(CHIP8* pChip8, DISPLAY* pDisplay) {
    // fetch the next opcode
    uint16_t opcode = (pChip8->memory[pChip8->cpu.PC] << 8) | (pChip8->memory[pChip8->cpu.PC+1]);

    // decode and process
    switch (opcode & 0xF000) {
        case 0x0000:
            switch (opcode & 0x00FF) {
                // Clear the display.
                case 0x00E0:
                    clearDisplay(pDisplay);
                    pChip8->cpu.PC += 2;
                    break;
                // Returns from a subroutine.
                case 0x00EE:
                    pChip8->cpu.SP--;
                    pChip8->cpu.PC = pChip8->stack[pChip8->cpu.SP];
                    break;
                /* 
                Calls machine code routine (RCA 1802 for COSMAC VIP) at address NNN. 
                only used on the old computers on which Chip-8 was originally implemented. 
                It is ignored by modern interpreters. 
                */
                case 0x00FF:
                    break;
            }
            break;
        case 0x1000:
            break;
        case 0x2000:
            break;  
        case 0x3000:
            break;
        case 0x4000:
            break;
        case 0x5000:
            break;
        case 0x6000:
            break;
        case 0x7000:
            break;  
        case 0x8000:
            break;
        case 0x9000:
            break;        
        case 0xA000:
            break;
        case 0xB000:
            break;
        case 0xC000:
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