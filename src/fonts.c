#include "fonts.h"

/**
 * @brief Load the fontset into the first 80 bytes of memory
 * 
 * @param pChip8 
 */
void loadFontset(CHIP8* pChip8) {
    memcpy(pChip8->memory, fonts, FONT_SIZE);
}