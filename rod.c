#include <stdio.h>  
#include "rngs.h"

int main(void){
    double i;
    for(int l = 0; l < 2; l++)
        i = Random();
    
    printf("%f",i);
    return 0;
}