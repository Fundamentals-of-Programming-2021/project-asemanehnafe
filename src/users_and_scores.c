//
// Created by ASUS on 2/9/2022.
//
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

int check_user(char text[100]){
    FILE* ptr = fopen("users.txt", "a+");
    if (ptr == NULL) {
        printf("no such file.");
        return -1;
    }
    /* Assuming that user.txt has content
    in below format
    NAME
    point
    asemane
    12
    amirali
    25
    ali
    65  */
    char buf[100];
    int last_point;
    int new=1;
    while (fscanf(ptr, "%[^\n]s",buf)== 1){
        fscanf(ptr," %d ", &last_point);
        if(strcmp(text,buf)==0){
            fclose(ptr);
            return last_point;
            new=0;
        }
    }
    if(new){
        fputs(text, ptr);
        fprintf(ptr,"\n%d\n",0);
    }
    fclose(ptr);
    return 0;
}
void sort_users(int *high_score,char rank[100]){
    FILE *file = fopen("users.txt","r");
    char buf[100];
    int max_score=0;
    int point;
    while (fscanf(file, "%[^\n]s",buf)== 1){
        fscanf(file," %d ", &point);
        if(max_score<=point){
            strcpy(rank,buf);
            *high_score=point;
            max_score=point;
        }
    }
    fclose(file);
}
void replace_Score(int new_point,char user[100]){
    FILE *original = fopen("users.txt","r");
    FILE *new = fopen("userstemp.txt","w");
    char buf[100];
    int last_point;
    while (fscanf(original, "%[^\n]s",buf)== 1){
        fscanf(original," %d ", &last_point);
        if(strcmp(user,buf)==0){
            last_point=new_point;
        }
        fprintf(new,"%s\n%d\n",buf,last_point);
    }
    fclose(new);
    fclose(original);
    remove("users.txt");
    rename("userstemp.txt","users.txt");
}