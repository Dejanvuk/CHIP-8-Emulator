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

		pDisplay->texture = SDL_CreateTexture(pDisplay->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, CHIP8_DISPLAY_WIDTH, CHIP8_DISPLAY_HEIGHT);

		if(pDisplay->texture == NULL )
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "The texture could not be created! SDL_Error: %s\n", SDL_GetError());
			return;
		}

		SDL_SetTextureColorMod( pDisplay->texture, 215, 255, 0 ); // change the color of all of the sprites to red
		pDisplay->videoBufferRect.x = 0;
		pDisplay->videoBufferRect.y = 0;
		/* since we no longer pass NULL to SDL_Rect 4th arg of SDL_RenderCopy which would've caused the texture to stretch to fill the whole render target
		we have to manually specify the rectangle we want our texture to stretch to */
		pDisplay->videoBufferRect.w = CHIP8_DISPLAY_WIDTH * CHIP8_DISPLAY_VIDEO_SCALE;
		pDisplay->videoBufferRect.h = CHIP8_DISPLAY_HEIGHT * CHIP8_DISPLAY_VIDEO_SCALE;

		pDisplay->shouldDraw = SDL_TRUE;
	}

	*ppDisplay = pDisplay; 
}

void draw(DISPLAY* pDisplay) {
	int pitch; // number of bytes per row, Pitch = 256,  4 bytes (RGBA) * 64 (WIDTH)
	void* pixeldata = NULL;
	//SDL_UpdateTexture(pDisplay->texture, NULL, pDisplay->video_buffer, sizeof(uint32_t) * CHIP8_DISPLAY_WIDTH);
	SDL_LockTexture(pDisplay->texture, NULL, &pixeldata, &pitch);
  	memcpy(pixeldata, pDisplay->video_buffer, pitch * CHIP8_DISPLAY_HEIGHT); // 256 bytes (Pitch) * 32 bytes (HEIGHT)
  	SDL_UnlockTexture(pDisplay->texture);

	SDL_RenderClear(pDisplay->renderer);
	SDL_RenderCopy(pDisplay->renderer, pDisplay->texture, NULL, &pDisplay->videoBufferRect);
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