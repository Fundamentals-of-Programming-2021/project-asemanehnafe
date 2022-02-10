#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#ifdef main
#undef main
#endif
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <sys/time.h>
#include <math.h>
#include <string.h>
#include "map.h"
#include "attack.h"
#include "potions.h"
#include "users_and_scores.h"
#include "primary_pages.h"
#include "play_page.h"
int main() {
    // error handling
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 0;
    }
    SDL_Window *sdlWindow = SDL_CreateWindow("Test_Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH,
                                             SCREEN_HEIGHT, SDL_WINDOW_OPENGL);

    SDL_Renderer *sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

    first_page(sdlRenderer);
    menu(sdlRenderer);
    if(shallExit ==SDL_FALSE&& see_rank)
        rank_page(sdlRenderer);
    if(shallExit ==SDL_FALSE&& letsplay){
        chose_map(sdlRenderer);
        play(sdlRenderer);}

    SDL_DestroyWindow(sdlWindow);
    SDL_Quit();
    return 0;
}