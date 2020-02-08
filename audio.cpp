#include "audio.h"

Audio::Audio() {
    SDL_InitSubSystem(SDL_INIT_AUDIO);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    Mix_Volume(2, MIX_MAX_VOLUME / 10);
    beep = Mix_LoadWAV("beep.wav");
}

Audio::~Audio(){
    Mix_FreeChunk(beep);
    Mix_Quit();
}

void Audio::playBeep(){
    Mix_PlayChannel(2, beep, 0);
}