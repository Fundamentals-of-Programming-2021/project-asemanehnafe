#ifndef STATE_IO_ATTACK_H
#define STATE_IO_ATTACK_H


struct attack{
    struct area* origin;
    struct area* Destination;
    short  number_solders_should_attack;
    short number_solders_inattack;
    struct solder solder[150];
    struct attack* next;
    double shib;
    double signx;
    short timer;
    short flag;
    short playerID;
};
void draw_attack(struct attack* head,SDL_Renderer *sdlRenderer,Uint32 color[]);
void add_at_tail(struct attack* head, struct area* source, struct area* destination);
void attack(struct attack* head,struct player player[]);
void crash( struct attack* head);
void delete(struct attack* head);
void produce_solder(struct attack* head);
#endif //STATE_IO_ATTACK_H
