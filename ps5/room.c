#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "room.h"

struct room* create_room(char *name, char *description){
    if(name == NULL || description == NULL || name[0] == '\0' || description[0] == '\0'){
        printf("name or desctiption are absent, NULL\n");
        return NULL;
    }
    struct room *newRoom = malloc(sizeof(struct room));
    newRoom->name = malloc(strlen(name) + 1);
    strcpy(newRoom->name, name);       // Copy the string to ensure it's not overwritten
    newRoom->description = malloc(strlen(description) + 1);
    strcpy(newRoom->description, description);       // Copy the string to ensure it's not overwritten

    newRoom->north = NULL;
    newRoom->south = NULL;
    newRoom->east = NULL;
    newRoom->west = NULL;
    newRoom->items = NULL;
    return newRoom;
}

struct room* destroy_room(struct room* room) {
    if(room == NULL) return NULL;
    
    if(room->name != NULL) free(room->name);         
    if(room->description != NULL) free(room->description);  

    free(room);              
    return NULL;            
}

void set_exits_from_room(struct room *room, struct room *north, struct room *south, struct room *east, struct room *west){
    if(room == NULL){
        printf("room is NULL\n");
        return;
    }
    room->north = north;
    room->south = south;
    room->east = east;
    room->west = west;
}

void show_room(const struct room* room){
    if(room == NULL){
        printf("Rooom is null\n");
        return;
    }
    printf("You are in %s.\n%s\n", room->name, room->description);
    if(room->north != NULL){
        printf("There is a %s on the north.\n", room->north->name);
    }
    if(room->south != NULL){
        printf("There is a %s on the south.\n", room->south->name);
    }
    if(room->east != NULL){
        printf("There is a %s on the east.\n", room->east->name);
    }
    if(room->west != NULL){
        printf("There is a %s on the west.\n", room->west->name);
    }
    if(room->items != NULL){
        printf("There are these items in the room %s.", room->items->item->name);
    }
}
//ПРОВЕРКА КЕЙС СЕНСИТИВ
void delete_item_from_room(struct room* room, struct item* item){
    if(room == NULL || item == NULL){
        return;
    }
    struct container* current = room->items;
    struct container* prev = NULL;

    while (current != NULL) {
        if (strcmp(current->item->name, item->name) == 0) {
            if (prev == NULL) {
                room->items = current->next;
            } else {
                prev->next = current->next;
            }

            // free(current->item->name);
            // free(current->item->description);
            // free(current->item);
            // free(current); // Free the container itself
            return; // Exit after removing the item
        }
        prev = current;
        current = current->next;
    }
}

void add_item_to_room(struct room* room, struct item* item){
    if(room == NULL || item == NULL){
        return;
    }

    if(room->items == NULL){
        room->items = malloc(sizeof(struct container)); // Allocate memory for the container
        if(room->items == NULL) return; // Always check if malloc succeeded
        room->items->item = item;
        room->items->type = ITEM; // Assuming 'type' needs to be set as well
        room->items->next = NULL;
    } else {
        // Logic to add the item to the end of the existing list
        struct container *current = room->items;
        while(current->next != NULL) {
            current = current->next;
        }
        current->next = malloc(sizeof(struct container));
        if(current->next == NULL) return; // Check if malloc succeeded
        current->next->item = item;
        current->next->type = ITEM;
        current->next->next = NULL;
    }
}

void strtolowers(char *data){
    if(data == NULL) return;
    while(*data){
        if(*data >= 'A' && *data <= 'Z'){
            *data = *data + 32;
        }
        data++;
    }
}

struct item* get_item_from_room(const struct room* room, const char* name){
    if(room == NULL || name == NULL || name[0] == '\0'){
        printf("Input parametrs are NULL\n");
        return NULL;
    }
    struct container* findItem = room->items;
    char* nameMod = malloc(strlen(name) + 1);
    strcpy(nameMod, name);
    strtolowers(nameMod);
    while(findItem->next != NULL){
        char* currentName = malloc(strlen(findItem->item->name) + 1);
        strcpy(currentName, findItem->item->name);
        strtolowers(currentName);
        if(strcmp(nameMod, currentName) == 0){
            free(nameMod);
            free(currentName);
            return findItem->item;
        }
        findItem = findItem->next;
        free(currentName);
    }
    char* currentName = malloc(strlen(findItem->item->name) + 1);
    strcpy(currentName, findItem->item->name);
    strtolowers(currentName);
    if(strcmp(nameMod, currentName) == 0){
        free(nameMod);
        free(currentName);
        return findItem->item;
    }
    free(nameMod);
    free(currentName);
    return NULL;
}