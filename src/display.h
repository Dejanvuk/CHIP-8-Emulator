#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL2/SDL.h>

/* 
64x32-pixel monochrome display 
|(0, 0).........(63, 0)|
|(0,31).........(63,31)|
*/

// Display size on the CHIP8 
#define CHIP8_DISPLAY_WIDTH 64
#define CHIP8_DISPLAY_HEIGHT 32

// We have to scale because CHIP8's graphic display is to small
#define VIDEO_SCALE_WIDTH 10
#define VIDEO_SCALE_HEIGHT 10

// Display size on the graphic device
#define REAL_DISPLAY_WIDTH CHIP8_DISPLAY_WIDTH * VIDEO_SCALE_WIDTH
#define REAL_DISPLAY_HEIGHT CHIP8_DISPLAY_HEIGHT * VIDEO_SCALE_HEIGHT

typedef struct display
{
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
} DISPLAY;


#endif