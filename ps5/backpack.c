#include "backpack.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct backpack* create_backpack(const int capacity){
    if(capacity < 0){
        printf("Capacity is <0\n");
        return NULL;
    } 
    struct backpack* newBackpack = malloc(sizeof(struct backpack));
    newBackpack->capacity = capacity;
    newBackpack->items = NULL;
    newBackpack->size = 0;
    return newBackpack;
}

struct backpack* destroy_backpack(struct backpack* backpack) {
    if (backpack == NULL) return NULL;

    struct container* head = backpack->items;
    struct container* next;

    while (head != NULL) {
        next = head->next;
        if (head->item != NULL) {
            free(head->item->name);   
            free(head->item->description); 
            free(head->item);
        }
        free(head); 
        head = next;
    }
    free(backpack);
    return NULL;
}

bool add_item_to_backpack(struct backpack* backpack, struct item* item){
    if (backpack == NULL || item == NULL || backpack->size >= backpack->capacity) {
        printf("NULL\n");
        return false;
    }

    if (!(item->properties & MOVABLE)) {
        printf("Item is not movable.\n");
        return false;
    }

    struct container* newCont = malloc(sizeof(struct container));

    newCont->item = item;
    newCont->next = NULL;

    if (backpack->items == NULL) {
        backpack->items = newCont;
    } else {
        struct container* head = backpack->items;
        while (head->next != NULL) {
            head = head->next;
        }
        head->next = newCont;
    }

    backpack->size++;
    return true;
}

void strtolow(char *data){
    if(data == NULL) return;
    while(*data){
        if(*data >= 'A' && *data <= 'Z'){
            *data = *data + 32;
        }
        data++;
    }
}

struct item* get_item_from_backpack(const struct backpack* backpack, char* name){
    if(backpack == NULL || name == NULL || name[0] == '\0') return NULL;

    //struct item* findItem = malloc(sizeof(struct item));

    struct container* head = backpack->items;
    char* nameMod = malloc(strlen(name) + 1);
    strcpy(nameMod, name);
    strtolow(nameMod);

    if(backpack->items->item != NULL){
        char* curName = malloc(strlen(backpack->items->item->name) + 1);
        strcpy(curName, backpack->items->item->name);
        strtolow(curName);
        if(strcmp(nameMod, curName) == 0){
            free(nameMod);
            free(curName);
            return backpack->items->item;
        }
        free(curName);
    }
    
    while(head->next != NULL){
        char* curName = malloc(strlen(head->item->name) + 1);
        strcpy(curName, head->item->name);
        strtolow(curName);
        if(strcmp(nameMod, curName) == 0){
            free(nameMod);
            free(curName);
            return head->item;
        }
        head = head->next;
    }
    char* curName = malloc(strlen(head->item->name) + 1);
    strcpy(curName, head->item->name);
    strtolow(curName);
    if(strcmp(nameMod, curName) == 0){
        free(nameMod);
        free(curName);
        return head->item;
    }
    free(curName);
    free(nameMod);
    return NULL; 
}

void delete_item_from_backpack(struct backpack* backpack, struct item* item) {
    if (backpack == NULL || item == NULL) return;

    struct container* current = backpack->items;
    struct container* prev = NULL;

    while(current != NULL){
        if (strcmp(current->item->name, item->name) == 0) {
            if(prev == NULL){
                backpack->items = current->next;
            }else{
                prev->next = current->next;
            }

            free(current);
            backpack->size--;
            return;
        }
        prev = current;
        current = current->next;
    }
}