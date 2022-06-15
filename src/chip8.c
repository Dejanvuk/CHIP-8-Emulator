#include "chip8.h"

/**
 * @brief Opens the ROM file and initializes the Chip 8 interpreter structure with the default data.
 * 
 * @param location ROM file location
 * @param pRomFilePointer Pointer to a FILE structure pointer 
 * @return size_t the number of bytes in a file aka its length
 */
void readRom(char* location, CHIP8* pChip8, FILE** pRomFilePointer) {
    size_t romLength = 0;
    FILE *romFilePointer = fopen(location, "r");

	if (!romFilePointer) {
		printf( "ROM could not be opened: %s\n", location);
        return;
	}

	fseek(romFilePointer, 0, SEEK_END);
	romLength = ftell(romFilePointer);


	if (romLength == 0) {
		printf( "ROM file is empty: %s\n", location);
        return;
	}

	fseek(romFilePointer, 0, SEEK_SET);

    // Read the file content into Chip8's memory
    int read = fread(pChip8->memory + DATA_SPACE_START, sizeof(uint8_t), romLength, romFilePointer);

    if(!read) {
        printf( "Unable to read data from ROM file: %s\n", location);
        return;
    }

    fclose(romFilePointer);


    *pRomFilePointer = romFilePointer;
}

CHIP8* initializeChip8() {
    CHIP8* pChip8 = (CHIP8*) malloc(sizeof(CHIP8));
    // Clear the memory and reset the registers to zero
    memset(pChip8, 0, sizeof(CHIP8));
    pChip8->cpu.PC = DATA_SPACE_START;
    pChip8->cpu.SP = 0;

    return pChip8;
}
