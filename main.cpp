#include "graphics.h"
#include "chip8.h"
#include <iostream>

char keyMapping[16] = 
{
    SDLK_1,
    SDLK_2,
    SDLK_3,
    SDLK_4,
    SDLK_q,
    SDLK_w,
    SDLK_e,
    SDLK_r,
    SDLK_a,
    SDLK_s,
    SDLK_d,
    SDLK_f,
    SDLK_z,
    SDLK_x,
    SDLK_c,
    SDLK_v
};

int main (int argc, char *args[]){
    //Initialize the display
    GameScreen *gs = new GameScreen();

    //Set up the chip
    Chip8 *chip = new Chip8(args[1]);
    
    //Store current key events
    SDL_Event e;

    //Pixel buffer
    Uint32 display[2048];

    //Game loop
    bool quit = false;
    while(!quit){
        chip->cycle();

        while(SDL_PollEvent(&e)){

            if (e.type == SDL_QUIT){
                quit = true;
            }

            if(e.type == SDL_KEYDOWN){
                for(int i = 0; i < 16; i++){
                    if(e.key.keysym.sym = keyMapping[i])
                        chip->updateKey(i, 1);
                }
            }

            if(e.type == SDL_KEYUP){
                for(int i = 0; i < 16; i++){
                    if(e.key.keysym.sym = keyMapping[i])
                        chip->updateKey(i, 0);
                }
            }
        }
        //Draw the screen
        if(chip->drawFlag){
            for(int i = 0; i < 2048; i++){
                uint8_t pixel = chip->display[i];
                display[i] = (0x00FFFFFF * pixel) | 0xFF000000;
            }

            gs->draw(display);
        }
        SDL_Delay(100);
    }
    delete gs;
    delete chip;
}