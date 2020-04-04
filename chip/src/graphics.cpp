#include "graphics.h"

/*
Defintion of the GameScreen class to handle all graphics
*/

void GameScreen::draw(){
    SDL_UpdateTexture(texture, NULL, pixels, 64 * sizeof(Uint32));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

GameScreen::GameScreen() {
    SDL_InitSubSystem(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("CHIP-8", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_RenderSetLogicalSize(renderer, WIDTH, HEIGHT);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STATIC, 64, 32);
}

GameScreen::~GameScreen() {
    SDL_DestroyWindow(window);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

const int GameScreen::HEIGHT = 640;
const int GameScreen::WIDTH = 1280;