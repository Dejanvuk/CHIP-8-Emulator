#include <stdlib.h>

#include "interpreter.h"

static uint8_t generateRandomNumber() {
    return (rand() % (255 + 1 - 0)) + 0;
}

void processNextInstruction(CHIP8* pChip8, DISPLAY* pDisplay) {
    // fetch the next opcode
    uint16_t opcode = (pChip8->memory[pChip8->cpu.PC] << 8) | (pChip8->memory[pChip8->cpu.PC+1]);
    //printf("0x%02x %02x | ", opcode, pChip8->cpu.PC);
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
                default:
                    printf("Unknown opcode: 0x%02x \n", opcode);
                    exit(1);
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
            if(pChip8->cpu.V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF)) 
                pChip8->cpu.PC += 2;
            break;
        // 4XNN Skips the next instruction if VX does not equal NN. (Usually the next instruction is a jump to skip a code block);
        case 0x4000:
            if(pChip8->cpu.V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF)) 
                pChip8->cpu.PC += 2;
            break;
        // 5XY0 Skips the next instruction if VX equals VY. (Usually the next instruction is a jump to skip a code block);
        case 0x5000:
            if(pChip8->cpu.V[(opcode & 0x0F00) >> 8] == pChip8->cpu.V[(opcode & 0x00F0) >> 4]) 
                pChip8->cpu.PC += 2;
            break;
        // 6XNN Sets VX to NN.
        case 0x6000:
            pChip8->cpu.V[(opcode & 0x0F00) >> 8] = (opcode & 0x00FF);
            break;
        // 7XNN Adds NN to VX. (Carry flag is not changed);
        case 0x7000:
            pChip8->cpu.V[(opcode & 0x0F00) >> 8] += (opcode & 0x00FF);
            break;  
        case 0x8000:
            switch (opcode & 0x000F) {
                // 8XY0 Sets VX to the value of VY.
                case 0x0000:
                    pChip8->cpu.V[(opcode & 0x0F00) >> 8] = pChip8->cpu.V[(opcode & 0x00F0) >> 4];
                    break;
                // 8XY1 Sets VX to VX or VY. (Bitwise OR operation);
                case 0x0001:
                    pChip8->cpu.V[(opcode & 0x0F00) >> 8] |= pChip8->cpu.V[(opcode & 0x00F0) >> 4];
                    break;
                // 8XY2 Sets VX to VX and VY. (Bitwise AND operation);
                case 0x0002:
                    pChip8->cpu.V[(opcode & 0x0F00) >> 8] &= pChip8->cpu.V[(opcode & 0x00F0) >> 4];
                    break;
                // 8XY3 Sets VX to VX xor VY.
                case 0x0003:
                    pChip8->cpu.V[(opcode & 0x0F00) >> 8] ^= pChip8->cpu.V[(opcode & 0x00F0) >> 4];
                    break;
                // 8XY4 Adds VY to VX. VF is set to 1 when there's a carry ( sum greater than 255), and to 0 when there is not.
                case 0x0004: 
                    {
                    uint8_t x = (opcode & 0x0F00) >> 8;  
                    uint8_t y = (opcode & 0x00F0) >> 4;
                    uint16_t sum = (pChip8->cpu.V[x] +  pChip8->cpu.V[y]);
                    pChip8->cpu.V[0xF] = sum > 0xFF ? 1 : 0;
                    pChip8->cpu.V[x] = sum & 0xFF; // or (uint8_t) sum
                    break;
                    }
                // 8XY5 VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there is not.
                case 0x0005:
                    {
                    uint8_t x = (opcode & 0x0F00) >> 8;  
                    uint8_t y = (opcode & 0x00F0) >> 4;
                    pChip8->cpu.V[0xF] = pChip8->cpu.V[x] > pChip8->cpu.V[y]  ? 1 : 0;
                    pChip8->cpu.V[x] -= pChip8->cpu.V[y]; // or diff & 0xFF
                    break;
                    }
                // 8XY6 Stores the least significant bit of VX in VF and then shifts VX to the right by 1.
                case 0x0006:
                    pChip8->cpu.V[0xF] = pChip8->cpu.V[(opcode & 0x0F00) >> 8] & 0x1;
                    pChip8->cpu.V[(opcode & 0x0F00) >> 8] >>= 1;
                    break;
                // 8XY7 Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there is not.
                case 0x0007:
                    {
                    uint8_t x = (opcode & 0x0F00) >> 8;  
                    uint8_t y = (opcode & 0x00F0) >> 4;
                    pChip8->cpu.V[0xF] = pChip8->cpu.V[y] > pChip8->cpu.V[x] ? 1 : 0;
                    pChip8->cpu.V[x] = pChip8->cpu.V[y] - pChip8->cpu.V[x]; // or diff & 0xFF
                    break;
                    }
                    break;
                // 8XYE Stores the most significant bit of VX in VF and then shifts VX to the left by 1.[b]
                case 0x000E:
                    pChip8->cpu.V[0xF] = pChip8->cpu.V[(opcode & 0x0F00) >> 8] >> 7;
                    pChip8->cpu.V[(opcode & 0x0F00) >> 8] <<= 1;
                    break;
                default:
                    printf("Unknown opcode: 0x%02x \n", opcode);
                    exit(1);
                    break;     
            }
            break;
        // 9XY0 Skips the next instruction if VX does not equal VY. (Usually the next instruction is a jump to skip a code block);
        case 0x9000:
            if(pChip8->cpu.V[(opcode & 0x0F00) >> 8] != pChip8->cpu.V[(opcode & 0x00F0) >> 4]) 
                pChip8->cpu.PC += 2;
            break;  
        // ANNN Sets I to the address NNN.      
        case 0xA000:
            pChip8->cpu.I = opcode & 0x0FFF;
            break;
        // BNNN Jumps to the address NNN plus V0.
        case 0xB000:
            pChip8->cpu.PC = (opcode & 0x0FFF) + pChip8->cpu.V[0];
            break;
        // CXNN Sets VX to the result of a bitwise and operation on a random number (Typically: 0 to 255) and NN.
        case 0xC000:
            pChip8->cpu.V[(opcode & 0x0F00)] = generateRandomNumber() & (opcode & 0x00FF);
            break;
        /* DXYN 
        Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels. 
        Each row of 8 pixels is read as bit-coded starting from memory location I; 
        I value does not change after the execution of this instruction. 
        As described above, VF is set to 1 if any screen pixels are flipped from set to unset when the sprite is drawn, and to 0 if that does not happen
        */  
        case 0xD000: 
        {
            uint8_t height = opcode & 0x000F;
            uint8_t x = pChip8->cpu.V[(opcode & 0x0F00) >> 8];
            uint8_t y = pChip8->cpu.V[(opcode & 0x00F0) >> 4];
            display(pChip8, pDisplay, height, x, y);
            break;
        }
        case 0xE000:
            switch (opcode & 0x00FF) {
                // EX9E Skips the next instruction if the key stored in VX is pressed. (Usually the next instruction is a jump to skip a code block);
                case 0x009E:
                    if(pChip8->keys[pChip8->cpu.V[(opcode & 0x0F00) >> 8]] == 1) 
                        pChip8->cpu.PC += 2;
                    break;
                // EXA1 Skips the next instruction if the key stored in VX is not pressed. (Usually the next instruction is a jump to skip a code block);
                case 0x00A1:
                    if(pChip8->keys[pChip8->cpu.V[(opcode & 0x0F00) >> 8]] == 0) 
                        pChip8->cpu.PC += 2;
                    break;
                default:
                    printf("Unknown opcode: 0x%02x \n", opcode);
                    exit(1);
                    break;
            }
            break;
        case 0xF000:
            switch (opcode & 0x00FF) {
                // FX07 Sets VX to the value of the delay timer.
                case 0x0007:
                    pChip8->cpu.V[(opcode & 0x0F00) >> 8] = pChip8->delayTimer;
                    break;
                // FX0A A key press is awaited, and then stored in VX. (Blocking Operation. All instruction halted until next key event);
                case 0x000A: {
                    uint8_t found = 0;

                    for(int i = 0; i < KEYS_COUNT; i++) {
                        if(pChip8->keys[i] == 1) { 
                            pChip8->cpu.V[(opcode & 0x0F00) >> 8] = i;
                            found = 1;
                            break;
                        }
                    }

                    if(!found) {
                        pChip8->cpu.PC -= 2;
                    }
                    break;
                }
                // FX15 Sets the delay timer to VX.
                case 0x0015:
                    pChip8->delayTimer = pChip8->cpu.V[(opcode & 0x0F00) >> 8];
                    break;
                // FX18 Sets the sound timer to VX.
                case 0x0018:
                    pChip8->soundTimer = pChip8->cpu.V[(opcode & 0x0F00) >> 8];
                    break;
                // FX1E Adds VX to I. VF is not affected.
                case 0x001E:
                    pChip8->cpu.I += pChip8->cpu.V[(opcode & 0x0F00) >> 8];
                    break;
                // FX29 Sets I to the location of the sprite for the character in VX. Characters 0-F (in hexadecimal) are represented by a 4x5 font.
                case 0x0029:
                    pChip8->cpu.I = FONT_WIDTH * pChip8->cpu.V[(opcode & 0x0F00) >> 8];
                    break;
                /* FX33 
                Stores the binary-coded decimal representation of VX, with the most significant of three digits at the address in I, the middle digit at I plus 1,
                and the least significant digit at I plus 2. 
                (In other words, take the decimal representation of VX, place the hundreds digit in memory at location in I, the tens digit at location I+1, 
                and the ones digit at location I+2.);
                */
                case 0x0033:
                {
                    uint8_t value = pChip8->cpu.V[(opcode & 0x0F00) >> 8];
                    pChip8->memory[pChip8->cpu.I + 2] = value % 10;
                    pChip8->memory[pChip8->cpu.I + 1] = (value / 10) % 10;
                    pChip8->memory[pChip8->cpu.I] = (value / 100) % 10;
                    break;
                }
                /* FX55 
                Stores from V0 to VX (including VX) in memory, starting at address I. The offset from I is increased by 1 for each value written, but I itself is left unmodified
                */
                case 0x0055: 
                    for(int i = 0; i <= ((opcode & 0x0F00) >> 8); i++) {
                        pChip8->memory[pChip8->cpu.I + i] = pChip8->cpu.V[i];
                    }
                    break;
                /* FX65 Fills from V0 to VX (including VX) with values from memory, starting at address I. 
                The offset from I is increased by 1 for each value read, but I itself is left unmodified */
                case 0x0065:
                    for(int i = 0; i <= ((opcode & 0x0F00) >> 8); i++) {
                        pChip8->cpu.V[i] = pChip8->memory[pChip8->cpu.I + i];
                    }
                    break;
                default:
                    printf("Unknown opcode: 0x%02x \n", opcode);
                    exit(1);
                    break;
            }
            break;
        default:
            printf("Unknown opcode: 0x%02x \n", opcode);
            exit(1);
            break;
    }
    
}

