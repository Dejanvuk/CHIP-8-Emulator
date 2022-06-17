#include "display.h"

void initializeDisplay(DISPLAY** ppDisplay) {
	DISPLAY* pDisplay = (DISPLAY*) malloc(sizeof(DISPLAY));
	memset(pDisplay, 0, sizeof(DISPLAY));
    //Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
		return;
	}
	else
	{
		//Create window
		pDisplay->window = SDL_CreateWindow( "CHIP-8 Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, REAL_DISPLAY_WIDTH, REAL_DISPLAY_HEIGHT, SDL_WINDOW_SHOWN );

		if(pDisplay->window == NULL )
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Window could not be created! SDL_Error: %s", SDL_GetError());
			return;
		}
		
		pDisplay->renderer = SDL_CreateRenderer(pDisplay->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);

		if(pDisplay->renderer == NULL )
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
			return;
		}

		pDisplay->texture = SDL_CreateTexture(pDisplay->renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, CHIP8_DISPLAY_WIDTH, CHIP8_DISPLAY_HEIGHT);

		if(pDisplay->texture == NULL )
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "The texture could not be created! SDL_Error: %s\n", SDL_GetError());
			return;
		}

		pDisplay->shouldDraw = SDL_TRUE;
	}

	*ppDisplay = pDisplay; 
}

void draw(DISPLAY* pDisplay) {
	SDL_UpdateTexture(pDisplay->texture, NULL, pDisplay->video_buffer, sizeof(pDisplay->video_buffer[0]) * CHIP8_DISPLAY_WIDTH);
	SDL_RenderClear(pDisplay->renderer);
	SDL_RenderCopy(pDisplay->renderer, pDisplay->texture, NULL, NULL);
	SDL_RenderPresent(pDisplay->renderer);

	pDisplay->shouldDraw = SDL_FALSE;
}

void cleanUpDisplay(DISPLAY* display) {
    SDL_DestroyTexture(display->texture);
    SDL_DestroyRenderer(display->renderer);
	SDL_DestroyWindow(display->window);

	//Quit SDL subsystems
	SDL_Quit();
}