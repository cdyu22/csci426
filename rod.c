#include <stdio.h>  
#include "rngs.h"

int main(void){
    double i;
    long int x;
    int win = 0, loss = 0;
    int attempts = 10000000;

    SelectStream(1);
    PutSeed(453248854);
    SelectStream(0);

    // for (int l = 0; l  < 10000001; l++)
        // i = Random();
    // GetSeed(&x);
    // printf("Seed is: %d",x);
    // The seeds are 10000001 apart, 453248854

    double p1, p2, temp;
    for(int l = 0; l < attempts; l++){
        SelectStream(0);
        p1 = Random();
        
        SelectStream(1);
        p2 = Random();

        if (p2 < p1){
            temp = p2;
            p2 = p1;
            p1 = temp;
        }

        double break1, break2, break3;
        break1 = p1;
        break2 = p2 - p1;
        break3 = 1 - p2;

        if ( (.5 < break1) || (.5 < break2) || (.5 < break3) ){
            loss++;
        }
        else{    
            win++;}
    }

    printf("wins: %d, loss: %d\n",win,loss);
    double winrate = win / ((double)attempts);
    double lossrate = loss / ((double)attempts);
    printf("winrate: %f, lossrate: %f",winrate,lossrate);



    return 0;
}