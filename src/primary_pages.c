#include <stdbool.h>
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "map.h"
#include "users_and_scores.h"
#include "primary_pages.h"
 SDL_bool letsplay=SDL_FALSE;
 SDL_bool shallExit= SDL_FALSE;
 struct  map map;
 char text[100];
int score;
SDL_bool second=SDL_FALSE;
SDL_bool see_rank=SDL_FALSE;
short num_player;
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
void first_page(SDL_Renderer *sdlRenderer){
    //create texture for start page
    SDL_Texture *sdlTexture = getImageTexture(sdlRenderer, "Start.bmp");
    SDL_Rect texture_rect = {.x=0, .y=0, .w=SCREEN_WIDTH, .h=SCREEN_HEIGHT};
    SDL_bool start=SDL_FALSE;
    int len=0;

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
                    SDL_DestroyRenderer(sdlRenderer);
                    SDL_DestroyTexture(sdlTexture);
                    break;
                case SDL_KEYDOWN:
                    if(sdlEvent.key.keysym.sym == SDLK_RETURN||sdlEvent.key.keysym.sym == SDLK_KP_ENTER){
                        SDL_DestroyTexture(sdlTexture);
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
        SDL_RenderPresent(sdlRenderer);
        SDL_Delay(1000 / FPS);
    }
    //if the user is new it will join to users.txt with score 0 else we will read his score
    score=check_user(text);

}