void display(CHIP8* pChip8, DISPLAY* pDisplay, uint8_t height, uint8_t x, uint8_t y) {
    x %= CHIP8_DISPLAY_WIDTH;
    y %= CHIP8_DISPLAY_HEIGHT;

    pChip8->cpu.V[0xF] = 0;

	for(int r = 0; r < height; r++) {
		uint8_t spriteRow = pChip8->memory[pChip8->cpu.I + r]; //the 8 columns as bit to draw

        for(int c = 0; c < 8; c++) {
            uint8_t spritePixel = spriteRow & (0x80 >> c);

            if(spritePixel) { // the place we will draw the sprite pixel is already occupied
                uint32_t* displayPixel = &pDisplay->video_buffer[(y + r) * CHIP8_DISPLAY_WIDTH + (x + c)];
                if(*displayPixel == 0xFFFFFFFF) {
                    // collision detected
                    pChip8->cpu.V[0xF] = 1;
                }
                *displayPixel ^= 0xFFFFFFFF;
            }
        }
	}

	pDisplay->shouldDraw = SDL_TRUE;
}

void clearDisplay(DISPLAY* pDisplay) {
	memset(pDisplay->video_buffer, 0, sizeof(uint32_t) * CHIP8_DISPLAY_WIDTH * CHIP8_DISPLAY_HEIGHT);

	// also redraw the background
	pDisplay->shouldDraw = SDL_TRUE;
}