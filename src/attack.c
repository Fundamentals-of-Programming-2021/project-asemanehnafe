#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <stdbool.h>
#include "map.h"
#include "attack.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void produce_solder(struct attack* head){
    if(head-> origin->solders >= 0 ){
        head->solder[head->number_solders_inattack].x=head->origin->x;
        head->solder[head->number_solders_inattack].y=head->origin->y;
        head->solder[head->number_solders_inattack].live=true;
        head -> number_solders_inattack++;
    }
}
void delete(struct attack* head){
    if(head->next->next==NULL){
        //printf("one %hd",head->next->playerID);
        if(head->next->origin->playerID!=head->next->Destination->playerID){
            head->next->origin->attacker=false;
            head->next->Destination->defender=false;
        }
        free(head->next->solder);
        free(head->next);
        head->next=NULL;
    }else{
        //printf("secound %hd",head->next->playerID);
        struct attack* temp=head->next;
        if(temp->origin->playerID!=temp->Destination->playerID){
            temp->origin->attacker=false;
            temp->Destination->defender=false;
        }
        head->next=head->next->next;
        free(temp->solder);
        free(temp);
    }
}
void crash( struct attack* head){

    for(struct attack* temp=head->next; temp!=NULL; temp=temp->next){
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

        if(head->origin->playerID!=head->Destination->playerID){
            head->origin->attacker=true;
            head->Destination->defender=true;
        }
        //check for
        crash(head);
        for (int i = 0; i < head->number_solders_inattack; i++) {
            //printf(" %d  ",player[head->origin->playerID].velocity);
            if(player[head->playerID].move){
                head->solder[i].x += head->signx * player[head->origin->playerID].velocity;
                head->solder[i].y += head->shib* player[head->origin->playerID].velocity;
            }

            if (head->solder[i].live == true) {
                if (abs((int) head->solder[i].x - head->Destination->x) < 6 &&
                    abs((int) head->solder[i].y - head->Destination->y) < 6) {
                    head->solder[i].live = false;
                    if (head->Destination->solders >= 0 && head->Destination->playerID != head->origin->playerID) {
                        if (head->Destination->solders == 0) {

                            player[head->origin->playerID].num_center++;
                            player[head->Destination->playerID].num_center--;
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
        short flag=1;
        for(int k=0;k<head->number_solders_inattack;k++){
            flag=0;
            if(head->solder[k].live==true){
                flag=1;
                break;
            }
        }
        if (flag == 0) {
            head->Destination->defender=false;
            head->origin->attacker=false;
            delete(temp);
            head=temp;
        }


    }
}
void number_of_solders(struct attack *head,struct attack* attack){
    while(head->next!=NULL){
        head=head->next;
        if(head->origin==attack->origin){
            attack->number_solders_should_attack-=head->number_solders_should_attack;
        }
    }
}
void add_at_tail(struct attack* head, struct area* source, struct area* destination){
    struct attack *temp=head;
    while(head->next != NULL) {
        head = head->next;
    }
    struct attack* attack = (struct attack*)(malloc(sizeof(struct attack)));

    attack->solder=(struct solder*)malloc(sizeof (struct solder) * source->solders);

    attack-> origin= source;
    attack->Destination=destination;
    attack->number_solders_inattack=0;
    attack->number_solders_should_attack= source->solders;

    attack->origin->solders=0;
    double distance= sqrt((destination->y-source->y)*(destination->y-source->y)+(destination->x-source->x)*(destination->x-source->x));

    //velocity=2 pixcel /s
    attack->shib=(destination->y-source->y)/distance;
    attack->signx=(destination->x-source->x)/distance;

    attack->playerID=source->playerID;
    attack->timer=10;
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
void computer_movement(short num_player,short num_area,struct area area[],struct player player[], struct attack* head) {
    int count=0;
    for(int i=0;i<num_area;i++){
        if(area[i].playerID!= 0 && area[i].playerID != num_player){
            count++;
        }
    }

    if(count && player[0].can_attack_to==true){
        for(int i=0; i<num_area;i++){
            if(player[0].num_center>1 && area[i] .playerID!=0 && area[i].playerID != num_player && area[i].attacker==false && area [i].defender==false){
                for(int j=0;j<num_area;j++){
                    if(area[j].playerID==0 && area[i].solders>area[j].solders&& area[j].defender==false){
                        add_at_tail(head, &area[i], &area[j]);
                        return;
                    }
                }
            }
        }
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
        if(player[0].num_center==1 && player[num_player].num_center==0){
            for(int i=0;i<num_area;i++){
                if(area[i].playerID==0 && area[i].defender == false){
                    for(int j=0;j<num_area;j++) {
                        if (area[j].playerID != 0 && area[j].playerID != num_player && area[j].solders>area[i].solders ){
                            add_at_tail(head, &area[j], &area[i]);
                            return;
                        }
                    }
                }
            }
        }
        if(player[0].num_center>1){
            for(int i=0;i<num_area;i++){
                if(area[i].playerID!=0 && area[i].playerID != num_player&& area[i].attacker==false && area[i].defender==false){
                    for(int j=0;j<num_area;j++){
                        if(area[j].playerID==0 && area[j].defender==false && area[i].solders>area[j].solders){
                            add_at_tail(head, &area[i], &area[j]);
                            return;
                        }
                    }
                }
            }
        }
    }
    if(count) {
        for(int i=0; i<num_area;i++){
            if(area[i] .playerID!=0 && area[i].playerID != num_player && area[i].attacker==false && area[i].defender==false ){


                for(int j=0;j<num_area;j++){
                    if(area[j].playerID==num_player && area[i].solders>area[j].solders && area[j].defender==false){
                        add_at_tail(head, &area[i], &area[j]);
                        return;
                    }
                }
            }
        }
    }


}