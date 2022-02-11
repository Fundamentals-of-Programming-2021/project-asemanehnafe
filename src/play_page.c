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
#include "users_and_scores.h"
#include "primary_pages.h"
#include "play_page.h"
int end_of_GAME(struct area area[],short num_area,short num_player,struct player player[],char user[100]){
    int exit=1;
    int i,counter=0;
    for(i=0;i<num_area;i++) {
        if (area[i].playerID == 0 ) {
            exit = 0;
        }
        if(area[i].playerID!=0 && area[i].playerID!=num_player)
            counter++;
    }
    if(exit){
        player[0].point--;
        replace_Score(player[0].point,user);
    }
    if(counter==0){
        player[0].point++;
        exit=1;
        replace_Score(player[0].point,user);
    }
    return exit;
}
void play(SDL_Renderer *sdlRenderer){
    Uint32 colors[10]={0x7514A0EB,0x753C4CE7,0x75B6C673,0x6593570D,0x65AAE082 ,0x656B6A61,0x65BD69A5,0x65BD69A5};
    Uint32 colorfull[10]={0xff14A0EB,0xff3C4CE7,0xffB6C673,0xff93570D,0xffAAE082 ,0xff6B6A61,0xffBD69A5,0xffBD69A5};
    Uint32 potion_colors[8]={0xff3FF45B,0xffF43FAD,0xff0303FC,0xffF403FC,0xffFDF300,0xff03ECFC,0xffF97709,0xff06259B};
    Sint16 x = (Sint16)(SCREEN_WIDTH/3), y = (Sint16)(SCREEN_HEIGHT/2);
    // initialize centers of hexagons & players
    struct player player[num_player+1];
    player[0].point=score;
    short num_area=map.area;
    struct area area[num_area];

    for(int i=0;i<num_player;i++){
        player[i].color=colors[i];
        player[i].live=SDL_TRUE;
        player[i].num_center=0;
        player[i].velocity=2;
        player[i].can_attack_to=true;
        player[i].produce_solders_timerate=100;
        player[i].potioned=false;
        player[i].increase=false;
        player[i].bimax_solder=false;
        player[i].power=false;
        player[i].move=true;

    }

    //the last player is always neutral
    player[num_player].color=0x45F4F3F0;
    player[num_player].live=SDL_TRUE;
    player[num_player].can_attack_to=true;
    player[num_player].velocity=0;
    player[num_player].produce_solders_timerate=100;
    player[num_player].num_center=0;
    player[num_player].increase=false;
    player[num_player].bimax_solder=false;
    player[num_player].power=false;
    player[num_player].move=false;



    centers_of_hexagonals(num_area,area,x,y,num_player, player,map.seed);

    struct timeval te;
    SDL_bool first_click=SDL_FALSE;

    short timer=0;

    struct attack* head=(struct attack*)(malloc(sizeof(struct attack)));
    head ->next=NULL;
    short origin;

    struct potion* potion_head= malloc(sizeof(struct potion));
    potion_head->next=NULL;

    while (letsplay&&shallExit==SDL_FALSE) {

        SDL_SetRenderDrawColor(sdlRenderer, 0x17, 0x20, 0x2A, 0xff);
        SDL_RenderClear(sdlRenderer);

        //check for attacks
        if(head ->next != NULL){
            attack(head,player);
        }

        //check for time to create a potion
        gettimeofday(&te, NULL);
        long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000;
        if(milliseconds % 1000==0){
            if(rand()%4==0)
            //create coordinate & kind of potions randomly
            random_potion(num_area,area,potion_head);
        }
        //computer movement
        computer_movement(num_player,num_area,area,player,head);
        //check for "end" of GAME
        if(end_of_GAME(area,num_area,num_player,player,text))
            shallExit=SDL_TRUE;

        if(potion_head->next != NULL) {
            //check for potions activate
            if (head->next != NULL) {
                active_check(potion_head, head, num_player, num_area, player, area);
            }
            //check deactivations
            deactivate_potion(potion_head,player,area,num_player,num_area);
        }

        // draw area and barracks
        for(int i=0;i<num_area;i++) {
            draw_hexagon(sdlRenderer, area[i].x, area[i].y, player[area[i].playerID].color);
            filledCircleColor(sdlRenderer,  area[i].x, area[i].y, 12, player[area[i].playerID].color);
        }
        //draw play information
        stringRGBA(sdlRenderer,(Sint16)(SCREEN_WIDTH*3/4+8),27,"potion_timer",255,255,255, 0xff);
        for(int i=0;i<num_player;i++) {
            i++;
            roundedBoxColor (sdlRenderer, (Sint16)(SCREEN_WIDTH*3/4+10),(Sint16)(i*30+8),(Sint16)(SCREEN_WIDTH*3/4+170),(Sint16)(i*30+32), 2, 0x45F4F3F0);
            filledTrigonColor(sdlRenderer, (Sint16)(SCREEN_WIDTH*3/4-30),(Sint16)(i*30+8),(Sint16)(SCREEN_WIDTH*3/4-38),(Sint16)(i*30+32),(Sint16)(SCREEN_WIDTH*3/4-22),(Sint16)(i*30+32),0x45F4F3F0);
            filledCircleColor(sdlRenderer,  (Sint16)(SCREEN_WIDTH*3/4-6),(Sint16)(i*30+20), 12, player[i-1].color);
            i--;
        }
        stringRGBA(sdlRenderer,(Sint16)(SCREEN_WIDTH*3/4),SCREEN_HEIGHT-27,"GREEN POTION: ENEMIES CANT ATTACK",255,255,255, 0xff);
        stringRGBA(sdlRenderer,(Sint16)(SCREEN_WIDTH*3/4),SCREEN_HEIGHT-27*2,"PURPLE POTION:  YOR VELOCITY * 2",255,255,255, 0xff);
        stringRGBA(sdlRenderer,(Sint16)(SCREEN_WIDTH*3/4),SCREEN_HEIGHT-27*3,"PINK:PRODUCING SOLDERS TIME RATE/2",255,255,255, 0xff);
        stringRGBA(sdlRenderer,(Sint16)(SCREEN_WIDTH*3/4),SCREEN_HEIGHT-27*4,"RED POTION: OPPONENT SPEED /2",255,255,255, 0xff);
        stringRGBA(sdlRenderer,(Sint16)(SCREEN_WIDTH*3/4),SCREEN_HEIGHT-27*5,"YELLOW POTION: YOU CAN'T MOVE",255,255,255, 0xff);
        stringRGBA(sdlRenderer,(Sint16)(SCREEN_WIDTH*3/4),SCREEN_HEIGHT-27*6,"ORANGE POTION :ALWAYS PRODUCE ",255,255,255, 0xff);
        stringRGBA(sdlRenderer,(Sint16)(SCREEN_WIDTH*3/4),SCREEN_HEIGHT-27*7,"BLUE POTION : INCREASE IN ATTACK",255,255,255, 0xff);
        stringRGBA(sdlRenderer,(Sint16)(SCREEN_WIDTH*3/4),SCREEN_HEIGHT-27*8,"DARKBLUE POTION : YOUR POWER * 2",255,255,255, 0xff);

        //draw number of barracks solders
        timer++;
        for(int i=0;i<num_area;i++){
            if((timer%(player[area[i].playerID].produce_solders_timerate))==0 &&area[i].playerID != num_player && (area[i].solders<99 || player[area[i].playerID].bimax_solder)){
                area[i].solders++;
            }
            char buffer[50];
            sprintf(buffer,"%d",area[i].solders);
            stringRGBA(sdlRenderer, area[i].x-4, area[i].y-3,buffer,0,0,0,255);
        }
        if(timer==200)
            timer=0;

        //draw attacks
        if(head -> next != NULL){
            draw_attack(head,sdlRenderer,colorfull);
        }

        //draw potions
        if(potion_head->next != NULL){
            draw_potion(sdlRenderer,potion_head,potion_colors);
            draw_potion_time(sdlRenderer,potion_head,potion_colors);
        }
        // listen for key events
        SDL_Event sdlEvent;
        while (SDL_PollEvent(&sdlEvent)) {
            switch (sdlEvent.type) {
                case SDL_QUIT:
                    shallExit = SDL_TRUE;
                    SDL_DestroyRenderer(sdlRenderer);
                    break;
                case SDL_MOUSEBUTTONUP:
                    // printf("%d %d",sdlEvent.button.x,centers[0][0]);
                    for(short i=0;i<num_area;i++){
                        if ((sdlEvent.button.x - area[i].x) * (sdlEvent.button.x - area[i].x) +
                            (sdlEvent.button.y - area[i].y) * (sdlEvent.button.y - area[i].y) <= 150) {
                            if (first_click && origin != i && (player[area[i].playerID].can_attack_to || area[i].playerID == area[origin].playerID)) {
                                //an attack is on
                                first_click = SDL_FALSE;
                                add_at_tail(head, &area[origin], &area[i]);
                            }else if(first_click==SDL_FALSE) {
                                if ( area[i].playerID == 0) {
                                    first_click = SDL_TRUE;
                                    origin = i;
                                }
                            }
                        }
                    }
                    break;
            }
        }

        SDL_RenderPresent(sdlRenderer);
        SDL_Delay(1000 / FPS);


    }
    free(head);
    free(potion_head);

}