#include "display.h"

void initSDL(DISPLAY* display) {
    //Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
	}
	else
	{
		//Create window
		display->window = SDL_CreateWindow( "CHIP-8 Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, REAL_DISPLAY_WIDTH, REAL_DISPLAY_HEIGHT, SDL_WINDOW_SHOWN );
		if( display->window == NULL )
		{
			printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
		}
		else
		{
			//Wait two seconds
			SDL_Delay( 2000 );
		}
	}
}

void cleanUp(SDL_Window* window) {
    //Destroy window
	SDL_DestroyWindow( window );

	//Quit SDL subsystems
	SDL_Quit();
}