#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "container.h"
#include "room.h"

struct container* create_container(struct container* first, enum container_type type, void* entry){
    if(entry == NULL) return NULL;
    
    if(first != NULL && first->type != type){
        printf("Incorrect type\n");
        return NULL;
    }

    struct container* node = calloc(1, sizeof(struct container));
    
    node->type = type;
    switch(type){
        case ROOM:
            node->room = entry;
            break;
        case ITEM:
            node->item = entry;
            break;
        case COMMAND:
            node->command = entry;
            break;
        case TEXT:
            node->text = malloc(strlen((char*)entry) + 1);
            strcpy(node->text, (char*)entry);
            break;
        default:
            free(node);
            return NULL;
    }

    if(first == NULL){
        //first = node;
        return node;
    }else{
        struct container* ptr = first;
        while(ptr->next != NULL){
            ptr = ptr->next;
        }
        ptr->next = node;
    }
    return node;
}


struct container* destroy_containers(struct container* first) {
    if (first == NULL) {
        fprintf(stderr, "Container is NULL\n");
        return NULL;
    }
    struct container *ptr = first;
    while (ptr != NULL) {
        struct container *tmp = ptr;
        ptr = ptr->next;
        
        switch (tmp->type) {
            case ROOM:
                if(tmp->room != NULL) destroy_room(tmp->room);
                break;
            case ITEM:
                destroy_item(tmp->item);
                break;
            case COMMAND:
                destroy_command(tmp->command);
                break;
            case TEXT:
                free(tmp->text);
                break;
            default:
                fprintf(stderr, "Unknown container type\n");
                break;
        }
        free(tmp);
    }   
    return NULL;
}

void strtolower(char *data){
    if(data == NULL) return;
    while(*data){
        if(*data >= 'A' && *data <= 'Z'){
            *data = *data + 32;
        }
        data++;
    }
}

void* search_room(struct container *first, const char *name){
    if(first == NULL || name == NULL) return NULL;
    struct container *ptr = first;
    char *nameMod = malloc(strlen(name) + 1);
    strcpy(nameMod, name);
    strtolower(nameMod);

    while(ptr->next != NULL){
        char *existInfo = malloc(strlen(ptr->room->name) + 1);
        strcpy(existInfo, ptr->room->name);
        strtolower(existInfo);

        if(strcmp(nameMod, existInfo) == 0){
            free(existInfo);
            free(nameMod);
            return ptr->room;
        }
        ptr = ptr->next;
        free(existInfo);
    }

    char *existInfo = malloc(strlen(ptr->room->name) + 1);
    strcpy(existInfo, ptr->room->name);    
    strtolower(existInfo);
    if(strcmp(nameMod, existInfo) == 0){
        free(existInfo);
        free(nameMod);
        return ptr->room;
    }
    free(existInfo);
    free(nameMod);
    return NULL;
}

void* search_item(struct container *first, const char *name){
    if(first == NULL || name == NULL) return NULL;
    struct container *ptr = first;
    char *nameMod = malloc(strlen(name) + 1);
    strcpy(nameMod, name);
    strtolower(nameMod);

    while(ptr->next != NULL){
        char *existInfo = malloc(strlen(ptr->item->name) + 1);
        strcpy(existInfo, ptr->item->name);
        strtolower(existInfo);
        if(strcmp(existInfo, nameMod) == 0){
            free(existInfo);
            free(nameMod);
            return ptr->item;
        }
        ptr = ptr->next;
        free(existInfo);
    }

    char *existInfo = malloc(strlen(ptr->item->name) + 1);
    strcpy(existInfo, ptr->item->name);

    strtolower(existInfo);
    if(strcmp(existInfo, nameMod) == 0){
        free(existInfo);
        free(nameMod);
        return ptr->item;
    }
    free(existInfo);
    free(nameMod);
    return NULL;
}

void* search_command(struct container *first, const char *name){
    if(first == NULL || name == NULL) return NULL;
    struct container *ptr = first;
    char *nameMod = malloc(strlen(name) + 1);
    strcpy(nameMod, name);
    strtolower(nameMod);

    while(ptr->next != NULL){
        char *existInfo = malloc(strlen(ptr->command->name) + 1);
        strcpy(existInfo, ptr->command->name);
        strtolower(existInfo);
        if(strcmp(existInfo, nameMod) == 0){
            free(existInfo);
            free(nameMod);
            return ptr->command;
        }
        ptr = ptr->next;
        free(existInfo);
    }

    char *existInfo = malloc(strlen(ptr->command->name) + 1);
    strcpy(existInfo, ptr->command->name);
    strtolower(existInfo);
    if(strcmp(existInfo, nameMod) == 0){
        free(existInfo);
        free(nameMod);
        return ptr->command;
    }
    free(existInfo);
    free(nameMod);
    return NULL;
}

void* search_text(struct container *first, const char *name){
    
    if(first == NULL || name == NULL) return NULL;
    struct container *ptr = first;
    char *nameMod = malloc(strlen(name) + 1);
    strcpy(nameMod, name);
    strtolower(nameMod);

    while(ptr->next != NULL){
        char *existInfo = malloc(strlen(first->text) + 1);
        strcpy(existInfo, ptr->text);
        strtolower(existInfo);
        if(strcmp(existInfo, nameMod) == 0){
            free(existInfo);
            free(nameMod);
            return ptr->text;
        }
        ptr = ptr->next;
        free(existInfo);
    }

    char *existInfo = malloc(strlen(first->text) + 1);
    strcpy(existInfo, ptr->text);
    strtolower(existInfo);
    if(strcmp(existInfo, nameMod) == 0){
        free(existInfo);
        free(nameMod);
        return ptr->text;
    }
    free(existInfo);
    free(nameMod);
    return NULL;
}

void* get_from_container_by_name(struct container *first, const char *name){
    if(first == NULL || name == NULL){
        fprintf(stderr, "container or name is NULL\n");
        return NULL;
    }
    if(first->type == ROOM){
        return search_room(first, name);
    }else if(first->type == ITEM){
        return search_item(first, name);
    }else if(first->type == COMMAND){
        return search_command(first, name);
    }else if(first->type == TEXT){
        return search_text(first, name);
    }
    return NULL;
}

struct container* remove_container(struct container *first, void *entry) {
    if (first == NULL) {
        printf("NULL returned\n");
        return NULL;
    }
    if (entry == NULL) {
        printf("entry NULL\n");
        return first;
    }
    
    struct container *ptr = first;
    struct container *prev = NULL;

    while (ptr != NULL) {
        if (ptr->room == entry || ptr->command == entry || ptr->item == entry || ptr->text == entry) {
            if (prev == NULL) { 
                first = ptr->next;
            } else {
                prev->next = ptr->next;
            }
            free(ptr); 
            break;
        }
        prev = ptr;
        ptr = ptr->next;
    }

    return first;
}