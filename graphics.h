#include <SDL2/SDL.h>
/*
Class declaration to handle graphics.
*/
class GameScreen {
    private:
        SDL_Window *window;
        SDL_Renderer *renderer;
        SDL_Texture *texture;

    public:
        const static int WIDTH;
        const static int HEIGHT;
        
        //Temporary buffer for drawing
        Uint32 pixels[2048];

        void draw();

        GameScreen();
        ~GameScreen();
};