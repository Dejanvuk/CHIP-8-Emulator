#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "chip8.h"
#include "display.h"
#include "interpreter.h"
#include "keyboard.h"

uint8_t getKeyIndex(CHIP8*, SDL_Keycode);
void processKeyDown(CHIP8*, SDL_KeyboardEvent);
void processKeyUp(CHIP8*, SDL_KeyboardEvent);


int main(int argc, char* argv[]) {
	char* currentRom = "roms/TANK";
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

	if(!readRom(currentRom, pChip8)) {
		printf( "ERROR: Couldn't read the Rom file! \n");
        return(-1);
	}

    while(shouldRun) {
		SDL_Event event;

		// fetch, decode and execute the next instruction
		processNextInstruction(pChip8, pDisplay);
		//SDL_Delay(520/60);

		// Process the user events
    	if (SDL_PollEvent(&event)) {
        	switch (event.type) {
            	case SDL_QUIT:
                	shouldRun = SDL_TRUE;
                	break;
				case SDL_KEYDOWN:
					break;
				case SDL_KEYUP:
					break;
				default:
					//shouldRun = false;
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

/* 
The CHIP-8 has 16 input keys that match the first 16 hex values: 0 through F.

Keypad       Keyboard
+-+-+-+-+    +-+-+-+-+
|1|2|3|C|    |1|2|3|4|
+-+-+-+-+    +-+-+-+-+
|4|5|6|D|    |Q|W|E|R|
+-+-+-+-+ => +-+-+-+-+
|7|8|9|E|    |A|S|D|F|
+-+-+-+-+    +-+-+-+-+
|A|0|B|F|    |Z|X|C|V|
+-+-+-+-+    +-+-+-+-+
*/
uint8_t getKeyIndex(CHIP8* pChip8, SDL_Keycode sym) {
	for(uint8_t i = 0; i < KEYS_COUNT; i++) {
		if(sym == KEYMAP[i]) {
			return i;
		}
	}

	return -1;
}

void processKeyDown(CHIP8* pChip8, SDL_KeyboardEvent event) {
	uint8_t index = getKeyIndex(pChip8, event.keysym.sym);

	if(index >= 0) {
		pChip8->keys[index] = 1;
	}
}

void processKeyUp(CHIP8* pChip8, SDL_KeyboardEvent event) {
	uint8_t index = getKeyIndex(pChip8, event.keysym.sym);

	if(index >= 0) {
		pChip8->keys[index] = 0;
	}
}