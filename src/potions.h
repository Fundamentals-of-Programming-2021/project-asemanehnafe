#ifndef STATE_IO_POTION_H
#define STATE_IO_POTION_H
struct potion{
    bool active;
    short ID;
    short playerID;
    short timer;
    struct potion* next;
    short x;
    short y;
};
void random_potion(short num_area,struct area area[num_area],struct potion* potion);
void deactivate_potion(struct potion* potion_head,struct player player[],struct area area[],short num_player,short area_num);
void active_check(struct potion* potion_head,struct attack* head, short  num_player, short  num_area, struct player player[], struct area area[]);
void draw_potion(SDL_Renderer *sdlRenderer,struct potion* head,Uint32 color[4]);
void draw_potion_time(SDL_Renderer *sdlRenderer,struct potion* potion_head,Uint32 potion_colors[4]);
#endif //STATE_IO_POTION_H
