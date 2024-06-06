#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// size_t countDigits(size_t n) {
//     int count = 0;
//     do {
//         count++;
//         n /= 10;
//     } while (n != 0);
//     return count;
// }

void result(){
    char input[1000001];
    int arr[1000000];
    int i = 1;

    scanf("%s", input);

    for (i = 0; i < strlen(input); i++)
    {
        arr[i] = input[i] - '0';
    }
    int arrSize = strlen(input);
    arr[arrSize] = '\0';
    while(1){
        int sum = 0;
        for(int k = 0; k < arrSize; k++){
            sum = arr[k] + sum;
        }
        char str[1000000];
        sprintf(str, "%d", sum);
        int q = 0;
        for (i = 0; i < strlen(str); i++)
        {
            arr[i] = str[i] - '0';
            q++;
        }
        if(q == 1){
            arr[q] = '\0';
            printf("%d\n", arr[0]);
            return;
            break;
        }
        arr[i] = '\0'; 
        arrSize = i;
    }
}

int main(){
    result();    
    return 0;
}