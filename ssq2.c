
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
//1000000000 100000
#define LAST         8367781L                   /* number of jobs processed */ 
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
  const double mean[3] = {4.0, 6.0, 8.0};
  static double arrival[3] = {START, START, START};
  static int init = 1;
    double temp;
  if (init) { // To initialize the array of values
    SelectStream(0);
    arrival[0] += Exponential(mean[0]);
    SelectStream(1);
    arrival[1] += Exponential(mean[1]);
    SelectStream(2);
    arrival[2] += Exponential(mean[2]);
    init = 0;
  }
  if (arrival[0] <= arrival[1] && arrival[0] <= arrival[2])
    *j = 0; // Next arrival is of type 0
  else if (arrival[1] <= arrival[0] && arrival[1] <= arrival[2])
    *j = 1; // Next arrival is of type 1
  else // Neither type 0 or 1, so must be type 2
    *j = 2;
  temp = arrival[*j]; // The next arrival time
  SelectStream(*j); // Should use the stream of type j
  arrival[*j] += Exponential(mean[*j]);
  return (temp);
}


   double GetService(int j)
/* ------------------------------
 * generate the next service time
 * ------------------------------
 */ 
{
  const double min[3] = {0.0, 1.0, 1.0};
  const double max[3] = {2.0, 2.0, 5.0};
  SelectStream(j + 3); /* use stream j + 2 for job type j */
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
  int jobs[3] = {0, 0, 0}; //Count all jobs
  struct {                                      /* sum of ...           */
    double delay;                               /*   delay times        */
    double wait;                                /*   wait times         */
    double service;                             /*   service times      */
    double interarrival;                        /*   interarrival times */
  } sum = {0.0, 0.0, 0.0};  

  // To record the value of the delay, wait and service for each job type.
  double sumJob[9] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 }; 
  
  PlantSeeds(123456789);

  while (index < LAST) {
    int jobType; // Either 0, 1 or 2
    index++;
    arrival      = GetArrival(&jobType);
    if (arrival < departure)
      delay      = departure - arrival;         /* delay in queue    */
    else
      delay      = 0.0;                         /* no delay          */
    service      = GetService(jobType);
    wait         = delay + service;
    departure    = arrival + wait;              /* time of departure */

    sumJob[0 + (jobType * 3)]   += delay;
    sumJob[1 + (jobType * 3)]   += wait;
    sumJob[2 + (jobType * 3)]   += service;
    
    sum.delay   += delay;
    sum.wait    += wait;
    sum.service += service;
    jobs[jobType]++;
  } 
  sum.interarrival = arrival - START;

  printf("\nfor %ld jobs\n", index);
  printf("   average interarrival time (r) = %6.2f\n\n", sum.interarrival / index);

  printf("ALL JOBS\n");
  printf("   average wait ............ (w) = %6.2f\n", sum.wait / index);
  printf("   average delay ........... (d) = %6.2f\n", sum.delay / index);
  printf("   average service time .... (s) = %6.2f\n", sum.service / index);
  printf("   average # in the node ... (l) = %6.2f\n", sum.wait / departure);
  printf("   average # in the queue .. (q) = %6.2f\n", sum.delay / departure);
  printf("   utilization ............. (x) = %6.2f\n\n", sum.service / departure);

  printf("JOB TYPE 0\n");
  printf("   average wait ............ (w) = %6.2f\n", sumJob[1] / index);
  printf("   average delay ........... (d) = %6.2f\n", sumJob[0] / index);
  printf("   average service time .... (s) = %6.2f\n", sumJob[2] / index);
  printf("   average # in the node ... (l) = %6.2f\n", sumJob[1] / departure);
  printf("   average # in the queue .. (q) = %6.2f\n", sumJob[0] / departure);
  printf("   utilization ............. (x) = %6.2f\n\n", sumJob[2] / departure);

  printf("JOB TYPE 1\n");
  printf("   average wait ............ (w) = %6.2f\n", sumJob[4] / index);
  printf("   average delay ........... (d) = %6.2f\n", sumJob[3] / index);
  printf("   average service time .... (s) = %6.2f\n", sumJob[5] / index);
  printf("   average # in the node ... (l) = %6.2f\n", sumJob[4] / departure);
  printf("   average # in the queue .. (q) = %6.2f\n", sumJob[3] / departure);
  printf("   utilization ............. (x) = %6.2f\n\n", sumJob[5] / departure);

  printf("JOB TYPE 2\n");
  printf("   average wait ............ (w) = %6.2f\n", sumJob[7] / index);
  printf("   average delay ........... (d) = %6.2f\n", sumJob[6] / index);
  printf("   average service time .... (s) = %6.2f\n", sumJob[8] / index);
  printf("   average # in the node ... (l) = %6.2f\n", sumJob[7] / departure);
  printf("   average # in the queue .. (q) = %6.2f\n", sumJob[6] / departure);
  printf("   utilization ............. (x) = %6.2f\n\n", sumJob[8] / departure);

  printf("Job 0: %f; Job 1: %f, Job 2: %f    \n", (float)jobs[0]/LAST, (float)jobs[1]/LAST, (float)jobs[2]/LAST);

  return (0);
}
