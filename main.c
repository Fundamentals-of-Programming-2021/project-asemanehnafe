#include <SDL.h>
#include <SDL2_gfxPrimitives.h>

#ifdef main
#undef main
#endif
#include <stdio.h>
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
};
struct area{
    int x;
    int y;
    short playerID;
    short solders;
};
struct solder{
    bool live;
    double x;
    double y;
};
struct attack{
    struct area* origin;
    struct area* Destination;
    short number_solders_inattack;
    short number_solders_should_attack;
    struct solder solder[150];
    struct attack* next;
    double shib;
    double signx;
    short timer;
};
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
void attack(struct attack* head){
    while(head->next != NULL) {
        struct attack* temp=head;
        head=head->next;
        head->timer--;
        for (int i = 0; i < head->number_solders_inattack; i++) {
            if (head->solder[i].live == true) {
                head->solder[i].x += head->signx;
                head->solder[i].y += head->shib;
                if (abs((int) head->solder[i].x - head->Destination->x) < 6 &&
                    abs((int) head->solder[i].y - head->Destination->y) < 6) {
                    head->solder[i].live = false;
                    if (head->Destination->solders >= 0 && head->Destination->playerID != head->origin->playerID) {
                        head->Destination->solders--;
                        if (head->Destination->solders == 0) {

                            head->Destination->playerID = head->origin->playerID;
                        }
                    } else {
                        head->Destination->solders++;
                    }
                }
            }
        }
        if (head->number_solders_inattack==head->number_solders_should_attack &&
            head->solder[head->number_solders_inattack-1].live == false) {
            delete(temp);
        }
        if(head->timer==0 && head->number_solders_inattack != head->number_solders_should_attack ){
            produce_solder(head);
            head->timer=10;
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
    attack->number_solders_inattack=0;
    attack->number_solders_should_attack=source->solders;
    double distance= sqrt((destination->y-source->y)*(destination->y-source->y)+(destination->x-source->x)*(destination->x-source->x));

    //velocity=1 pixcel /s
    attack->shib=1.0*(destination->y-source->y)/distance;
    attack->signx=1.0*(destination->x-source->x)/distance;


    attack->timer=10;
    head->next=attack;
    attack->next = NULL;
}
void draw_attack(struct attack* head,SDL_Renderer *sdlRenderer,Uint32 color[]){
    while(head -> next != NULL){
        head=head->next;
        for(short i=0;i<head->number_solders_inattack;i++) {
            if (head->solder[i].live == true) {
                filledCircleColor(sdlRenderer, (Sint16)head->solder[i].x, (Sint16)head->solder[i].y, 5, color[head->origin->playerID]);
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
                player[produced_player].centers[0][1]=x;
                player[produced_player].centers[1][1]=y;
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
    }
    player[num_player].color=0x45F4F3F0;
    player[num_player].live=SDL_TRUE;
    player[num_player].point=0;

    int arr=4;
    short timer=0;
    struct timeval te;
    SDL_bool first_click=SDL_FALSE;

    struct attack* head=(struct attack*)(malloc(sizeof(struct attack)));
    head ->next=NULL;
    short origin;

    while (shallExit==SDL_FALSE) {

        SDL_SetRenderDrawColor(sdlRenderer, 0x17, 0x20, 0x2A, 0xff);
        SDL_RenderClear(sdlRenderer);

        //check for attacks
        if(head ->next != NULL){
            //gettimeofday(&te, NULL);
            //long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000;
            attack(head);
        }

        // draw area and barracks
        for(int i=0;i<num_area;i++) {
            draw_hexagon(sdlRenderer, area[i].x, area[i].y, player[area[i].playerID].color);
            filledCircleColor(sdlRenderer,  area[i].x, area[i].y, 12, player[area[i].playerID].color);
        }

        //draw number of barracks solders
        timer++;
        if(timer==100)
            timer=0;
        for(int i=0;i<num_area;i++){
            if(timer==0 &&area[i].playerID != num_player&&area[i].solders<99){
                area[i].solders++;
            }
            char buffer[50];
            sprintf(buffer,"%d",area[i].solders);
                stringRGBA(sdlRenderer, area[i].x-4, area[i].y-3,buffer,0,0,0,255);
        }

        //draw attacks
        if(head -> next != NULL){
            draw_attack(head,sdlRenderer,colorfull);
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
                           if (first_click) {
                               //an attack is on
                               first_click = SDL_FALSE;
                               printf("ATACK");
                               add_at_tail(head, &area[origin], &area[i]);
                           } else {
                               if (area[i].playerID != num_player) {
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
    SDL_DestroyWindow(sdlWindow);
    SDL_Quit();
    return 0;
}