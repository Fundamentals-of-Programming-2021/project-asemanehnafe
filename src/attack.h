#ifndef STATE_IO_ATTACK_H
#define STATE_IO_ATTACK_H

struct attack{
    struct area* origin;
    struct area* Destination;
    short  number_solders_should_attack;
    short number_solders_inattack;
    struct solder* solder;
    struct attack* next;
    double shib;
    double signx;
    short timer;
    short playerID;

};
struct solder{
    bool live;
    double x;
    double y;
    bool power;
};
void draw_attack(struct attack* head,SDL_Renderer *sdlRenderer,Uint32 color[]);
void add_at_tail(struct attack* head, struct area* source, struct area* destination);
void attack(struct attack* head,struct player player[]);
void computer_movement(short num_player,short num_area,struct area area[],struct player player[], struct attack* head);
#endif //STATE_IO_ATTACK_H