void menu(SDL_Renderer *sdlRenderer){

    SDL_Texture *sdlTexture_second = getImageTexture(sdlRenderer, "second_page.bmp");
    SDL_Rect texture_second= {.x=0, .y=0, .w=SCREEN_WIDTH, .h=SCREEN_HEIGHT};

    while (shallExit==SDL_FALSE && second==SDL_FALSE){
        SDL_RenderCopy(sdlRenderer, sdlTexture_second, NULL, &texture_second);
        //SDL_RenderClear(sdlRenderer);
        // listen for key events
        SDL_Event sdlEvent;
        while (SDL_PollEvent(&sdlEvent)) {
            switch (sdlEvent.type) {
                case SDL_QUIT:
                    shallExit = SDL_TRUE;
                    SDL_DestroyRenderer(sdlRenderer);
                    SDL_DestroyTexture(sdlTexture_second);
                    break;
                case SDL_MOUSEBUTTONUP:
                    if (sdlEvent.button.x < 720&& sdlEvent.button.x > 490 &&
                        sdlEvent.button.y <215 && sdlEvent.button.y >120) {
                        second=SDL_TRUE;
                        letsplay=SDL_TRUE;
                        SDL_DestroyTexture(sdlTexture_second);

                    }else if (sdlEvent.button.x < 720&& sdlEvent.button.x > 490 &&
                              sdlEvent.button.y <520 && sdlEvent.button.y >430) {
                        second=SDL_TRUE;
                        see_rank=SDL_TRUE;
                        SDL_DestroyTexture(sdlTexture_second);
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
}
void rank_page(SDL_Renderer *sdlRenderer){
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
                    SDL_DestroyRenderer(sdlRenderer);
                    SDL_DestroyTexture(sdlTexture_rank);
                    break;
                case SDL_MOUSEBUTTONUP:
                    if (sdlEvent.button.x < 780 && sdlEvent.button.x > 422&&
                        sdlEvent.button.y < 482 && sdlEvent.button.y >440) {
                        see_rank=SDL_FALSE;
                        letsplay=SDL_TRUE;
                        SDL_DestroyTexture(sdlTexture_rank);
                    }
                    /*  case SDL_MOUSEMOTION:

                          printf("%d,%d ",sdlEvent.motion.x,sdlEvent.motion.y);
                          break;*/
            }
        }
        SDL_RenderPresent(sdlRenderer);
        SDL_Delay(1000 / FPS);
    }
}
void getrandom(SDL_Renderer *sdlRenderer){
    SDL_bool got=SDL_FALSE;
        SDL_Texture *sdlTexture = getImageTexture(sdlRenderer, "randomget_page.bmp");
        SDL_Rect texture_rect = {.x=0, .y=0, .w=SCREEN_WIDTH, .h=SCREEN_HEIGHT};
        int len=0;
        char temp[100];
        for(int i=0;i<100;i++){
            temp[i]='\0';
        }
        while (shallExit==SDL_FALSE && got==SDL_FALSE){
            SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, &texture_rect);
            //SDL_RenderClear(sdlRenderer);
            // listen for key events
            SDL_Event sdlEvent;
            while (SDL_PollEvent(&sdlEvent)) {
                switch (sdlEvent.type) {
                    case SDL_QUIT:
                        shallExit = SDL_TRUE;
                        SDL_DestroyRenderer(sdlRenderer);
                        SDL_DestroyTexture(sdlTexture);
                        break;
                    case SDL_KEYDOWN:
                        if(sdlEvent.key.keysym.sym == SDLK_RETURN||sdlEvent.key.keysym.sym == SDLK_KP_ENTER){
                            SDL_DestroyTexture(sdlTexture);
                            got = SDL_TRUE;
                        }
                        else if(sdlEvent.key.keysym.sym == SDLK_BACKSPACE && len) {
                            len--;
                            temp[len]='\0';
                        }
                        break;
                    case SDL_TEXTINPUT:
                        // Add new text onto the end of our text
                        temp[len]=*sdlEvent.text.text;
                        len ++;
                        break;
                }
            }
            stringRGBA(sdlRenderer,350,275,"Enter area number and player number whit an space between them, then press Enter",255,165,0,255);
            stringRGBA(sdlRenderer,350,295,"or write a '+' then choose the number of past random map which is saved ",255,165,0,255);
            stringRGBA(sdlRenderer,500,325,temp,255,165,0,255);
            SDL_RenderPresent(sdlRenderer);
            SDL_Delay(1000 / FPS);
        }
        //change text to number
        if(temp[0]=='+'){
            int map_number=0;
            for(int x=1;temp[x] != '\0';x++){
                map_number*=10;
                map_number=(short)(temp[x])-48+map_number;
            }
            FILE *file= fopen("radom_maps.txt","r");
            for( int i=0;i<map_number;i++){
                fscanf(file,"%hd %hd %d",&map.area,&num_player,&map.seed);
            }
            printf("%hd %hd %hd",map.area,num_player,map.seed);
            fclose(file);

        }else{
            map.area=0;
            num_player=0;
            int x ;
            for(x=0;temp[x] != ' ';x++){
                map.area*=10;
                map.area=(short)(temp[x])-48+map.area;
            }
            x++;
            for(;temp[x] != '\0';x++){
                num_player*=10;
                num_player=(short)temp[x]-48+num_player;
            }
            time_t t1;
            srand ( &t1);
            map.seed=rand();
            //save area player seed
            FILE *file= fopen("radom_maps.txt","a+");
            fprintf(file,"\n%hd %hd %d",map.area,num_player,map.seed);
            fclose(file);
            }
}
void chose_map(SDL_Renderer *sdlRenderer){
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
                    SDL_DestroyRenderer(sdlRenderer);
                    SDL_DestroyTexture(sdlTexture_map);
                    shallExit = SDL_TRUE;
                    break;
                case SDL_MOUSEBUTTONUP:
                    if (sdlEvent.button.x < 410 && sdlEvent.button.x > 145&&
                        sdlEvent.button.y <280 && sdlEvent.button.y >130) {
                        map.seed=78000;
                        map.area=19;
                        num_player=3;
                        mapbool=SDL_TRUE;
                        SDL_DestroyTexture(sdlTexture_map);
                    }
                    else if (sdlEvent.button.x < 410 && sdlEvent.button.x > 145&&
                             sdlEvent.button.y < 500 && sdlEvent.button.y >355) {
                        map.seed=600;
                        map.area=20;
                        num_player=3;
                        mapbool=SDL_TRUE;
                        SDL_DestroyTexture(sdlTexture_map);
                    }
                    else if (sdlEvent.button.x < 1055&& sdlEvent.button.x >810&&
                             sdlEvent.button.y <280 && sdlEvent.button.y >130) {
                        map.seed=400;
                        map.area=13;
                        num_player=3;
                        mapbool=SDL_TRUE;
                        SDL_DestroyTexture(sdlTexture_map);
                    }
                    else if (sdlEvent.button.x < 1055&& sdlEvent.button.x >810&&
                             sdlEvent.button.y < 500 && sdlEvent.button.y >355) {
                        map.seed=10000;
                        map.area=16;
                        num_player=3;
                        //for memory check
/*                        map.area=12;
                        num_player=2;*/
                        mapbool=SDL_TRUE;
                        SDL_DestroyTexture(sdlTexture_map);
                    }
                    else if (sdlEvent.button.x < 749&& sdlEvent.button.x > 483&&
                             sdlEvent.button.y < 400 && sdlEvent.button.y >255) {
                        getrandom(sdlRenderer);
                        mapbool=SDL_TRUE;
                        SDL_DestroyTexture(sdlTexture_map);
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
}