#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include "k.h"

int po(int base, int exp){
    for(int i = 1; i < exp; i++){
        base = base * 2;
    }
    return base;
}
void add_random_tile(struct game *game){
    int row, col;
    // find random, but empty tile
    do{
        row = rand() % SIZE;
        col = rand() % SIZE;
    }while(game->board[row][col] != ' ');

    // place to the random position 'A' or 'B' tile
    if(rand() % 2 == 0){
        game->board[row][col] = 'A';
    }else{
        game->board[row][col] = 'B';
    }
}

bool is_game_won(const struct game game){
    for(int i = 0; i < SIZE; i++){
        for(int j = 0; j < SIZE; j++){
            if(game.board[i][j] == 'K'){
                return true;
            }
        }
    }
    return false;
}

bool is_move_possible(const struct game game){
    for(int i = 0; i < SIZE; i++){
        for(int j = 0; j < SIZE; j++){
            if(game.board[i][j] == ' ') return true;
        }
    }
    for(int i = 0; i < SIZE - 1; i++){
        for(int j = 0; j < SIZE; j++){
            if(game.board[i][j] == game.board[i + 1][j]){
                return true;
            }
        }
    }
    for(int j = 0; j < SIZE - 1; j++){
        for(int i = 0; i < SIZE; i++){
            if(game.board[i][j] == game.board[i][j + 1]){
                return true;
            }
        }
    }
    return false;
}

