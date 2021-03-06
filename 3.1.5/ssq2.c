
/* ------------------------------------------------------------------------- 
 * This program - an extension of program ssq1.c - simulates a single-server 
 * FIFO service node using Exponentially distributed interarrival times and 
 * Uniformly distributed service times (i.e. a M/U/1 queue). 
 *
 * Name              : ssq2.c  (Single Server Queue, version 2)
 * Author            : Steve Park & Dave Geyer 
 * Language          : ANSI C 
 * Latest Revision   : 9-11-98
 * ------------------------------------------------------------------------- 
 */

#include <stdio.h>
#include <math.h>                                             
#include "rngs.h"

#define LAST         1000000L                   /* number of jobs processed */ 
#define START        0.0                      /* initial time             */ 


   double Exponential(double m)                 
/* ---------------------------------------------------
 * generate an Exponential random variate, use m > 0.0 
 * ---------------------------------------------------
 */
{       
  SelectStream(0);                                
  return (-m * log(1.0 - Random()));     
}


   double Uniform(double a, double b)           
/* --------------------------------------------
 * generate a Uniform random variate, use a < b 
 * --------------------------------------------
 */
{        
  SelectStream(85);                                 
  return (a + (b - a) * Random());    
}

  long Geometric(double p)
/* -----------------------------------
 * generate a geometric random variate
 * -----------------------------------
 */ 
{
    SelectStream(170);
    return ((long) (log(1.0 - Random()) / log(p)));
}

   double GetArrival(void)
/* ------------------------------
 * generate the next arrival time
 * ------------------------------
 */ 
{       
  static double arrival = START;                                        

  arrival += Exponential(2.0);
  return (arrival);
}

   double GetService(void)
/* ------------------------------
 * generate the next service time
 * ------------------------------
 */ 
{
  long   k;
  double sum = 0.0;
  long   tasks = 1 + Geometric(0.9);

  for (k = 0; k < tasks; k++)
    sum += Uniform(0.1,0.2);
  return sum;

  // Original GetService function for this file.
  // return (Uniform(1.0, 2.0));
}


  int main(void)
{
  long   index     = 0;                         /* job index            */
  double arrival   = START;                     /* time of arrival      */
  double delay;                                 /* delay in queue       */
  double service;                               /* service time         */
  double wait;                                  /* delay + service      */
  double departure = START;                     /* time of departure    */
  struct {                                      /* sum of ...           */
    double delay;                               /*   delay times        */
    double wait;                                /*   wait times         */
    double service;                             /*   service times      */
    double interarrival;                        /*   interarrival times */
  } sum = {0.0, 0.0, 0.0};  

  PlantSeeds(123456789);

  while (index < LAST) {
    index++;
    arrival      = GetArrival();
    if (arrival < departure)
      delay      = departure - arrival;         /* delay in queue    */
    else
      delay      = 0.0;                         /* no delay          */
    service      = GetService();
    wait         = delay + service;
    departure    = arrival + wait;              /* time of departure */
    sum.delay   += delay;
    sum.wait    += wait;
    sum.service += service;
  } 
  sum.interarrival = arrival - START;

  printf("\nfor %ld jobs\n", index);

  printf("   average interarrival time (r) = %6.2f\n", sum.interarrival / index);
  printf("   average wait ............ (w) = %6.2f\n", sum.wait / index);
  printf("   average delay ........... (d) = %6.2f\n\n", sum.delay / index);
  printf("   average service time .... (s) = %6.2f\n\n", sum.service / index);
  printf("   average # in the node ... (l) = %6.2f\n", sum.wait / departure);
  printf("   average # in the queue .. (q) = %6.2f\n", sum.delay / departure);
  printf("   utilization ............. (x) = %6.2f\n\n", sum.service / departure);

  return (0);
}
