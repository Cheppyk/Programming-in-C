#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// float racer(){
//     int n = 4, t = 10;
//     double sum_s = 0, sum_v = 0;

//     scanf("%d %d", &n, &t);
//     int svArr[n][2];

//     // svArr[0][0] = 5;
//     // svArr[0][1] = 3;
//     int s,v;
//     for (int i = 0; i < n; i++) {
//         scanf("%d %d", &s, &v);
//         svArr[i][0] = s; //Distance
//         svArr[i][1] = v; //Skorost
//         // svArr[i][0] = rand() % 1000 + 1; //Distance
//         // svArr[i][1] = rand() % 2001 - 1000; //Skorost
//     }
//     // for (int i = 0; i < n; i++) {
//     //     sum_s += rand() % 1000 + 1; //Distance
//     //     sum_v += rand() % 2001 - 1000; //Skorost
//     // }

//     for (int i = 0; i < n; i++) {
//         sum_s += svArr[i][0]; //Distance
//         sum_v += svArr[i][1]; //Skorost
//     }

//     double avg_speed = sum_s / t;
//     double k = avg_speed - (sum_v / n);
//     return k;
// }

// double result(int n, int t, double svArr[][2]) {
//     double k = 0;
//     double error = 0.000001;
//     double step = 2.0;
//     long double diff;
//     //int i = 0;
//     do {
//         long double sum = 0;
        
//         for (int i = 0; i < n; ++i) {
//             double s = svArr[i][0];
//             double v = svArr[i][1];
//             sum += s / (v + k);
//         }
//         diff = sum - t;
        
        
//         if (diff > 0 || sum < 0) {
//             if(fabs(diff)>error) k += step;
//         }else{
//             if(fabs(diff)>error) k -= step;
//         }
//         step /= 1.1;
//         printf("%f\n", k);
//     } while (fabs(diff) > error);
    
//     return k;
// }

double result(int n, int t, double svArr[][2]){
    double low = -2001, high = 2001, k;
    double error = 0.000000001;
    double res = high - low;
    while(res > error){
        k = (low + high) / 2;
        double sumT = 0;
        
        for(int i = 0; i < n; i++){
            if(svArr[i][1] + k <= 0){
                sumT = -1;
                break;
            }
            sumT += svArr[i][0] / (svArr[i][1] + k);
        }
        
        if(sumT < 0 || sumT > t){
            low = k;
        }else{
            high = k;
        }
        res = high-low;
    }
    k = (low + high) / 2;
    return k;
}

int main() {
    int n, t;
    scanf("%d %d", &n, &t);
    
    double svArr[n][2];
    for(int i = 0; i < n; i++){
        scanf("%lf %lf", &svArr[i][0], &svArr[i][1]);
    }
    
    double k = result(n, t, svArr);
    printf("%.6f\n", k);
    return 0;
}