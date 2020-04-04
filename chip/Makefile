SRC = src/main.cpp src/graphics.cpp src/chip8.cpp src/audio.cpp

CC = g++

LINKS = -lSDL2 -lSDL2_mixer

OBJ_NAME = chip8 

all : $(SRC)
	$(CC) $(SRC) $(LINKS) -o $(OBJ_NAME)
