#include "item.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

struct item* create_item(char* name, char* description, unsigned int properties){
    if(name == NULL || description == NULL || name[0] == '\0' || description[0] == '\0') return NULL;
    struct item* newItem = malloc(sizeof(struct item));
    newItem->name = malloc(strlen(name) + 1);
    strcpy(newItem->name, name);
    newItem->description = malloc(strlen(description) + 1);
    strcpy(newItem->description, description);
    newItem->properties = properties;
    return newItem;
}

struct item* destroy_item(struct item* item){
    if (item == NULL) return NULL;
    if(item->name != NULL) free(item->name);
    if(item->description != NULL) free(item->description);
    free(item);
    return NULL;
}
