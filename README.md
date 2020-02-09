# CHIP8 C++ Emulator
Complete emulation of the original CHIP8 specification (including sound) created by Joseph Weisbecker in the 70's. 

## Building the Emulator
To begin, you will first need to install the [SDL2](https://www.libsdl.org/download-2.0.php) and [SDL_Mixer](https://www.libsdl.org/projects/SDL_mixer/https://www.libsdl.org/projects/SDL_mixer/) development libraries. You are also going to need to be able to run makefiles as well as have g++ installed. Finally clone the repo and enter the main project directory.

Run `make` to trigger the g++ compilation. You should see a chip8 executable.  
Run the executable with `./chip8 <path-to-rom> <clock speed>`  

It seems some games were meant to be run at much lower clock speeds thus I parameterized this value. Normally this value should be 1.3 and should never be set to more than 7.

[Link to bunch of chip8 games.](https://www.zophar.net/pdroms/chip8/chip-8-games-pack.html)
