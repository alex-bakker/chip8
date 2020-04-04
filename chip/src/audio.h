#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

/*
*  Simple class to play classin chip8 beep
*/

class Audio{
    private:
        Mix_Chunk *beep;

    public:
        void playBeep();
        Audio();  
        ~Audio(); 
};