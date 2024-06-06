#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

void char2bits(char c, unsigned char bits[8]) {
  for (size_t i = 0; i < 8; i += 1) {
    bits[i] = (c >> (7 - i)) & 1;
  }
}

char bits2char(unsigned char bits[8]) {
  char result = 0;
  for (size_t i = 0; i < 8; i += 1) {
    result |= bits[i] << (7 - i);
  }
  return result;
}

void tupper(char *arr, int i){
        if(arr[i] >= 'a' && arr[i] <= 'z') {
            arr[i] = arr[i] - 32;
        } 
}

char* reverse(const char* text){
    if(text == NULL) return NULL;
    char *reversedText = calloc(strlen(text)+ 1, sizeof *text);
    int k = 0;
    for (int i = 0; i < strlen(text); i++) {
        reversedText[i] = text[strlen(text) - 1 - i];
        tupper(reversedText, i);
        k++;
    }
    reversedText[k] = '\0';
    return reversedText;
}

char* vigenere_encrypt(const char* key, const char* text){
    if(key == NULL || key[0] == '\0' || text == NULL || text[0] == '\0') return NULL;
    for(int i = 0; i < strlen(key); i++){
        if(isalpha(key[i]) == 0) return NULL;
    }
    char keyUpper[strlen(key)];
    char *encryptedText = calloc(strlen(text) + 1, sizeof *text);
    for(int i = 0; i < strlen(text); i++){
        encryptedText[i] = text[i];
        tupper(encryptedText, i);
    }
    for(int i = 0; i < strlen(key) + 1; i++){
        keyUpper[i] = key[i];
        tupper(keyUpper, i);
    }
    int curLet = 0;
    int k = 0;    
    for(int i = 0; i < strlen(text); i++){
        if(encryptedText[i] < 'A' || encryptedText[i] > 'Z'){
            continue;
        }
        curLet = encryptedText[i] + keyUpper[k];
        k++;
        if(curLet > 90){
            curLet = curLet % 26 + 65;
        }
        if(k > strlen(keyUpper) - 1){
            k = 0;
        }
        encryptedText[i] = curLet;
    }
    return encryptedText;
}

char* vigenere_decrypt(const char* key, const char* text){
    if(key == NULL || key[0] == '\0' || text == NULL || text[0] == '\0') return NULL;
    for(int i = 0; i < strlen(key); i++){
        if(isalpha(key[i]) == 0) return NULL;
    }
    char keyUpper[strlen(key)];
    char *encryptedText = calloc(strlen(text) + 1, sizeof *text);
    for(int i = 0; i < strlen(text); i++){
        encryptedText[i] = text[i];
        tupper(encryptedText, i);
    }
    for(int i = 0; i < strlen(key); i++){
        keyUpper[i] = key[i];
        tupper(keyUpper, i);
    }
    keyUpper[strlen(key)] = '\0';
    int curLet = 0;
    int k = 0;
    for(int i = 0; i < strlen(text); i++){
        
        if(encryptedText[i] < 'A' || encryptedText[i] > 'Z'){
            continue;
        }
        curLet = encryptedText[i] - (keyUpper[k] - 65);
        k++;
        if(curLet < 65){
            curLet = curLet + 26;
        }
        if(k > strlen(keyUpper) - 1){
            k = 0;
        }
        encryptedText[i] = curLet;
    }
    return encryptedText;
}

unsigned char* bit_encrypt(const char* text){
    if(text == NULL || text[0] == '\0') return NULL;
    int len = strlen(text);
    unsigned char* encrypted = calloc(strlen(text) + 1, sizeof *text);
    unsigned char bits[8];
    for(int i = 0; i < len; i++){
        char2bits(text[i], bits);
        char temp = bits[0];
        bits[0] = bits[1];
        bits[1] = temp;
        temp = bits[2];
        bits[2] = bits[3];
        bits[3] = temp;
        for(int k = 0; k < 4; k++){
            if(bits[k] != bits[k + 4]){
                bits[k + 4] = 1;
            }else{
                if(bits[k] == bits[k + 4]) bits[k + 4] = 0;
            }
        }
        encrypted[i] = bits2char(bits);
    }
    return encrypted;
}

char* bit_decrypt(const unsigned char* text) {
    if(text == NULL || text[0] == '\0') return NULL;
    int len = 0;
    while(text[len] != '\0'){
        len++;
    }
    char* decrypted = calloc(len + 1, sizeof *text);
    unsigned char bits[8];
    for (int i = 0; i < len; i++) {
        char2bits(text[i], bits);

        for (int k = 0; k < 4; k++) {
            if (bits[k] != bits[k + 4]) {
                bits[k + 4] = 1;
            } else {
                if (bits[k] == bits[k + 4]) bits[k + 4] = 0;
            }
        }
        char temp = bits[0];
        bits[0] = bits[1];
        bits[1] = temp;
        temp = bits[2];
        bits[2] = bits[3];
        bits[3] = temp;

        decrypted[i] = bits2char(bits);
    }
    return decrypted;
}

unsigned char* bmp_encrypt(const char* key, const char* text){
    if(key == NULL || key[0] == '\0' || text == NULL || text[0] == '\0') {
        return NULL;
    }
    for(int i = 0; i < strlen(key); i++){
        if(isalpha(key[i]) == 0 && key[i] != ' ') {
            return NULL;
        }
    }
    char *textMod = calloc(strlen(text)+1, sizeof *text);
    for(int i = 0; i < strlen(text); i++){
        textMod[i] = text[i];
    }
    char* rever = reverse(textMod);
    free(textMod);
    char * vegenerenc = vigenere_encrypt(key, rever);
    free(rever);
    unsigned char* encrypted = bit_encrypt(vegenerenc);
    free(vegenerenc);
    return encrypted;
}

char* bmp_decrypt(const char* key, const unsigned char* text){
    if(key == NULL || key[0] == '\0' || text == NULL || text[0] == '\0') return NULL;
    for(int i = 0; i < strlen(key); i++){
        if(isalpha(key[i]) == 0) return NULL;
    }
    int len = 0;
    while(text[len] != '\0'){
        len++;
    }
    unsigned char *textMod = calloc(len + 1, sizeof *text);
    for(int i = 0; i < len; i++){
        textMod[i] = text[i];
    }
    char *decrypted = bit_decrypt(textMod);
    free(textMod);
    char *vigdecrypt = vigenere_decrypt(key, decrypted);
    free(decrypted);
    char *rev = reverse(vigdecrypt);
    free(vigdecrypt);
    return rev;
}

// int main(){
//     unsigned char* encrypted;
//     encrypted = bmp_encrypt("osculating", "Striga nemohla vydrzat bolest dala mu jednu mast aby tam a tam siel do tej jamy a tou mastou bratovi hrdlo potrel naraz ozil a prisli oba ku zverom");
//     for(int i=0; i < 147;i++){
//          printf("%x ", encrypted[i]);
//     }
//     char* decrypt = bmp_decrypt("osculating", encrypted);
//     printf("\n%s\n", decrypt);
//     free(encrypted);
//     free(decrypt);
// }