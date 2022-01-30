#include <SDL.h>
#include <SDL2_gfxPrimitives.h>

#ifdef main
#undef main
#endif

#include <stdio.h>
//#include <stdbool.h>
#include <math.h>
#include <time.h>

const int SCREEN_WIDTH = 1250;
const int SCREEN_HEIGHT = 600;
const int FPS = 60;

void draw_hexagon(SDL_Renderer *sdlRenderer,Sint16 x,Sint16 y){

    // draw box using gfx_primitives
    Uint32 color = 0xffffff00;
    Sint16  dy = 25,dx =(Sint16)((sqrt(3.0))*dy);
    boxColor(sdlRenderer, (Sint16)(x-dx), (Sint16)(y-dy), (Sint16)(x+dx), (Sint16)(y+dy), color);

    // draw triangle using gfx_primitives
    filledTrigonColor	(	sdlRenderer ,(Sint16)(x-dx),(Sint16)(y-dy),(Sint16)(x+dx),(Sint16)(y-dy),x, (Sint16)(y-2*dy), color);
    filledTrigonColor	(	sdlRenderer ,(Sint16)(x-dx),(Sint16)(y+dy),(Sint16)(x+dx),(Sint16)(y+dy),x, (Sint16)(y+2*dy), color);

}

void centers_of_hexagonals(int area, Sint16 centers[area][2], Sint16 x, Sint16 y){

    Sint16  dy = 25,dx = (Sint16)(sqrt(3.0)*dy);
    double dir[6][6]={{2*dx,0},
                     {-2*dx,0},
                     {dx,3*dy},
                     {dx,-3*dy},
                     {-1*dx,3*dy},
                     {-1*dx,-3*dy}};
    time_t t1;
    srand ( (unsigned) time (&t1));
    int a;
    short invalid_count=0;
    short  invalid_dirs[6]={0,0,0,0,0,0};
    for (int i=0;i<area;){
        short flag=1;
        a= rand() % 6;
        if(invalid_dirs[a]) {
            flag = 0;
        }else {
            for (int j = 0; j < i; j++) {
                if ((x + dir[a][0] == centers[j][0] && y + dir[a][1] == centers[j][1]) ||
                    x + dir[a][0] + dx > SCREEN_WIDTH || x + dir[a][0] - dx < 0 || y + dir[a][1] - 2 * dy < 0 ||
                    y + dir[a][1] + 2 * dy > SCREEN_HEIGHT) {
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
            centers[i][0]=(Sint16)(x+dir[a][0]);
            centers[i][1]=y+dir[a][1];
            x+=dir[a][0];
            y+=dir[a][1];
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
    char *composition= malloc(sizeof (char)*100);
    Sint32 cursor;
    Sint32 selection_len;


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
                    /* Add new text onto the end of our text */
                    text[len]=*sdlEvent.text.text;
                    len ++;
                    break;

                case SDL_TEXTEDITING:
                    /*
                    Update the composition text.
                    Update the cursor position.
                    Update the selection length (if any).
                    */
                    composition = sdlEvent.edit.text;
                    cursor = sdlEvent.edit.start;
                    selection_len = sdlEvent.edit.length;
                    break;
            }
        }
        stringRGBA(sdlRenderer,500,300,text,255,165,0,255);
        SDL_RenderPresent(sdlRenderer);
        SDL_Delay(1000 / FPS);
    }

    Sint16 x = (Sint16)(SCREEN_WIDTH/2), y = (Sint16)(SCREEN_HEIGHT/2);

    // initialize centers of hexagons
    int area=9;
    Sint16 centers[area][2];
    centers_of_hexagonals( area, centers,  x, y);
    int arr=4,timer=0;

    while (shallExit==SDL_FALSE) {

        SDL_SetRenderDrawColor(sdlRenderer, 0xff, 0xff, 0xff, 0xff);
        SDL_RenderClear(sdlRenderer);

        // draw area and barracks
        for(int i=0;i<area;i++) {
            draw_hexagon(sdlRenderer,centers[i][0], centers[i][1]);
            Uint32 color2 = 0xff0000ff;
            //Sint16 x = 480, y = 240, r = 100;
            filledCircleColor(sdlRenderer, centers[i][0], centers[i][1], 12,color2);
        }

        //draw numbeer of barracks solders
        timer++;
        if(timer==100)
            timer=0;
        if(timer==0 && arr<99){
            arr++;
        }
        char buffer[50];
        sprintf(buffer,"%d",arr);
        for(int i=0;i<9;i++){
            stringRGBA(sdlRenderer,centers[i][0]-4,centers[i][1]-3,buffer,0,0,0,255);
        }

        // listen for key events
        SDL_Event sdlEvent;
        while (SDL_PollEvent(&sdlEvent)) {
            switch (sdlEvent.type) {
                case SDL_QUIT:
                    shallExit = SDL_TRUE;
                    break;

            }
        }


        SDL_RenderPresent(sdlRenderer);
        SDL_Delay(1000 / FPS);

    }
    SDL_DestroyWindow(sdlWindow);
    SDL_Quit();
    return 0;
}