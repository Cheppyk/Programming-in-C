#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

bool wasLetterBefore (const char arr[25], char lettertocheck){
    if(lettertocheck >= 'a' && lettertocheck <= 'z') {
            lettertocheck = lettertocheck - 32;
    }
    for(int i = 0; i < 25; i++){
        if(arr[i] == lettertocheck){
            return true;
        }
    }
    return false;
}

void upper(char *arr, int i){
        if(arr[i] >= 'a' && arr[i] <= 'z') {
            arr[i] = arr[i] - 32;
        } 
}

void generateMatric(char cipher[5][5], const char *key){
    int key_lngth = strlen(key);
    char* keymod = calloc(26, sizeof *key);
    char alphabet[25];
    int k = 0;
    for(int i = 0; i < 25; i++){
        if(i != 22){
            alphabet[k] = i + 65;
            k++;
        }else{
            alphabet[k] = i + 65 + 1;
            k++;
        }
    }
    alphabet[23] = 'Y';
    alphabet[24] = 'Z';
    k = 0;
    int flag = 0;
    for(int i = 0; i < 25; i++){
        if(k < key_lngth && flag != 1){
            if(wasLetterBefore(keymod, key[k]) || key[k] == 'w' || key[k] == 'W' || key[k] == ' '){
                if(key[k] == 'w' || key[k] == 'W'){
                    keymod[i] = 'V';
                    k++;
                    continue;
                }
                i--;
                k++;
                continue;
            }
            keymod[i] = key[k];
            upper(keymod, i);
            k++;
        }else{
            if(flag == 0){
                k = 0;
            }
            flag = 1;
            if(wasLetterBefore(keymod, alphabet[k])){
                i--;
                k++;
                continue;
            }
            keymod[i] = alphabet[k];
            k++;
        }   
    }
    k = 0;
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
            cipher[i][j] = keymod[k];
            k++;
        }
    }
    free(keymod);
}

void breakTheText(char *textMod, const char *text){
    int k = 0;
    int len = strlen(text);
    char* text1 = calloc(strlen(text) + 1, sizeof *text);
    for(int i = 0; i < len; i++){
        if(text[i] == ' ') continue;
        if(text[i] == 'W' || text[i] == 'w'){
            text1[k] = 'V'; 
            k++;
            continue;
        } 
        text1[k] = text[i];
        upper(text1, k);
        k++;
    }
    text1[k] = '\0';
    k = 0;
    for(int i = 0; i < strlen(text1); i++){
        if(text1[i] == text1[i+1] && (text1[i] != 'X' && text1[i+1] != 'X')){
            textMod[k] = text1[i];
            textMod[k+1] = 'X';
            textMod[k+2] = text1[i+1];
            k =k+3;
            i++;
        }else{
            textMod[k] = text1[i];
            textMod[k+1] = text1[i+1];
            k=k+2;
            i++;
        }
    }
    k = strlen(textMod);
    if(k % 2 == 1){
        textMod[k] = 'X';
        textMod[k+1] = '\0';
    }
    free(text1);
}

void findLetters(char cipher[5][5], char textMod[], int index){
    int x1 = 0, x2 = 0, y1 = 0, y2 = 0;
        for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
            if(cipher[i][j] == textMod[index]){
                x1 = j;
                y1 = i;
            }
            if(cipher[i][j] == textMod[index + 1]){
                x2 = j;
                y2 = i;
            }
        }
    }
    if(x1 == x2){
        y1 = y1 + 1;
        y2 = y2 + 1;
        if(y1 > 4) y1 = 0;
        if(y2 > 4) y2 = 0;
        textMod[index] = cipher[y1][x1];
        textMod[index + 1] = cipher[y2][x2];
        return;
    }
    if(y1 == y2){
        x1 = x1 + 1;
        x2 = x2 + 1;
        if(x1 > 4) x1 = 0;
        if(x2 > 4) x2 = 0;
        textMod[index] = cipher[y1][x1];
        textMod[index + 1] = cipher[y2][x2];
        return;
    }
    textMod[index] = cipher[y1][x2];
    textMod[index + 1] = cipher[y2][x1];
    return;
}