bool update(struct game *game, int dy, int dx){
    if((dy == 0 && dx == 0) || ((dy == 1 || dy == -1) && dx != 0) || (dy != 0 && (dx == 1 || dx == -1))) return false;
    int count = 0;
    int score = 0;
    int flag = 0;
    if(dy != 0){
        switch(dy){
            //up
            case -1:
                for(int i = 0; i < SIZE; i++){
                    count = 0;
                    for(int j = 0; j < SIZE; j++){
                        if(game->board[j][i] == ' ' && count != 3){
                            for(int k = j; k < SIZE - 1; k++){
                                game->board[k][i] = game->board[k+1][i];
                                game->board[k+1][i] = ' ';
                            }
                            j--;
                            count++;
                            flag = 1;
                        }
                    }
                }
                for(int i = 0; i < SIZE; i++){
                    for(int j = 0; j < SIZE; j++){
                        if(game->board[j][i] ==  game->board[j + 1][i] && game->board[j][i] != ' '){
                            game->board[j][i] = game->board[j][i] + 1;
                            score = score + po(2,game->board[j][i] - 64);
                            game->board[j + 1][i] = ' ';
                            j++;
                            flag = 1;

                        }
                    }
                }
                for(int i = 0; i < SIZE; i++){
                    count = 0;
                    for(int j = 0; j < SIZE; j++){
                        if(game->board[j][i] == ' ' && count != 3){
                            for(int k = j; k < SIZE - 1; k++){
                                game->board[k][i] = game->board[k+1][i];
                                game->board[k+1][i] = ' ';
                            }
                            j--;
                            count++;
                            flag = 1;

                        }
                    }
                }
                break;
            //down
            case 1:
                for(int i = 0; i < SIZE; i++){
                    count = 0;
                    for(int j = 3; j > 0; j--){
                        if(game->board[j][i] == ' ' && count != 3){
                            for(int k = j; k > 0; k--){
                                game->board[k][i] = game->board[k-1][i];
                                game->board[k-1][i] = ' ';
                            }
                            j++;
                            count++;
                            flag = 1;
                        }
                    }
                }
                for(int i = 0; i < SIZE; i++){
                    for(int j = 3; j > 0; j--){
                        if(game->board[j][i] ==  game->board[j - 1][i] && game->board[j][i] != ' '){
                            game->board[j][i] = game->board[j][i] + 1;
                            score = score + po(2,game->board[j][i] - 64);
                            game->board[j - 1][i] = ' ';
                            j--;
                            flag = 1;
                        }
                    }
                }
                for(int i = 0; i < SIZE; i++){
                    count = 0;
                    for(int j = 3; j > 0; j--){
                        if(game->board[j][i] == ' ' && count != 3){
                            for(int k = j; k > 0; k--){
                                game->board[k][i] = game->board[k-1][i];
                                game->board[k-1][i] = ' ';
                            }
                            j++;
                            count++;
                            flag = 1;
                        }
                    }
                }
                break;
            default:
                return false;
        }
    }else{
        switch(dx){
            case -1:
                count = 0;
                for(int i = 0; i < SIZE; i++){
                    count = 0;
                    for(int j = 0; j < SIZE; j++){
                        if(game->board[i][j] == ' ' && count != 3){
                            for(int k = j; k < SIZE - 1; k++){
                                game->board[i][k] = game->board[i][k + 1];
                                game->board[i][k + 1] = ' ';
                            }
                            j--;
                            count++;
                            flag = 1;
                        }
                    }
                }
                for(int i = 0; i < SIZE; i++){
                    for(int j = 0; j < SIZE - 1; j++){
                        if(game->board[i][j] ==  game->board[i][j + 1] && game->board[i][j] != ' '){
                            game->board[i][j] = game->board[i][j] + 1;
                            score = score + po(2,game->board[i][j] - 64);
                            game->board[i][j + 1] = ' ';
                            j++;
                            flag = 1;
                        }
                    }
                }
                for(int i = 0; i < SIZE; i++){
                    count = 0;
                    for(int j = 0; j < SIZE; j++){
                        if(game->board[i][j] == ' ' && count !=3){
                            for(int k = j; k < SIZE - 1; k++){
                                game->board[i][k] = game->board[i][k + 1];
                                game->board[i][k + 1] = ' ';
                            }
                            j--;
                            count++;
                            flag = 1;
                        }
                    }
                }
                break;
            case 1:
                for(int i = 0; i < SIZE; i++){
                    count = 0;
                    for(int j = 3; j > 0; j--){
                        if(game->board[i][j] == ' ' && count != 3){
                            for(int k = j; k > 0; k--){
                                game->board[i][k] = game->board[i][k - 1];
                                game->board[i][k - 1] = ' ';
                            }
                            j++;
                            count++;
                            flag = 1;
                        }
                    }
                }
                for(int i = 0; i < SIZE; i++){
                    for(int j = 3; j > 0; j--){
                        if(game->board[i][j] ==  game->board[i][j - 1] && game->board[i][j] != ' '){
                            game->board[i][j] = game->board[i][j] + 1;
                            score = score + po(2,game->board[i][j] - 64);
                            game->board[i][j - 1] = ' ';
                            j--;
                            flag = 1;
                        }
                    }
                }
                for(int i = 0; i < SIZE; i++){
                    count = 0;
                    for(int j = 3; j > 0; j--){
                        if(game->board[i][j] == ' ' && count != 3){
                            for(int k = j; k > 0; k--){
                                game->board[i][k] = game->board[i][k - 1];
                                game->board[i][k - 1] = ' ';
                            }
                            j++;
                            count++;
                            flag = 1;
                        }
                    }
                }
                break;
            default:
                return false;
        }
    }
    game->score = game->score + (int)score;
    if(flag == 1){
        return true;
    }else{
        return false;
    }
}

// struct game game = {
//     .board = {
//         {'J', 'I', ' ', 'C'},
// 	{' ', 'C', 'E', 'E'},
// 	{'F', 'C', ' ', 'H'},
// 	{' ', ' ', ' ', 'G'}
//     },
//     .score = 5247
// };

// int main(){
//     update(&game, 0, -1);
//     for(int i = 0; i < SIZE; i++){
//         for(int j = 0; j < SIZE; j++){
//             printf("%c",game.board[i][j]);
//         }
//         printf("\n");
//     }
//     printf("\n%d\n", game.score); //807
//     return 0;
// }