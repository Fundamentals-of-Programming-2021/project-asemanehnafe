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
const int FPS = 60;

struct map{
    int seed;
    short area;
};

//struct area* comp_area[50];
//struct area* player_area[50];

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
        printf("you lost");
        player[0].point--;
        replace_Score(player[0].point,user);
    }
    if(counter==0){
        printf("youwin");
        player[0].point++;
        exit=1;
        replace_Score(player[0].point,user);
    }
    return exit;
}
SDL_Texture *getImageTexture(SDL_Renderer *sdlRenderer, char *image_path) {
    SDL_Surface *image = SDL_LoadBMP(image_path);

    /* Let the user know if the file failed to load */
    if (!image) {
        printf("Failed to load image at %s: %s\n", image_path, SDL_GetError());
        return 0;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(sdlRenderer, image);

    SDL_FreeSurface(image);
    image = NULL;

    return texture;
}
void computer_movement(short num_player,short num_area,struct area area[],struct player player[], struct attack* head) {
    //printf("%d",player[0].num_center);
    for (int i = 0; i < num_area; i++) {
        if (area[i].playerID == 0 && area[i].attacker == true && area[i].defender==false && player[0].num_center>1) {
            for (int j = 0; j < num_area; j++) {
                if (area[j].attacker == false && area[j].solders > area[i].solders && area[j].playerID != 0 &&
                    area[j].playerID != num_player) {
                    add_at_tail(head, &area[j], &area[i]);
                    return ;
                }
            }
        }
    }
    /*if(player[0].num_center==1 && player[num_player].num_center==0){
        for(int i=0;i<num_area;i++){
            if(area[i].playerID==0){
                for(int j=0;j<num_area;j++) {
                    if (area[j].playerID != 0 && area[j].playerID != num_player && area[j].solders>area[i].solders ){
                        add_at_tail(head, &area[j], &area[i]);
                        return;
                    }
                }
            }
        }
    }
    for(int i=0; i<num_area;i++){
        if(area[i] .playerID!=0 && area[i].playerID != num_player){
            for(int j=0;j<num_area;j++){
                if(area[j].playerID==0 && area[i].solders>area[j].solders&& player[0].num_center>1){
                    add_at_tail(head, &area[i], &area[j]);
                }
            }
        }
    }
    if(player[0].num_center==1){
        for(int i=0; i<num_area;i++){
            if(area[i] .playerID!=0 && area[i].playerID != num_player){
                for(int j=0;j<num_area;j++){
                    if(area[j].playerID==num_player && area[i].solders>area[j].solders){
                        add_at_tail(head, &area[i], &area[j]);
                    }
                }
            }
        }
    }*/
}
int main() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 0;
    }
    SDL_Window *sdlWindow = SDL_CreateWindow("Test_Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH,
                                             SCREEN_HEIGHT, SDL_WINDOW_OPENGL);

// error handling
    SDL_Renderer *sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

    SDL_bool shallExit = SDL_FALSE;

    short num_player = 3;
    struct player player[num_player+1];

//create texture for start page
    SDL_Texture *sdlTexture = getImageTexture(sdlRenderer, "Start.bmp");
    SDL_Rect texture_rect = {.x=0, .y=0, .w=SCREEN_WIDTH, .h=SCREEN_HEIGHT};
    SDL_bool start=SDL_FALSE;
    int len=0;
    char text[100];
    for(int i=0;i<100;i++){
        text[i]='\0';
    }

    while (shallExit==SDL_FALSE && start==SDL_FALSE){
        SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, &texture_rect);
//SDL_RenderClear(sdlRenderer);
// listen for key events
        SDL_Event sdlEvent;
        while (SDL_PollEvent(&sdlEvent)) {
            switch (sdlEvent.type) {
                case SDL_QUIT:
                    shallExit = SDL_TRUE;
                    break;
                case SDL_KEYDOWN:
                    if(sdlEvent.key.keysym.sym == SDLK_RETURN||sdlEvent.key.keysym.sym == SDLK_KP_ENTER){
                        start = SDL_TRUE;
                    }
                    else if(sdlEvent.key.keysym.sym == SDLK_BACKSPACE && len) {
                        len--;
                        text[len]='\0';
                    }
                    break;
                case SDL_TEXTINPUT:
                    // Add new text onto the end of our text
                    text[len]=*sdlEvent.text.text;
                    len ++;
                    break;

            }
        }
        stringRGBA(sdlRenderer,500,300,text,255,165,0,255);
//printf("%s",text);
        SDL_RenderPresent(sdlRenderer);
        SDL_Delay(1000 / FPS);
    }

