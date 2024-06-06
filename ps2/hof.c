#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hof.h"

int load(struct player list[]){
    FILE *file;
    if((file = fopen("score", "r")) == NULL){
        return -1;
    }
    char name[30];
    int score;
    int count = 0;
    while (fscanf(file, "%s %d", name, &score) == 2 && count < 10) {
        strcpy(list[count].name, name);
        list[count].score = score;
        count++;
    }
    struct player buffer;
    for(int i = 0; i < count - 1; i++){
        for(int j = 0; j < count - 1; j++){
            if(list[j].score < list[j+1].score){
                strcpy(buffer.name, list[j].name);
                strcpy(list[j].name, list[j+1].name);
                strcpy(list[j+1].name, buffer.name);
                buffer.score = list[j].score;
                list[j].score = list[j+1].score;
                list[j+1].score = buffer.score;
            }
        }
    }
    fclose(file);
    return count;
}

bool save(const struct player list[], const int size){
    FILE *file;
    if((file = fopen("score", "w")) == NULL){
        return -1;
    }
    struct player *list1 = calloc(10 , sizeof(struct player));
    memcpy(list1, list, 10 * sizeof(struct player));
    struct player buffer;

    for(int i = 0; i < size - 1; i++){
        for(int j = 0; j < size - 1; j++){
            if(list1[j].score < list1[j+1].score){
                strcpy(buffer.name, list1[j].name);
                strcpy(list1[j].name, list1[j+1].name);
                strcpy(list1[j+1].name, buffer.name);
                buffer.score = list1[j].score;
                list1[j].score = list1[j+1].score;
                list1[j+1].score = buffer.score;
            }
        }
    }

    for (int i = 0; i < size; i++) {
        fprintf(file, "%s %d\n", list1[i].name, list1[i].score);
    }
    fclose(file);
    free(list1);
    return true;
}

bool add_player(struct player list[], int* size, const struct player player){
    FILE *file;
    if((file = fopen("score", "w")) == NULL){
        return -1;
    }
    int flag = 1;
    for(int i = 0; i < *size; i++){
        if(list[i].score <= player.score){
            int l = 0;
            if(*size == 10){
                l = 9;
            }else{
                l = *size;
            }
            for(int j = l; j > i; j--){
                strcpy(list[j].name, list[j-1].name);
                list[j].score = list[j-1].score;
            }
            strcpy(list[i].name, player.name);
            list[i].score = player.score;
            flag = 0;
            break;
        }
    }
    if(*size < 10 && flag == 1){
        strcpy(list[*size].name, player.name);
        list[*size].score = player.score;
        flag = 0;
    }
    if(*size < 10){
        *size = *size + 1;
    }
    for (int i = 0; i < *size; i++) {
        fprintf(file, "%s %d\n", list[i].name, list[i].score);
    }
    fclose(file);
    if(flag == 0){
        return true;
    }else{
        return false;
    }
}


// int main(){
//     struct player list[10];
// int size = 0;
// struct player player = {
//     .name = "John",
//     .score = 100
// };
//     bool result = add_player(list, &size, player);
//     printf("%d, %d\n", result, size);
// }