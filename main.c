#include <SDL.h>
#include <SDL2_gfxPrimitives.h>

#ifdef main
#undef main
#endif

#include <stdio.h>
#include <stdbool.h>
int main(){

        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
            printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
            return 0;
        }

        SDL_Window *sdlWindow = SDL_CreateWindow("Test_Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH,
                                         SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
        // error handling
        SDL_Renderer *sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

        while (true) {
            SDL_SetRenderDrawColor(sdlRenderer, 0xff, 0xff, 0xff, 0xff);
            SDL_RenderClear(sdlRenderer);

            // draw box using gfx_primitives
            Uint32 color = 0xffffff00;
            Sint16 x1 = 60, x2 = 280;
            Sint16 y1 = 160, y2 = 320;
            boxColor(sdlRenderer, x1, y1, x2, y2, color);

            // draw circle using gfx_primitives
            Uint32 color2 = 0xff0000ff;
            Sint16 x = 480, y = 240, r = 100;
            filledCircleColor(sdlRenderer, x, y, r, color2);

            SDL_RenderPresent(sdlRenderer);
            SDL_Delay(1000 / FPS);
        }
        SDL_Delay(1000);
        SDL_DestroyWindow(sdlWindow);
        SDL_Quit();
        return 0;
}