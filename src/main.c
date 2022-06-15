#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "chip8.h"
#include "display.h"


int main(int argc, char* argv[]) {
	char* currentRom = "roms/TANK";
	FILE *romFilePointer = NULL;
	DISPLAY* pDisplay = NULL;
	SDL_bool shouldRun = SDL_TRUE;
	SDL_bool shouldDraw = SDL_FALSE;

	CHIP8* pChip8 = initializeChip8();

	if(pChip8 == NULL) {
		printf( "ERROR: Couldn't initialize the Chip8 interpreter! \n");
        return(-1);
	}

	initializeDisplay(&pDisplay);

	if(pDisplay == NULL) {
		printf( "ERROR: Couldn't initialize the display! \n");
		return(-1);
	}

	readRom(currentRom, pChip8, &romFilePointer);

	if(romFilePointer == NULL) {
		printf( "ERROR: Couldn't read the Rom file! \n");
        return(-1);
	}

	/* Main loop:
    While 'ESC' is not pressed:
        3a: Get next instruction
        3b: Decode the instruction
        3c: Execute the instruction
        3d: Draw
    */

    while(shouldRun) {
		SDL_Event event;

		// Process the user events
    	while (SDL_PollEvent(&event)) {
        	switch (event.type) {
            	case SDL_QUIT:
                	shouldRun = SDL_TRUE;
                	break;
				default:
					SDL_Delay(3000);
					shouldRun = false;
					break;
        	}
    	}

		// Draw
		//if (shouldDraw) {draw()}

    }

	// Clean up everything.
	cleanUpDisplay(pDisplay);
	return 0;
}