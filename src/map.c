#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <stdbool.h>
#include "map.h"

void draw_hexagon(SDL_Renderer *sdlRenderer,Sint16 x,Sint16 y, Uint32 color){
    Sint16  dy = 25,dx =(Sint16)((sqrt(3.0))*dy);
    Sint16 Xvects[6]={(Sint16)(x-dx), x, (Sint16)(x+dx), (Sint16)(x+dx), x, (Sint16)(x-dx)};
    Sint16 Yvects[6]={(Sint16)(y+dy), (Sint16)(y+2*dy), (Sint16)(y+dy), (Sint16)(y-dy),(Sint16)(y-2*dy),(Sint16)(y-dy)};
    filledPolygonColor (sdlRenderer,  Xvects, Yvects, 6, color);
}
void centers_of_hexagonals(int numarea, struct area area[numarea], Sint16 x, Sint16 y, int numplayer, struct player player[],int seed){
    int z=numarea/numplayer;
    Sint16  dy = 25,dx = (Sint16) (1.0*sqrt(3.0)*dy);
    int dir[6][6]={{2*dx,0},
                   {-2*dx,0},
                   {dx,3*dy},
                   {dx,-3*dy},
                   {-1*dx,3*dy},
                   {-1*dx,-3*dy}};
    srand ( seed);
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
                if ((abs(x + dir[a][0] - area[j].x )<22&& abs( y + dir[a][1] - area[j].y) <22) ||
                    x + dir[a][0] + dx > SCREEN_WIDTH*2/3 || x + dir[a][0] - dx < 0 || y + dir[a][1] - 2 * dy < 0 ||
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
                area[i].attacker=false;
                area[i].defender=false;
                player[produced_player].num_center++;

                player[produced_player].centers[0][produced_player]=x;
                player[produced_player].centers[1][produced_player]=y;
                player[produced_player].num_center=1;
                player[produced_player].move=true;
                produced_player++;
            }else{
                area[i].playerID=numplayer;
                area[i].solders=20;
                area[i].attacker=false;
                area[i].defender=false;
                player[numplayer].num_center++;
                player[produced_player].move=false;
            }
            i++;
            for(int n=0; n<6; n++){
                invalid_dirs[n]=0;
            }
            invalid_count=0;
        }
    }
}
/*
 * for memory check
void centers_of_hexagonals(int numarea, struct area area[numarea], Sint16 x, Sint16 y, int numplayer, struct player player[],int seed){
    int z=numarea/numplayer;
    Sint16  dy = 25,dx = (Sint16) (1.0*sqrt(3.0)*dy);
    int dir[6][6]={{2*dx,0},
                   {-2*dx,0},
                   {dx,3*dy},
                   {dx,-3*dy},
                   {-1*dx,3*dy},
                   {-1*dx,-3*dy}};
    srand ( seed);
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
                    x + dir[a][0] + dx > SCREEN_WIDTH*2/3 || x + dir[a][0] - dx < 0 || y + dir[a][1] - 2 * dy < 0 ||
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
            if(i==0){
                area[i].playerID=1;
                area[i].solders=10;
                area[i].attacker=false;
                area[i].defender=false;
                player[produced_player].num_center++;

                player[produced_player].centers[0][produced_player]=x;
                player[produced_player].centers[1][produced_player]=y;
                player[produced_player].num_center=1;
            }else{
                area[i].playerID=0;
                area[i].solders=10;
                area[i].attacker=false;
                area[i].defender=false;
                player[numplayer].num_center=9;
            }
            i++;
            for(int n=0; n<6; n++){
                invalid_dirs[n]=0;
            }
            invalid_count=0;
        }
    }
}*/
