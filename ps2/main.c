#include <stdio.h>
#include <stdlib.h>
#include "k.h"
#include "ui.h"
#include "hof.h"

int main(){
    struct game game = {
        .board = {
            {'A', 'B', 'D', 'D'},
            {'B', ' ', 'C', 'A'},
            {'D', 'A', 'B', 'D'},
            {' ', 'B', 'C', 'H'}
        },
        .score = 412
    };
    int dy = 1;
    int dx = 0;
    update(&game, dy, dx);
        render(game);
    is_game_won(game);

    add_random_tile(&game);
    is_move_possible(game);
    
        struct player player;

    struct player list[10];

    strcpy(player.name, "Banka");
    player.score = 100;
    int size = load(list);
    bool result = save(list, size);
    result = add_player(list, &size, player);

    printf("%d", result);
    return 0;
}