//if the user is new it will join to users.txt with score 0 else we will read his score
    check_user(text,player);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    SDL_Texture *sdlTexture_second = getImageTexture(sdlRenderer, "second_page.bmp");
    SDL_Rect texture_second= {.x=0, .y=0, .w=SCREEN_WIDTH, .h=SCREEN_HEIGHT};
    SDL_bool second=SDL_FALSE,see_rank=SDL_FALSE,letsplay=SDL_FALSE;
    while (shallExit==SDL_FALSE && second==SDL_FALSE){
        SDL_RenderCopy(sdlRenderer, sdlTexture_second, NULL, &texture_second);
//SDL_RenderClear(sdlRenderer);
// listen for key events
        SDL_Event sdlEvent;
        while (SDL_PollEvent(&sdlEvent)) {
            switch (sdlEvent.type) {
                case SDL_QUIT:
                    shallExit = SDL_TRUE;
                    break;
                case SDL_MOUSEBUTTONUP:
                    if (sdlEvent.button.x < 720&& sdlEvent.button.x > 490 &&
                        sdlEvent.button.y <215 && sdlEvent.button.y >120) {
                        second=SDL_TRUE;
                        letsplay=SDL_TRUE;

                    }else if (sdlEvent.button.x < 720&& sdlEvent.button.x > 490 &&
                              sdlEvent.button.y <520 && sdlEvent.button.y >430) {
                        second=SDL_TRUE;
                        see_rank=SDL_TRUE;
                    }else if (sdlEvent.button.x < 720&& sdlEvent.button.x > 490 &&
                              sdlEvent.button.y < 360 && sdlEvent.button.y >250) {

                    }
                    /*  case SDL_MOUSEMOTION:

                          printf("%d,%d ",sdlEvent.motion.x,sdlEvent.motion.y);
                          break;*/
            }
        }
        SDL_RenderPresent(sdlRenderer);
        SDL_Delay(1000 / FPS);
    }
    //////////////////////////////////////////////////////////////////////////////////////////////////
    //create texture for See rank page
    int point;
    char user[100];
    sort_users(&point,user);
    strcat(user," with score : ");
    char buffer[100];
    sprintf(buffer,"%d",point);
    SDL_Texture *sdlTexture_rank = getImageTexture(sdlRenderer, "rank.bmp");
    SDL_Rect texture_rank = {.x=0, .y=0, .w=SCREEN_WIDTH, .h=SCREEN_HEIGHT};
    while (see_rank && shallExit==SDL_FALSE){
        SDL_RenderCopy(sdlRenderer, sdlTexture_rank, NULL, &texture_rank);
        stringRGBA(sdlRenderer,SCREEN_WIDTH/2-30,SCREEN_HEIGHT/2-30,user,255,255,255, 0xff);
        stringRGBA(sdlRenderer,SCREEN_WIDTH/2-4,SCREEN_HEIGHT/2-15,buffer,255,255,255, 0xff);
        // listen for key events
        SDL_Event sdlEvent;
        while (SDL_PollEvent(&sdlEvent)) {
            switch (sdlEvent.type) {
                case SDL_QUIT:
                    shallExit = SDL_TRUE;
                    break;
                case SDL_MOUSEBUTTONUP:
                    if (sdlEvent.button.x < 780 && sdlEvent.button.x > 422&&
                        sdlEvent.button.y < 482 && sdlEvent.button.y >440) {
                        see_rank=SDL_FALSE;
                        letsplay=SDL_TRUE;
                    }
                    /*  case SDL_MOUSEMOTION:

                          printf("%d,%d ",sdlEvent.motion.x,sdlEvent.motion.y);
                          break;*/
            }
        }
        SDL_RenderPresent(sdlRenderer);
        SDL_Delay(1000 / FPS);
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    struct  map map;
    //create texture for choose_map page
    SDL_Texture *sdlTexture_map = getImageTexture(sdlRenderer, "choosemap.bmp");
    SDL_Rect texture_map = {.x=0, .y=0, .w=SCREEN_WIDTH, .h=SCREEN_HEIGHT};
    SDL_bool mapbool=SDL_FALSE;
    while (letsplay && shallExit==SDL_FALSE && mapbool==SDL_FALSE){
        SDL_RenderCopy(sdlRenderer, sdlTexture_map, NULL, &texture_map);
        //SDL_RenderClear(sdlRenderer);
        // listen for key events
        SDL_Event sdlEvent;
        while (SDL_PollEvent(&sdlEvent)) {
            switch (sdlEvent.type) {
                case SDL_QUIT:
                    shallExit = SDL_TRUE;
                    break;
                case SDL_MOUSEBUTTONUP:
                    if (sdlEvent.button.x < 410 && sdlEvent.button.x > 145&&
                        sdlEvent.button.y <280 && sdlEvent.button.y >130) {
                        map.seed=78000;
                        map.area=19;
                        mapbool=SDL_TRUE;
                    }
                    else if (sdlEvent.button.x < 410 && sdlEvent.button.x > 145&&
                             sdlEvent.button.y < 500 && sdlEvent.button.y >355) {
                        map.seed=600;
                        map.area=20;
                        mapbool=SDL_TRUE;

                    }
                    else if (sdlEvent.button.x < 1055&& sdlEvent.button.x >810&&
                             sdlEvent.button.y <280 && sdlEvent.button.y >130) {
                        map.seed=400;
                        map.area=13;
                        mapbool=SDL_TRUE;
                    }
                    else if (sdlEvent.button.x < 1055&& sdlEvent.button.x >810&&
                             sdlEvent.button.y < 500 && sdlEvent.button.y >355) {
                        map.seed=10000;
                        map.area=16;
                        mapbool=SDL_TRUE;
                    }
                    else if (sdlEvent.button.x < 749&& sdlEvent.button.x > 483&&
                             sdlEvent.button.y < 400 && sdlEvent.button.y >255) {
                        time_t t1;
                        srand ( &t1);
                        map.seed=rand();
                        map.area=16;
                        mapbool=SDL_TRUE;

                    }
                    /*case SDL_MOUSEMOTION:

                        printf("%d,%d ",sdlEvent.motion.x,sdlEvent.motion.y);
                        break;*/
            }
        }
        //printf("%s",text);
        SDL_RenderPresent(sdlRenderer);
        SDL_Delay(1000 / FPS);
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Uint32 colors[10]={0x7514A0EB,0x753C4CE7,0x75B6C673,0x6593570D,0x65AAE082 ,0x656B6A61,0x65BD69A5,0x65BD69A5};
    Uint32 colorfull[10]={0xff14A0EB,0xff3C4CE7,0xffB6C673,0xff93570D,0xffAAE082 ,0xff6B6A61,0xffBD69A5,0xffBD69A5};
    Uint32 potion_colors[4]={0xff3FF45B,0xffF43FAD,0xff0303FC,0xffF403FC};
    Sint16 x = (Sint16)(SCREEN_WIDTH/3), y = (Sint16)(SCREEN_HEIGHT/2);
    // initialize centers of hexagons & players
    short num_area=map.area;
    struct area area[num_area];

    for(int i=0;i<num_player;i++){
        player[i].color=colors[i];
        player[i].live=SDL_TRUE;
        player[i].point=0;
        player[i].num_center=0;
        player[i].velocity=2;
        player[i].can_attack_to=true;
        player[i].produce_solders_timerate=100;
        player[i].potioned=false;
    }

    //the last player is always neutral
    player[num_player].color=0x45F4F3F0;
    player[num_player].live=SDL_TRUE;
    player[num_player].point=0;
    player[num_player].can_attack_to=true;
    player[num_player].velocity=0;
    player[num_player].produce_solders_timerate=100;
    player[num_player].num_center=0;

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
            //create coordinate & kind of potions randomly
            random_potion(num_area,area,potion_head);
        }
        //computer movement
        //computer_movement(num_player,num_area,area,player,head);
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

        //draw number of barracks solders
        timer++;
        for(int i=0;i<num_area;i++){
            if((timer%(player[area[i].playerID].produce_solders_timerate))==0 &&area[i].playerID != num_player&&area[i].solders<99){
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
                    break;
                case SDL_MOUSEBUTTONUP:
                    // printf("%d %d",sdlEvent.button.x,centers[0][0]);
                    for(short i=0;i<num_area;i++){
                        if ((sdlEvent.button.x - area[i].x) * (sdlEvent.button.x - area[i].x) +
                            (sdlEvent.button.y - area[i].y) * (sdlEvent.button.y - area[i].y) <= 150) {
                            if (first_click && origin != i && player[area[i].playerID].can_attack_to) {
                                //an attack is on
                                first_click = SDL_FALSE;
                                add_at_tail(head, &area[origin], &area[i]);
                            } else if(first_click==SDL_FALSE) {
                                if ( area[i].playerID == 0 ) {
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

    SDL_DestroyWindow(sdlWindow);
    SDL_Quit();
    return 0;
}