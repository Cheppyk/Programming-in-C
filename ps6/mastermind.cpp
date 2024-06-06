#include "mastermind.h"
#include "lcd_wrapper.h"
#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define LIVES 10

enum led_controls{
    WELCOME,
    SOLVED,
    FAILED
};

bool alreadyExists(const char* secret, char random_number){
    for(int i = 0; i < strlen(secret); i++){
        if(secret[i] == random_number){
            return true;
        }
    }
    return false;
}

char* generate_code(bool repeat, int length){
    if(length < 1){
        fprintf(stderr, "Length is lower than 1.\n");
        return NULL;
    }
    char* secret = (char*)malloc(length + 1);
    if(secret == NULL){
        fprintf(stderr, "Memory allocation failed.\n");
        return NULL;
    }
    srand(time(NULL));
    for(int i = 0; i < length; i++){
        int random_number;
        do {
            random_number = rand() % 10;
        } while (repeat == false && alreadyExists(secret, random_number + '0'));
        secret[i] = random_number + '0';
    }
    secret[length] = '\0';
    return secret;
}

void render_leds(const int peg_a, const int peg_b){
    turn_off_leds();
    int redLed = 7;
    for(int j = 0; j < peg_a; j++){
        digitalWrite(redLed, HIGH);
        redLed += 2;
    }
    int blueLed = 6;
    for(int i = 0; i < peg_b; i++){
        digitalWrite(blueLed, HIGH);
        blueLed += 2;
    }
}

void turn_off_leds(){
    for(int i = 6; i < 14; i++){
        digitalWrite(i, LOW);
    }
}

void render_history(char* secret, char** history, const int entry_nr){
    int peg_a = 0, peg_b = 0;
    get_score(secret, history[entry_nr], &peg_a, &peg_b);
    render_leds(peg_a, peg_b);

}

void get_score(const char* secret, const char* guess, int* peg_a, int* peg_b){
    int len = strlen(secret);
    if(secret == NULL || guess == NULL){
        fprintf(stderr, "Secret or guess == NULL");
        return;
    }
    bool matched[len] = {false};
    *peg_a = 0;
    *peg_b = 0;

    // First pass for exact matches
    for(int i = 0; i < len; i++){
        if(secret[i] == guess[i]){
            *peg_a += 1;
            matched[i] = true;
        }
    }

    // Second pass for color matches in wrong positions
    for(int i = 0; i < len; i++){
        if(secret[i] != guess[i]){
            for(int j = 0; j < len; j++){
                if(!matched[j] && secret[j] == guess[i]){
                    *peg_b += 1;
                    matched[j] = true;
                    break;
                }
            }
        }
    }
}

//Visual effects for different states of the game.
void show_led(int state){
    switch(state){
        case WELCOME:
            for (int i = 6; i < 14; i++) {
                digitalWrite(i, HIGH);
                delay(100);
                digitalWrite(i, LOW);
            }
            for (int i = 13; i >= 6; i--) {
                digitalWrite(i, HIGH);
                delay(100);
                digitalWrite(i, LOW);
            }
            break;
        case SOLVED:
            for (int j = 0; j < 3; j++) {
                for (int i = 6; i < 14; i++) {
                    digitalWrite(i, HIGH);
                }
                delay(300);
                for (int i = 6; i < 14; i++) {
                    digitalWrite(i, LOW);
                }
                delay(300);
            }
            break;
        case FAILED:
            for (int i = 6; i < 14; i++) {
                digitalWrite(i, HIGH);
                delay(200);
            }
            for (int i = 6; i < 14; i++) {
                digitalWrite(i, LOW);
                delay(200);
            }
            break;
        default:
            break;
    }
}
//Updates the array that contains the current guess combination
void update_guess(const int button, char* guess, int index) {
    if(button == BTN_1_PIN || digitalRead(button) == HIGH) {
        delay(20);
        if (guess[index] < '9') {
            guess[index]++;
        } else {
            guess[index] = '0';
        }

        if(button != BTN_1_PIN) {
            while(digitalRead(button) == HIGH);
            delay(20);
        }
    }
    return;
}

//Outputs a welcome screen along with LED effects.
void printWelcome(){
    lcd_clear();
    lcd_print_at(0,0, "Welcome to");
    delay(500);
    lcd_print_at(0, 1, "MasterMind.");
    show_led(WELCOME);
    turn_off_leds();
    delay(2000);
    lcd_clear();
    lcd_print_at(0, 0, "Your task is to");
    lcd_print_at(0, 1, "guess a number.");
    delay(2000);
    lcd_clear();
    lcd_print_at(0, 0, "I am thinking");
    lcd_print_at(0, 1, "a number...");
    delay(1000);
    lcd_clear();
    lcd_print_at(0,0, "Thinking...");
}

