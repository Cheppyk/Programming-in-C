#include "game.h"
#include "world.h"
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

void play_game(struct game* game){
    return;
}

struct game* destroy_game(struct game* game){
    if(game == NULL) return NULL;
    destroy_backpack(game->backpack);
    destroy_parser(game->parser);
    game->current_room = NULL;
    destroy_world(game->world);
    free(game);
    return NULL;
}

struct game* create_game(){
    struct game* Game = malloc(sizeof(struct game));
    Game->state = PLAYING;
    if(Game->state != PLAYING){
        printf("Game state null\n");
        free(Game);
        return NULL;
    }
    Game->parser = create_parser();
    if(Game->parser == NULL){
        printf("Game parser == NULL\n");
        free(Game);
        return NULL;
    }
    Game->world = create_world();
    if(Game->world == NULL){
        printf("Game world == nULL\n");
        destroy_parser(Game->parser);
        free(Game);
        return NULL;
    }
    Game->current_room = Game->world->room;
    if(Game->current_room == NULL){
        printf("Current room is NULL\n");
        destroy_world(Game->world);
        destroy_parser(Game->parser);
        free(Game);
        return NULL;
    }
    Game->backpack = create_backpack(4);
    if(Game->backpack == NULL){
        printf("Backpack == NULL\n");
        free(Game->current_room);
        destroy_containers(Game->world);
        destroy_parser(Game->parser);
        free(Game);
        return NULL;
    }
    return Game;
}

void print_commands(struct container* command){
    if(command == NULL) return;
    struct container* head = command;
    while(head != NULL){
        printf("%s\n", head->command->name);
        head = head->next;
    }
    return;
}

void show_inventory(struct container* backpackItems){
    struct container* head = backpackItems;
    printf("\nYour inventory:\n");
    while(head != NULL){
        printf("%s\n", head->item->name);
        head = head->next;
    }
    return;
}

void execute_command(struct game* game, struct command* command){
    if(game == NULL || command == NULL) return;
    if(strcmp(command->name, "KONIEC") == 0){
        game->state = GAMEOVER;
        return;
    }else if(strcmp(command->name, "RESTART") == 0){
        game->state = RESTART;
        return;
    }else if(strcmp(command->name, "JUH") == 0){
        game->current_room = game->current_room->south;
        return;
    }else if(strcmp(command->name, "SEVER") == 0){
        game->current_room = game->current_room->north;
        return;
    }else if(strcmp(command->name, "VYCHOD") == 0){
        game->current_room = game->current_room->east;
        return;
    }else if(strcmp(command->name, "ZAPAD") == 0){
        game->current_room = game->current_room->west;
        return;
    }else if(strcmp(command->name, "ROZHLIADNI SA") == 0){
        show_room(game->current_room);
        return;
    }else if(strcmp(command->name, "PRIKAZY") == 0){
        print_commands(game->parser->commands);
        return;
    }else if(strcmp(command->name, "VERZIA") == 0){
        printf("\nMade by Voronovskyi Yehor, Version 1.\n");
        return;
    }else if(strcmp(command->name, "O HRE") == 0){
        printf("\nToto je moja hra.\n");
        return;
    }else if(strcmp(command->name, "VEZMI") == 0){
        char* input = malloc(50 * sizeof(char));
        scanf("Co chcete vziat? %49s", input);
        struct item* item = get_item_from_room(game->current_room, input);
        if(item == NULL){
            printf("Failde to find an item.\n");
            free(input);
            return;
        }
        bool additem = add_item_to_backpack(game->backpack, item);
        if(additem == false){
            printf("You can't pickup this item.\n");
        }
        free(input);
        return;
    }else if(strcmp(command->name, "POLOZ") == 0){
        char* input = malloc(50 * sizeof(char));
        scanf("Co chcete polozit? %49s", input);
        struct item* foundItem = get_item_from_backpack(game->backpack, input);
        if(foundItem == NULL){
            printf("Failde to find an item.\n");
            free(input);
            return;
        }
        delete_item_from_backpack(game->backpack, foundItem);
        add_item_to_room(game->current_room, foundItem);
        free(input);
        return;
    }else if(strcmp(command->name, "INVENTAR") == 0){
        show_inventory(game->backpack->items);
        return;
    }else if(strcmp(command->name, "PRESKUMAJ") == 0){
        char* input = malloc(50 * sizeof(char));
        scanf("Co chcete preskumat? %49s", input);
        struct item* item = get_item_from_backpack(game->backpack, input);
        if(item == NULL){
            item = get_item_from_room(game->current_room, input);
        }
        if(item == NULL){
            printf("Couldn't find your item\n");
            free(input);
            return;
        }else{
            printf("%s", item->description);
        }
        return;
    }else if(strcmp(command->name, "NAHRAJ") == 0){
        //sdelat
        return;
    }else if(strcmp(command->name, "ULOZ") == 0){
        //sdelat
        return;
    }
    return;
}
