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
const int SCREEN_WIDTH = 1250;
const int SCREEN_HEIGHT = 600;
const int FPS = 60;


struct player{
    Uint32 color;
    SDL_bool live;
    short point;
    short num_center;
    short centers[2][100];
    short velocity;
    bool can_attack_to;
    short produce_solders_timerate;
    bool potioned;
};
struct area{
    short x;
    short y;
    short playerID;
    short solders;
};

struct potion{
    bool active;
    short ID;
    short playerID;
    short timer;
    struct potion* next;
    short x;
    short y;
};
struct solder{
    bool live;
    double x;
    double y;
    //short player;
};
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

//struct area* comp_area[50];
//struct area* player_area[50];
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
            printf("\ntimout\n");
            switch (potion_head->ID) {
                //players potions
                //first potion
                case 1:
                    player[potion_head->playerID].velocity = 2;
                    break;
                    //second potion
                case 2:
                    for(int i=0;i<num_player;i++){
                        if(i != potion_head->playerID){
                            player[i].velocity=2;
                        }
                    }
                    break;
                    //barracks potions
                    //decreasing time rate of produce=ing solder
                case 3:
                    player[potion_head->playerID].produce_solders_timerate=100;
                    break;
                case 0:
                    player[potion_head->playerID].can_attack_to = true;
                    break;
            }
            player[potion_head->playerID].potioned=false;
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
            printf("*1.5 velocity");
            player[player_ID].velocity=4;
            break;
            //second potion: /2 op velocity_red
        case 2:
            printf("dec velocity");
            for(int i=0;i<num_player;i++){
                if(i != player_ID){
                    player[i].velocity=1;
                }
            }
            break;
            //barracks potions
            //decreasing time rate of producing solder _pink
        case 3:
            printf("time rate");
            player[player_ID].produce_solders_timerate=200;
            break;
        case 0:
            //enemies can not attack_green
            printf("can attack ");
            player[player_ID].can_attack_to = false;


            break;
    }

}
void active_check(struct potion* potion_head,struct attack* head, short  num_player, short  num_area, struct player player[], struct area area[]){
    while(head->next != NULL) {
        head = head->next;
        if (player[head->playerID].potioned == false) {
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
void produce_solder(struct attack* head){
    if(head-> origin->solders >= 0 ){
        head->solder[head->number_solders_inattack].x=head->origin->x;
        head->solder[head->number_solders_inattack].y=head->origin->y;
        head->solder[head->number_solders_inattack].live=true;
        head -> number_solders_inattack++;
        head->origin->solders--;
    }
}
void delete(struct attack* head){
    if(head->next->next==NULL){
        head->next=NULL;
    }else{
        struct attack* temp=head->next;
        head->next=head->next->next;
        free(temp);
    }
}
void crash( struct attack* head){
/*    int hamle=0;
    printf("\n");
    hamle++;
    printf("%d",hamle);*/
    for(struct attack* temp=head->next; temp!=NULL; temp=temp->next){
        //ehtemal???
        if(head->playerID!= temp->playerID){
            for(short i=0;i < head->number_solders_inattack;i++){
                for(short j=0; j < temp->number_solders_inattack;j++){
                    if(head->solder[i].live==true&&temp->solder[j].live==true&&(head->solder[i].x - temp -> solder[j].x)*(head->solder[i].x - temp -> solder[j].x)+ (head->solder[i].y - temp -> solder[j].y )*(head->solder[i].y - temp -> solder[j].y )<30 ){
                        head->solder[i].live=false;
                        temp->solder[j].live=false;
                    }
                }
            }
        }
    }
}
void attack(struct attack* head,struct player player[]){

    while(head->next != NULL) {
        struct attack* temp=head;
        head=head->next;
        head->timer--;
        if(head->timer==0 && head->number_solders_inattack != head->number_solders_should_attack){
            produce_solder(head);
            head->timer=10;
        }
        for (int i = 0; i < head->number_solders_inattack; i++) {
            //check for crash
            crash(head);
            //printf(" %d  ",player[head->origin->playerID].velocity);
            head->solder[i].x += head->signx * player[head->origin->playerID].velocity;
            head->solder[i].y += head->shib* player[head->origin->playerID].velocity;

            if (head->solder[i].live == true) {
                if (abs((int) head->solder[i].x - head->Destination->x) < 6 &&
                    abs((int) head->solder[i].y - head->Destination->y) < 6) {
                    head->solder[i].live = false;
                    if (head->Destination->solders >= 0 && head->Destination->playerID != head->origin->playerID) {
                        if (head->Destination->solders == 0) {
                            head->Destination->playerID = head->playerID;
                            head->Destination->solders++;
                        }
                        else{
                            head->Destination->solders--;
                        }

                    } else {
                        head->Destination->solders++;
                    }
                }
            }
        }
        for(int k=0;k<head->number_solders_inattack;k++){
            head->flag=0;
            if(head->solder[k].live==true){
                head->flag=1;
                break;
            }
        }
        if (head->flag == 0) {
            delete(temp);
        }


    }
}
void add_at_tail(struct attack* head, struct area* source, struct area* destination){
    while(head->next != NULL) {
        head = head->next;
    }
    struct attack* attack = (struct attack*)(malloc(sizeof(struct attack)));
    attack-> origin= source;
    attack->Destination=destination;
    attack->number_solders_inattack=0;attack;
    //printf("%d",source->solders);
    attack->number_solders_should_attack= source->solders;
    double distance= sqrt((destination->y-source->y)*(destination->y-source->y)+(destination->x-source->x)*(destination->x-source->x));

    //velocity=2 pixcel /s
    attack->shib=(destination->y-source->y)/distance;
    attack->signx=(destination->x-source->x)/distance;
    attack->playerID=source->playerID;
    attack->timer=10;
    attack->flag=1;
    head->next=attack;
    attack->next = NULL;
}

void draw_attack(struct attack* head,SDL_Renderer *sdlRenderer,Uint32 color[]){
    while(head -> next != NULL){
        head=head->next;
        for(short i=0;i<head->number_solders_inattack;i++) {
            if (head->solder[i].live == true) {
                filledCircleColor(sdlRenderer, (Sint16)head->solder[i].x, (Sint16)head->solder[i].y, 5, color[head->playerID]);
            }
        }
    }
}

void draw_hexagon(SDL_Renderer *sdlRenderer,Sint16 x,Sint16 y, Uint32 color){
    Sint16  dy = 25,dx =(Sint16)((sqrt(3.0))*dy);
    Sint16 Xvects[6]={(Sint16)(x-dx), x, (Sint16)(x+dx), (Sint16)(x+dx), x, (Sint16)(x-dx)};
    Sint16 Yvects[6]={(Sint16)(y+dy), (Sint16)(y+2*dy), (Sint16)(y+dy), (Sint16)(y-dy),(Sint16)(y-2*dy),(Sint16)(y-dy)};
    filledPolygonColor (sdlRenderer,  Xvects, Yvects, 6, color);
}

void centers_of_hexagonals(int numarea, struct area area[numarea], Sint16 x, Sint16 y, int numplayer, struct player player[]){
    int z=numarea/numplayer;
    Sint16  dy = 25,dx = (Sint16) (1.0*sqrt(3.0)*dy);
    int dir[6][6]={{2*dx,0},
                   {-2*dx,0},
                   {dx,3*dy},
                   {dx,-3*dy},
                   {-1*dx,3*dy},
                   {-1*dx,-3*dy}};
    time_t t1;
    srand ( (unsigned) time (&t1));
    int a,produced_player=0;
    short invalid_count=0;
    short  invalid_dirs[6]={0,0,0,0,0,0};
    for (int i=0;i<numarea;){
        short flag=1;
        a= rand() % 6;
        if(invalid_dirs[a]) {
            flag = 0;
        }else {
            for (int j = 0; j < i; j++) {
                if ((x + dir[a][0] == area[j].x && y + dir[a][1] == area[j].y ) ||
                    x + dir[a][0] + dx > SCREEN_WIDTH || x + dir[a][0] - dx < 0 || y + dir[a][1] - 2 * dy < 0 ||
                    y + dir[a][1] + 2 * dy > SCREEN_HEIGHT){
                    flag = 0;
                    invalid_dirs[a]=1;
                    invalid_count++;
                    break;
                }
            }
            if(invalid_count==6){
                x+=dir[a][0];
                y+=dir[a][1];
                for(int n=0; n<6; n++){
                    invalid_dirs[n]=0;
                }
                invalid_count=0;
            }
        }
        if(flag){
            area[i].x =x+dir[a][0];
            area[i].y =y+dir[a][1];
            x+=dir[a][0];
            y+=dir[a][1];
            if(i%z==0){
                area[i].playerID=produced_player;
                area[i].solders=10;
                player[produced_player].centers[0][produced_player]=x;
                player[produced_player].centers[1][produced_player]=y;
                player[produced_player].num_center++;
                produced_player++;
            }else{
                area[i].playerID=numplayer;
                area[i].solders=20;
            }
            i++;
            for(int n=0; n<6; n++){
                invalid_dirs[n]=0;
            }
            invalid_count=0;
        }
    }

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

int len=0;
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

    //create texture for start page
    SDL_Texture *sdlTexture = getImageTexture(sdlRenderer, "Start.bmp");
    SDL_Rect texture_rect = {.x=0, .y=0, .w=SCREEN_WIDTH, .h=SCREEN_HEIGHT};
    SDL_bool start=SDL_FALSE;

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
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Uint32 colors[10]={0x7514A0EB,0x753C4CE7,0x75B6C673,0x6593570D,0x65AAE082 ,0x656B6A61,0x65BD69A5,0x65BD69A5};
    Uint32 colorfull[10]={0xff14A0EB,0xff3C4CE7,0xffB6C673,0xff93570D,0xffAAE082 ,0xff6B6A61,0xffBD69A5,0xffBD69A5};
    Uint32 potion_colors[4]={0xff3FF45B,0xffF43FAD,0xff0303FC,0xffF403FC};
    Sint16 x = (Sint16)(SCREEN_WIDTH/2), y = (Sint16)(SCREEN_HEIGHT/2);
    // initialize centers of hexagons
    short num_player = 3, num_area=90;
    struct area area[num_area];
    struct player player[num_player+1];
    centers_of_hexagonals(num_area,area,x,y,num_player, player);

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
    struct timeval te;
    SDL_bool first_click=SDL_FALSE;

    short timer=0;

    struct attack* head=(struct attack*)(malloc(sizeof(struct attack)));
    head ->next=NULL;
    short origin;

    struct potion* potion_head= malloc(sizeof(struct potion));
    potion_head->next=NULL;

    while (shallExit==SDL_FALSE) {

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
            printf("hi ");
            //create coordinate & kind of potions randomly
            random_potion(num_area,area,potion_head);
        }

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

        //draw number of barracks solders
        timer++;
        for(int i=0;i<num_area;i++){
            if(timer%player[area[i].playerID].produce_solders_timerate==0 &&area[i].playerID != num_player&&area[i].solders<99){
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
                                if ( area[i].playerID != num_player) {
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