#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "chip8.h"
#include "display.h"


int main(int argc, char* argv[]) {
	char* currentRom = "roms/TANK";
	uint16_t romLength = 0;
	FILE *romFilePointer = NULL;

	// Load the ROM and initialize the Chip8 interpreter.
    romFilePointer = fopen(currentRom, "r");

	if (!romFilePointer) {
		printf( "ROM could not be opened: %s\n", currentRom);
        exit(0);
	}

	fseek(romFilePointer, 0, SEEK_END);
	romLength = ftell(romFilePointer);


	if (romLength == 0) {
		printf( "ROM file is empty: %s\n", currentRom);
        exit(0);
	}

	fseek(romFilePointer, 0, SEEK_SET);

	printf("size: %d\n", romLength);

	// Initialize SDL.

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