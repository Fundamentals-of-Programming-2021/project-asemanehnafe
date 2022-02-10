#ifndef MAIN_C_PRIMARY_PAGES_H
#define MAIN_C_PRIMARY_PAGES_H
#define  FPS 60
extern SDL_bool letsplay;
extern SDL_bool shallExit;
struct map{
    int seed;
    short area;
};
extern struct  map map;
extern char text[100];
extern int score;
extern SDL_bool second;
extern SDL_bool see_rank;
extern short num_player;
SDL_Texture *getImageTexture(SDL_Renderer *sdlRenderer, char *image_path) ;

void first_page(SDL_Renderer *sdlRenderer);

void menu(SDL_Renderer *sdlRenderer);

void rank_page(SDL_Renderer *sdlRenderer);

void chose_map(SDL_Renderer *sdlRenderer);

#endif //MAIN_C_PRIMARY_PAGES_H
