#include "game.h"
#include "world.h"
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

// void print_items(struct room* room){
//     struct container* itemcont = room->items;
//     if(itemcont == NULL){
//         printf("No items in the room\n\n");
//         return;
//     }
//     printf("List of items: \n");
//     while(itemcont != NULL){
//         printf("%s\n", itemcont->item->name);
//         itemcont = itemcont->next;
//     }
//     return;
// }

// void print_room(struct container* first){
//     struct container *head = first;
//     while(head != NULL){
//         printf("Name: %s\nDescription: %s\n", head->room->name, head->room->description);
//         show_room(head->room);
//         print_items(head->room);
//         if(head->next != NULL){
//             head = head->next;
//         }else{
//             return;
//         }
//     }
//     return;
// }

// int main(){
//     struct parser* parser = create_parser();
//     destroy_parser(parser);
// }

//для коммитов
int main(){
    struct room* start = create_room("Entry", "This is the entry to the house.");
    struct container* first = create_container(NULL, ROOM, start);
    destroy_room(start);
    // struct room* bathroom = create_room("Bathroom", "This is the bathroom.");
    // create_container(first, ROOM, bathroom);
    // get_from_container_by_name(first, "Bathroom");
    // set_exits_from_room(start, NULL, NULL, NULL, NULL);
    // remove_container(first, bathroom);
    // destroy_room(start);
    // destroy_room(bathroom);
    // destroy_containers(NULL);
    // destroy_backpack(NULL);
    // create_command(NULL, NULL, NULL, 20);
    // play_game(NULL);
    // create_item(NULL, NULL, 4);
    // destroy_parser(NULL);

    // show_room(NULL);
    // delete_item_from_room(NULL, NULL);
    // add_item_to_room(NULL, NULL);
    // create_world(NULL);
    // get_item_from_room(NULL, NULL);
    // create_backpack(4);
    // add_item_to_backpack(NULL, NULL);
    // get_item_from_backpack(NULL, NULL);
    // delete_item_from_backpack(NULL, NULL);
    // destroy_command(NULL);
    // create_parser();
    // create_game();
    // add_room_to_world(NULL, NULL);
    // create_world();
    // destroy_world(NULL);
    // get_room(NULL, NULL);
    // parse_input(NULL, NULL);
    // execute_command(NULL, NULL);
    // destroy_game(NULL);
}