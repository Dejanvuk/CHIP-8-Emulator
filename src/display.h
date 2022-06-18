#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL.h>
#include <SDL_TTF/SDL_ttf.h>
#include <stdio.h>

/* 
64x32-pixel monochrome display 
|(0, 0).........(63, 0)|
|(0,31).........(63,31)|
*/

// Display size on the CHIP8 
#define CHIP8_DISPLAY_WIDTH 64
#define CHIP8_DISPLAY_HEIGHT 32

// We have to scale because CHIP8's graphic display is to small
#define CHIP8_DISPLAY_VIDEO_SCALE 10

// Display size on the graphic device
#define REAL_DISPLAY_WIDTH CHIP8_DISPLAY_WIDTH * CHIP8_DISPLAY_VIDEO_SCALE * 2
#define REAL_DISPLAY_HEIGHT CHIP8_DISPLAY_HEIGHT * CHIP8_DISPLAY_VIDEO_SCALE * 2

#define SPRITE_WIDTH 8

#define CLEAR_COLOR 0x00000000
#define SPRITE_COLOR 0xFFFFFFFF

typedef struct display
{
    SDL_Window* window; //The window we'll be rendering to
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    uint32_t video_buffer[CHIP8_DISPLAY_WIDTH * CHIP8_DISPLAY_HEIGHT]; // holds ARGB values of the pixels
    SDL_Rect videoBufferRect;

    SDL_bool shouldDraw;
} DISPLAY;

void initializeDisplay(DISPLAY**);
void cleanUpDisplay(DISPLAY*);
void draw(DISPLAY*);

#endif