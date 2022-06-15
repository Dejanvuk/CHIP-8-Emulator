#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "chip8.h"
#include "display.h"


int main(int argc, char* argv[]) {
	char* currentRom = "roms/TANK";
	FILE *romFilePointer = NULL;

	// Initialize SDL.

	// Initialize the Chip8 interpreter.
	CHIP8* pChip8 = initializeChip8();

	// Load the ROM
	readRom(currentRom, pChip8, &romFilePointer);

	/* Main loop:
    While 'ESC' is not pressed:
        3a: Get next instruction
        3b: Decode the instruction
        3c: Execute the instruction
        3d: Draw
    */
	

	// Clean up everything.
	return 0;
}