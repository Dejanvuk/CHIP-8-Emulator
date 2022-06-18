# To run 'mingw32-make.exe'

#CC specifies which compiler we're using
CC = gcc 
C_STANDARD = c11

#COMPILER_FLAGS specifies the additional compilation options we're using
# -Wall verbose warnings
# -Wl,-subsystem,windows gets rid of the console window
COMPILER_FLAGS = -Wall 

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf

#INCLUDE_PATHS specifies the additional include paths we'll need
INCLUDE_PATHS = -Iinclude/SDL2 -Iinclude

#LIB_PATHS specifies the additional library paths we'll need
LIB_PATHS = -Llib/SDL2 -Llib/SDL_TTF

#EXE_NAME specifies the name of our exectuable
EXE_NAME = chip8.exe

#OBJS specifies which files to compile as part of the project
#$(wildcard *.c) $(wildcard */*.c) OR $(wildcard **/*.c) on some platforms
OBJS = $(wildcard src/*.c)

all: $(OBJS)
	$(CC) -std=$(C_STANDARD) $(OBJS) $(INCLUDE_PATHS) $(LIB_PATHS) $(COMPILER_FLAGS) ${LINKER_FLAGS} -o ${EXE_NAME}