#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL2/SDL.h>
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
#define VIDEO_SCALE_WIDTH 10
#define VIDEO_SCALE_HEIGHT 10

// Display size on the graphic device
#define REAL_DISPLAY_WIDTH CHIP8_DISPLAY_WIDTH * VIDEO_SCALE_WIDTH
#define REAL_DISPLAY_HEIGHT CHIP8_DISPLAY_HEIGHT * VIDEO_SCALE_HEIGHT

#define CLEAR_COLOR 0x00000000

typedef struct display
{
    SDL_Window* window; //The window we'll be rendering to
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    uint32_t video_buffer[CHIP8_DISPLAY_WIDTH * CHIP8_DISPLAY_HEIGHT]; // holds ARGB values of the pixels
    SDL_bool shouldDraw;
} DISPLAY;

void initializeDisplay(DISPLAY**);
void cleanUpDisplay(DISPLAY*);
void draw(SDL_Renderer*, SDL_Texture*);
void clearDisplay(DISPLAY*);

#endif