#include "graphics.h"
int main (int argc, char *args[]){
    GameScreen *gs = new GameScreen();
    Uint32 *pixels = new Uint32[64*32];

    memset(pixels, 255, 64 * 32 * sizeof(Uint32));

    bool quit = false;
    SDL_Event e;
    while(!quit){
        while(SDL_PollEvent(&e) != 0){
            if(e.type == SDL_QUIT){
                quit = true;
            }
        }
        gs->draw(pixels);
    }
    delete gs;
}