void findLettersDecrypt(char cipher[5][5], char textMod2[], int index){
    int x1 = 0, x2 = 0, y1 = 0, y2 = 0;
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
            if(cipher[i][j] == textMod2[index]){
                x1 = j;
                y1 = i;
            }
            if(cipher[i][j] == textMod2[index + 1]){
                x2 = j;
                y2 = i;
            }
        }
    }
    if(x1 == x2){
        y1 = y1 - 1;
        y2 = y2 - 1;
        if(y1 < 0) y1 = 4;
        if(y2 < 0) y2 = 4;
        textMod2[index] = cipher[y1][x1];
        textMod2[index + 1] = cipher[y2][x2];
        return;
    }
    if(y1 == y2){
        x1 = x1 - 1;
        x2 = x2 - 1;
        if(x1 < 0) x1 = 4;
        if(x2 < 0) x2 = 4;
        textMod2[index] = cipher[y1][x1];
        textMod2[index + 1] = cipher[y2][x2];
        return;
    }
    textMod2[index] = cipher[y1][x2];
    textMod2[index + 1] = cipher[y2][x1];
    return;
}

char* playfair_encrypt(const char* key, const char* text){
    if(key == NULL || key[0] == '\0' || key[0] == ' ' || text == NULL || text[0] == '\0') return NULL;
    for(int i = 0; i < strlen(key); i++){
        if(isalpha(key[i]) == 0 && key[i] != ' ') return NULL;
    }
    char cipher[5][5];
    char *textMod = calloc(2 * strlen(text) + 2, sizeof *text);
    generateMatric(cipher, key);
    // for(int i = 0; i < 5; i++){
    //     for(int j = 0; j < 5; j++){
    //         printf("%c", cipher[i][j]);
    //     }
    //     printf("\n");
    //}
    breakTheText(textMod, text);
    for(int i = 0; i < strlen(textMod) - 1; i++){
        findLetters(cipher, textMod, i);
        i++;
    }
    char *spacedString = calloc(2 * strlen(textMod), sizeof *textMod);

    int j = 0;
    for (int i = 0; i < strlen(textMod); i++) {
        spacedString[j++] = textMod[i];
        if (i % 2 == 1) {
            spacedString[j++] = ' ';
        }
    }

    spacedString[j-1] = '\0';
    free(textMod);
    return spacedString;
}

char* playfair_decrypt(const char* key, const char* text){
    if(key == NULL || key[0] == '\0' || text == NULL || text[0] == '\0' || key[0] == ' ') return NULL;
    for(int i = 0; i < strlen(key); i++){
        if(isalpha(key[i]) == 0 && key[i] != ' ') return NULL;
    }
    for(int i = 0; i < strlen(text); i++){
        if(text[i] == 'W' || text[i] == 'w') return NULL;
    }
    char cipher[5][5];
    int len = strlen(text);
    char *textMod2 = calloc(len, sizeof *text);
    int k = 0;
    int p = 0;
    while(text[p] != '\0'){
        if(text[p] == ' '){
            p++;
            continue;
        }
        textMod2[k] = text[p];
        p++;
        k++;
    }
    textMod2[k] = '\0';
    generateMatric(cipher, key);
    for(int i = 0; i < strlen(textMod2); i++){
        findLettersDecrypt(cipher, textMod2, i);
        i++;
    }
    return textMod2;
}

// int main(){

//     char *encrypted, *decrypted;

//     // even length of string
//     encrypted = playfair_encrypt("SeCReT", "Hello world");
//     printf("%s", encrypted);
//     // "Hello world" --> "HELXLOVORLDX"
//     // IS JZ JQ XN TK JC
//     decrypted = playfair_decrypt("SeCReT", encrypted);
//     printf("\n%s\n", decrypted);
//     // HELXLOVORLDX
//     free(encrypted);
//     free(decrypted);
//     return 0;
// }