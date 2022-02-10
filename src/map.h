#ifndef STATE_IO_MAP_H
#define STATE_IO_MAP_H
#define SCREEN_WIDTH 1150
#define SCREEN_HEIGHT 600
struct area{
    short x;
    short y;
    short playerID;
    short solders;
    bool defender;
    bool attacker;
};

struct solder{
    bool live;
    double x;
    double y;
    //short player;
};

struct player{
    Uint32 color;
    SDL_bool live;
    int point;
    short num_center;
    short centers[2][100];
    short velocity;
    SDL_bool can_attack_to;
    short produce_solders_timerate;
    bool potioned;
};

void print();
void draw_hexagon(SDL_Renderer *sdlRenderer,Sint16 x,Sint16 y, Uint32 color);
void centers_of_hexagonals(int numarea, struct area area[numarea], Sint16 x, Sint16 y, int numplayer, struct player player[],int seed);
#endif //STATE_IO_MAP_H
