#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
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
void random_potion(short num_area,struct area area[num_area],struct potion* potion){
    while(potion->next!=NULL){
        potion=potion->next;
    }
    struct potion* new_potion = (struct potion*)(malloc(sizeof(struct potion)));
    time_t t1;
    srand ( (unsigned) time (&t1));
    short first = rand() % num_area;
    short second= rand() % num_area;
    while(first == second){
        second=rand() % num_area;
    }
    new_potion->x=(area[first].x + area[second].x)/2;
    new_potion->y=(area[first].y+area[second].y)/2;
    new_potion->timer=2000;
    new_potion->active=false;
    new_potion->ID=rand()%4;
    new_potion->next=NULL;
    potion->next=new_potion;
    //double distance=sqrt((area[first].x - area[second].x) * (area[first].x - area[second].x) + (area[first].y - area[second].y)*(area[first].y  - area[second].y));
}
void delete_potion(struct potion* head){
    if(head->next->next==NULL){
        free(head->next);
        head->next=NULL;
    }else{
        struct potion* temp=head->next;
        head->next=head->next->next;
        free(temp);
    }
}
void deactivate_potion(struct potion* potion_head,struct player player[],struct area area[],short num_player,short area_num){
    while(potion_head->next !=NULL) {
        struct potion *temp = potion_head;
        potion_head = potion_head->next;
        if(potion_head->active==true)
            potion_head->timer--;
        if ( potion_head->timer == 0 && potion_head->active == true){
            switch (potion_head->ID) {
                //players potions
                //first potion
                case 1:
                    player[potion_head->playerID].velocity /=2;
                    break;
                    //second potion
                case 2:
                    for(int i=0;i<num_player;i++){
                        if(i != potion_head->playerID){
                            player[i].velocity *=2;
                        }
                    }
                    break;
                    //barracks potions
                    //decreasing time rate of produce=ing solder
                case 3:
                    player[potion_head->playerID].produce_solders_timerate/=2;
                    break;
                case 0:
                    player[potion_head->playerID].can_attack_to = true;
                    break;
            }
            player[potion_head->playerID].potioned=false;
            potion_head=temp;
            delete_potion(temp);
        }
    }
}
void activate_potion(struct player player[],struct area area[],struct potion* potion,short num_player,short player_ID,short area_num){
    potion->active=true;
    switch(potion->ID){
        //players potions
        //first potion: *1.5 player velocity_Purple
        case 1:
            player[player_ID].velocity*=2;
            break;
            //second potion: /2 op velocity_red
        case 2:
            for(int i=0;i<num_player;i++){
                if(i != player_ID){
                    player[i].velocity/=2;
                }
            }
            break;
            //barracks potions
            //decreasing time rate of producing solder _pink
        case 3:
            player[player_ID].produce_solders_timerate*=2;
            break;
        case 0:
            //enemies can not attack_green
            player[player_ID].can_attack_to = false;

            break;
    }

}
void active_check(struct potion* potion_head,struct attack* head, short  num_player, short  num_area, struct player player[], struct area area[]){
    while(head->next != NULL) {
        head = head->next;
        if (player[head->playerID].potioned == false) {
            struct potion* potion_temp=potion_head;
            while (potion_head->next != NULL) {
                potion_head = potion_head->next;
                if(potion_head->active==false){
                    for (short i = 0; i < head->number_solders_inattack; i++) {
                        if (abs(head->solder[i].x - potion_head->x) <= 10 &&
                            abs(head->solder[i].y - potion_head->y) <= 10) {
                            potion_head->playerID = head->playerID;
                            player[head->playerID].potioned = true;
                            activate_potion(player, area, potion_head, num_player, head->playerID, num_area);
                        }
                    }
                }
            }
            potion_head = potion_temp;
        }
    }
}
void draw_potion(SDL_Renderer *sdlRenderer,struct potion* head,Uint32 color[4]){
    while(head->next!=NULL){
        head=head->next;
        if(head->active==false)
            filledTrigonColor(sdlRenderer, (Sint16)(5+head->x), (Sint16)(head->y-5), (Sint16)(head->x), (Sint16)(head->y+5), (Sint16)(head->x-5),  (Sint16)(head->y-5), color[head->ID]);
    }
}
void draw_potion_time(SDL_Renderer *sdlRenderer,struct potion* potion_head,Uint32 potion_colors[4]){
    while(potion_head->next!=NULL){
        potion_head=potion_head->next;
        if(potion_head->active){
            int i=potion_head->playerID+1;
            filledTrigonColor(sdlRenderer, (Sint16)(SCREEN_WIDTH*3/4-30),(Sint16)(i*30+8),(Sint16)(SCREEN_WIDTH*3/4-38),(Sint16)(i*30+32),(Sint16)(SCREEN_WIDTH*3/4-22),(Sint16)(i*30+32),potion_colors[potion_head->ID]);
            roundedBoxColor (sdlRenderer, (Sint16)(SCREEN_WIDTH*3/4+10),(Sint16)(i*30+8),(Sint16)(SCREEN_WIDTH*3/4+10+(potion_head->timer*2/25)),(Sint16)(i*30+32), 2, 0x45F4F3F0);
        }
    }
}
