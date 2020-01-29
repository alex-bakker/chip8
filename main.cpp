#include "graphics.h"
#include "chip8.h"
#include <iostream>

int main (int argc, char *args[]){
    GameScreen *gs = new GameScreen();
    Chip8 *chip = new Chip8(args[1]);

    delete gs;
}