//Function outputs corresponding text for winning or loosing the game and frees the memory allocated for history
void game_won(bool isWon, char** history){
    turn_off_leds();
    if(isWon == true){
        lcd_print_at(0, 0,"Congratulations!");
        turn_off_leds();
        show_led(SOLVED);
        turn_off_leds();
        lcd_clear();
        lcd_print_at(4, 1, "You won!");
        delay(2000);
    }else{
        lcd_clear();
        lcd_print_at(3, 0, "You lost!");
        turn_off_leds();
        show_led(FAILED);
        turn_off_leds();
        delay(1000);
    }

    for(int i = 0; i < LIVES; i++){
        if(history[i] != NULL) free(history[i]);
    }
    free(history);
    lcd_clear();
    lcd_print_at(0, 0, "Button 1 to");
    lcd_print_at(0, 1, "restart the game");
    while(digitalRead(BTN_1_PIN) == LOW);
}

//The main function
void play_game(char* secret){
    Serial.println(secret);
    char guess[5] = "0000";
    int lives = LIVES;
    char** history = (char**)calloc(LIVES, sizeof(char*));
    if(history == NULL){
        fprintf(stderr, "history allocation failed");
        return;
    }
    int entry = 0;
    printWelcome();
    do{
        //Emptying guess array.
        strcpy(guess, "0000");
        lcd_print_at(0, 1, "Guess: 0000");
        //Variable to correctly handle the output of the history.
        int tempEntry = entry - 1;
        do{
            //if corresponding button pressed, updates the array.
            update_guess(BTN_2_PIN, guess, 1);
            update_guess(BTN_3_PIN, guess, 2);
            update_guess(BTN_4_PIN, guess, 3);
            //An if to handle the view of the history.
            if(digitalRead(BTN_1_PIN) == HIGH){
                //A flag to check if the user pressed two buttons simultaneously if not  the first elemnt appends.
                bool btnFlag = true;
                while(digitalRead(BTN_1_PIN) == HIGH){
                    if(digitalRead(BTN_2_PIN) == HIGH){
                        delay(20);
                        btnFlag = false;
                        //Check if we are going out of the history bounds
                        if(tempEntry > 0){
                            tempEntry -= 1;
                            render_history(secret, history, tempEntry);
                            if((tempEntry+1) < 10){
                                lcd_print_at(0, 0, ( "0" + String(tempEntry + 1) + ": " + history[tempEntry]).c_str());

                            }else{
                                lcd_print_at(0, 0, (String(tempEntry + 1) + ": " + history[tempEntry]).c_str());
                            }
                            while(digitalRead(BTN_2_PIN) == HIGH);
                            delay(20);
                        }
                    }
                    if(digitalRead(BTN_3_PIN) == HIGH){
                        btnFlag = false;
                        delay(20);
                        //Check if we are going out of the history bounds
                        if(tempEntry < entry - 1){
                            tempEntry += 1;
                            render_history(secret, history, tempEntry);
                            if((tempEntry+1) < 10){
                                lcd_print_at(0, 0, ( "0" + String(tempEntry + 1) + ": " + history[tempEntry]).c_str());
                            }else{
                                lcd_print_at(0, 0, (String(tempEntry + 1) + ": " + history[tempEntry]).c_str());
                            }              
                            while(digitalRead(BTN_3_PIN) == HIGH);
                            delay(20);
                        }
                    }
                }
                if(btnFlag){
                    update_guess(BTN_1_PIN, guess, 0);
                }
            }
            lcd_print_at(7, 1, guess);
        }while(digitalRead(BTN_ENTER_PIN) == LOW);
        delay(50);
        int peg_a, peg_b;
        history[entry] = (char*)malloc(sizeof(char) * 5);
        if(history[entry] == NULL){
            fprintf(stderr, "History entry allocation failed.");
            return;
        }
        strcpy(history[entry], guess);
        history[entry][4] = '\0';
        entry++;
        lives--;
        lcd_clear();
        get_score(secret, guess, &peg_a, &peg_b);
        render_leds(peg_a, peg_b);
        //Check if the game is won.
        if(peg_a == 4){
            game_won(true, history);
            break;
        }
        if(lives == -1){
            lcd_clear();
            lcd_print_at(0, 0, "The answer is:");
            lcd_print_at(0, 3, secret);
            delay(3000);
            game_won(false, history);
            break;
        }else if(lives > 0){
            lcd_print_at(0, 0, (("0" + String(entry) + ": " + String(guess)).c_str()));
        }else{
            lcd_print_at(0, 0, ((String(entry) + ": " + String(guess)).c_str()));
        }
        //Waiting to enter button to become LOW to avoid unintentional guess skip.
        while(digitalRead(BTN_ENTER_PIN) == HIGH);
        delay(20);
    }while(true);
}
