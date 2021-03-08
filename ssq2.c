
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

#define LAST         1000000000L                   /* number of jobs processed */ 
#define START        0.0                      /* initial time             */ 


   double Exponential(double m)                 
/* ---------------------------------------------------
 * generate an Exponential random variate, use m > 0.0 
 * ---------------------------------------------------
 */
{                                       
  return (-m * log(1.0 - Random()));     
}


   double Uniform(double a, double b)           
/* --------------------------------------------
 * generate a Uniform random variate, use a < b 
 * --------------------------------------------
 */
{                                         
  return (a + (b - a) * Random());    
}


   double GetArrival(int *j)
/* ------------------------------
 * generate the next arrival time
 * ------------------------------
 */ 
{       
const double mean[2] = {4.0, 6.0};
static double arrival[2] = {START, START};
static int init = 1;
  double temp;
if (init) { /* initialize the arrival array */
  SelectStream(0);
  arrival[0] += Exponential(mean[0]);
  SelectStream(1);
  arrival[1] += Exponential(mean[1]);
  init = 0;
}
if (arrival[0] <= arrival[1])
  *j = 0; /* next arrival is job type 0 */
else
  *j = 1; /* next arrival is job type 1 */
temp = arrival[*j]; /* next arrival time */
SelectStream(*j); /* use stream j for job type j */
arrival[*j] += Exponential(mean[*j]);
return (temp);

}


   double GetService(int j)
/* ------------------------------
 * generate the next service time
 * ------------------------------
 */ 
{
  const double min[2] = {1.0, 0.0};
  const double max[2] = {3.0, 4.0};
  SelectStream(j + 2); /* use stream j + 2 for job type j */
  return (Uniform(min[j], max[j]));
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
    int jobType; // Either 0 or 1
    index++;
    arrival      = GetArrival(&jobType);
    if (arrival < departure)
      delay      = departure - arrival;         /* delay in queue    */
    else
      delay      = 0.0;                         /* no delay          */
    service      = GetService(jobType);
    wait         = delay + service;
    departure    = arrival + wait;              /* time of departure */
    sum.delay   += delay;
    sum.wait    += wait;
    sum.service += service;
  } 
  sum.interarrival = arrival - START;

  printf("\nfor %ld jobs\n", index);
  printf("   average interarrival time = %6.2f\n", sum.interarrival / index);
  printf("   average wait ............ = %6.2f\n", sum.wait / index);
  printf("   average delay ........... = %6.2f\n", sum.delay / index);
  printf("   average service time .... = %6.2f\n", sum.service / index);
  printf("   average # in the node ... = %6.2f\n", sum.wait / departure);
  printf("   average # in the queue .. = %6.2f\n", sum.delay / departure);
  printf("   utilization ............. = %6.2f\n", sum.service / departure);

  return (0);
}
