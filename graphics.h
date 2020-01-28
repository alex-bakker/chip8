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
        
        void draw(Uint32 *pixels);

        GameScreen();
        ~GameScreen();
};