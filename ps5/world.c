#include "world.h"
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "container.h"

struct container* create_world(){
    struct room* entryHall = create_room("Entry Hall", "You are in the grand entry hall of the castle.");
    struct room* diningRoom = create_room("Dining Room", "A large table set with dusty plates and silver.");
    struct room* mainHall = create_room("Main Hall", "The heart of the castle with several doorways leading to mysterious rooms.");
    struct room* wineRoom = create_room("Wine Room", "A room filled with rare vintages and a peculiar, musty aroma.");
    struct room* kitchen = create_room("Kitchen", "Where meals were prepared. It has an unsettling, cold feel.");
    struct room* courtyard = create_room("Courtyard", "An open area exposed to the elements, surrounded by the towering castle.");
    struct room* chambers = create_room("Lady Dimitrescu's Chambers", "An opulently decorated bedroom fit for the towering matriarch of the castle.");
    struct room* tower = create_room("Tower", "The highest point in the castle with a panoramic, yet haunting view.");
    struct room* library = create_room("Library", "Shelves of ancient books fill this quiet, dusty room.");
    struct room* ballroom = create_room("Ballroom", "A vast room with a faded grandeur, once used for lavish dances.");
    struct room* gallery = create_room("Gallery", "Paintings and statues line the walls, their eyes seeming to follow you.");
    struct room* study = create_room("Study", "A personal study filled with notes and books about strange experiments.");
    struct room* armory = create_room("Armory", "Rows of ancient weapons and armor are displayed here.");
    struct room* observatory = create_room("Observatory", "A room with a large telescope, overlooking the castle's lands.");
    struct room* gardens = create_room("Gardens", "Overgrown and wild, these gardens hold many hidden secrets.");
    struct room* crypt = create_room("Crypt", "The cold, stone crypt under the castle, echoing with whispers of the past.");

    // Linking rooms
    set_exits_from_room(entryHall, mainHall, diningRoom, wineRoom, kitchen);
    set_exits_from_room(diningRoom, entryHall, courtyard, ballroom, NULL);
    set_exits_from_room(mainHall, chambers, entryHall, tower, NULL);
    set_exits_from_room(wineRoom, NULL, NULL, NULL, entryHall);
    set_exits_from_room(kitchen, NULL, NULL, entryHall, NULL);
    set_exits_from_room(courtyard, diningRoom, gardens, NULL, NULL);
    set_exits_from_room(chambers, tower, mainHall, NULL, NULL);
    set_exits_from_room(tower, NULL, chambers, library, mainHall);
    set_exits_from_room(library, NULL, NULL, study, tower);
    set_exits_from_room(ballroom, diningRoom, gallery, NULL, NULL);
    set_exits_from_room(gallery, ballroom, study, NULL, NULL);
    set_exits_from_room(study, gallery, NULL, observatory, library);
    set_exits_from_room(observatory, NULL, NULL, NULL, study);
    set_exits_from_room(armory, NULL, NULL, crypt, NULL);
    set_exits_from_room(gardens, courtyard, NULL, crypt, NULL);
    set_exits_from_room(crypt, gardens, NULL, armory, NULL);

    // Create and add items
    struct item* key = create_item("Iron Key", "A heavy, ornate key that seems to unlock several doors within the castle.", MOVABLE | USABLE);
    struct item* candle = create_item("Candle", "An ever-burning candle, casting eerie shadows.", MOVABLE | USABLE);
    struct item* dagger = create_item("Silver Dagger", "A beautifully crafted dagger, rumored to be incredibly sharp.", MOVABLE | USABLE);
    struct item* map = create_item("Castle Map", "A detailed map of the castle, essential for navigation.", MOVABLE | EXAMINABLE);
    struct item* book = create_item("Ancient Book", "An old, mysterious book with cryptic texts.", MOVABLE | EXAMINABLE);

    add_item_to_room(library, book);
    add_item_to_room(ballroom, candle);
    add_item_to_room(study, map);
    add_item_to_room(armory, dagger);
    add_item_to_room(entryHall, key);

    // World container
    struct container* world = add_room_to_world(NULL, entryHall);
    add_room_to_world(world, diningRoom);
    add_room_to_world(world, mainHall);
    add_room_to_world(world, wineRoom);
    add_room_to_world(world, kitchen);
    add_room_to_world(world, courtyard);
    add_room_to_world(world, chambers);
    add_room_to_world(world, tower);
    add_room_to_world(world, library);
    add_room_to_world(world, ballroom);
    add_room_to_world(world, gallery);
    add_room_to_world(world, study);
    add_room_to_world(world, armory);
    add_room_to_world(world, observatory);
    add_room_to_world(world, gardens);
    add_room_to_world(world, crypt);

    return world;
}

struct container* add_room_to_world(struct container* world, struct room* room) {
    if (room == NULL) return NULL; // Check if the room is NULL

    if (world == NULL) {
        struct container* newContainer = malloc(sizeof(struct container));
        if (newContainer == NULL) return NULL;
        newContainer->type = ROOM;
        newContainer->room = room;
        newContainer->next = NULL;
        return newContainer;
    }

    struct container* current = world;
    while (current->next != NULL) {
        if (strcmp(current->room->name, room->name) == 0) {
            return NULL; 
        }
        current = current->next;
    }
    if (strcmp(current->room->name, room->name) == 0) {
        return NULL; 
    }

    struct container* newRoomContainer = malloc(sizeof(struct container));
    if (newRoomContainer == NULL) return NULL;
    newRoomContainer->type = ROOM;
    newRoomContainer->room = room;
    newRoomContainer->next = NULL;
    current->next = newRoomContainer;

    return newRoomContainer;
}

struct container* destroy_world(struct container* world){
    if(world == NULL) return NULL;
    struct container* head = world;
    while(head != NULL){
        if(head->room->items != NULL){
            destroy_containers(head->room->items);
        }
        head = head->next;
    }
    destroy_containers(world);
    return NULL;
}

struct room* get_room(struct container* world, char* name){
    if(world == NULL || name == NULL || name[0] == '\0') return NULL;
    return get_from_container_by_name(world, name);
}
