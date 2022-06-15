#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "chip8.h"
#include "display.h"


int main(int argc, char* argv[]) {
	char* currentRom = "roms/TANK";
	FILE *romFilePointer = NULL;
	SDL_bool shouldRun = SDL_TRUE;
	DISPLAY* pDisplay = NULL;

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

    while(shouldRun) {
		SDL_Event event;

		// fetch, decode and execute the next instruction

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