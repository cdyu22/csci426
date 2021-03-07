#include <stdio.h>  
#include "rngs.h"

int main( void ){

    // The amount of wins (triangles) and losses (not triangles)
    int win = 0, loss = 0; 

    // The amount of times we want to run the simulation
    int runs = 10000000;

    // Populating the stream so we can plant our own values.
    PlantSeeds( 123456789 );

    // Default value, placed in stream 0
    // SelectStream( 0 );
    // PutSeed( 123456789 );

    // Seed 10,000,001 cycles away, placed in stream 1
    // SelectStream( 1 );
    // PutSeed( 453248854 );
    

    /* ----------------------------------------------------------
     * The calculation to get the seed 10,000,001 cycles away    |
     * double i, long int x;                                     |
     * for ( int l = 0; l < 10000001; l++ )                      |
     *     i = Random();                                         |
     * GetSeed( &x );                                            |
     * printf( "Seed is: %d", x );                               |
     * ----------------------------------------------------------                      
     */

    // Declaration of points
    double p1, p2, temp;
    
    // The sides that are created
    double side1, side2, side3;

    // The for loop to run our simulation
    for( int cycles = 0; cycles < runs; cycles++ ){
        
        // Go to stream 0 and get a random value, iterates stream 0
        SelectStream( 0 );
        p1 = Random();
        
        // Go to stream 1 and get a random value, iterates stream 1
        SelectStream( 1 );
        p2 = Random();

        // The switch statement if p1 is less than p2
        if ( p2 < p1 ){
            temp = p2;
            p2 = p1;
            p1 = temp;
        }

        // Calculating the length of the sides
        side1 = p1;
        side2 = p2 - p1;
        side3 = 1 - p2;

        // If any of them are over .5, then it's not a triangle
        if ( ( .5 < side1 ) || ( .5 < side2 ) || ( .5 < side3 ) )
            loss++;
        else
            win++;
    }

    // Calculate the rate it's a triangle, and the rate it's not
    double winrate  = win  / ( ( double ) runs );
    double lossrate = loss / ( ( double ) runs );

    printf( "wins: %d, loss: %d\n", win, loss );
    printf( "winrate: %f, lossrate: %f", winrate, lossrate );

    return 0;
}