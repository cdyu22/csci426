
/* -------------------------------------------------------------------------
 * This program simulates a simple (s,S) inventory system using demand read  
 * from a text file.  Backlogging is permitted and there is no delivery lag.
 * The output statistics are the average demand and order per time interval
 * (they should be equal), the relative frequency of setup and the time
 * averaged held (+) and short (-) inventory levels.
 *
 * NOTE: use 0 <= MINIMUM < MAXIMUM, i.e., 0 <= s < S.
 *
 * Name              : sis1.c  (Simple Inventory System, version 1)
 * Authors           : Steve Park & Dave Geyer 
 * Language          : ANSI C 
 * Latest Revision   : 8-20-97 
 * Compile with      : gcc sis1.c
 * ------------------------------------------------------------------------- 
*/

#include <stdio.h>                        

#define FILENAME  "sis1.dat"            /* input data file                */
#define MINIMUM   20                    /* 's' inventory policy parameter */
#define MAXIMUM   80                    /* 'S' inventory policy parameter */
#define sqr(x)    ((x) * (x))

/* ======================== */
   long GetDemand(FILE *fp)            
/* ======================== */
{
  long d;

  fscanf(fp, "%ld\n", &d);
  return (d);
}

/* ============== */
   int main(void)
/* ============== */
{
  FILE *fp;                                /* input data file         */
  long index     = 0;                      /* time interval index     */
  long inventory = MAXIMUM;                /* current inventory level */
  long demand;                             /* amount of demand        */
  long order;                              /* amount of order         */
  struct {                                 /* sum of ...              */
    double setup;                          /*   setup instances       */
    double holding;                        /*   inventory held (+)    */
    double shortage;                       /*   inventory short (-)   */
    double order;                          /*   orders                */
    double demand;                         /*   demands               */
  } sum = { 0.0, 0.0, 0.0, 0.0, 0.0 };

  fp = fopen(FILENAME, "r");
  if (fp == NULL) {
    fprintf(stderr, "Cannot open input file %s\n", FILENAME);
    return (1);
  }

  while (!feof(fp)) {
    index++;
    if (inventory < MINIMUM) {             /* place an order          */
      order         = MAXIMUM - inventory;
      sum.setup++;
      sum.order    += order;
    }
    else                                   /* no order                 */
      order         = 0;                   
    inventory      += order;               /* there is no delivery lag */
    demand          = GetDemand(fp);
    sum.demand     += demand;
    if (inventory > demand) 
      sum.holding  += (inventory - 0.5 * demand);
    else {
      sum.holding  += sqr(inventory) / (2.0 * demand);
      sum.shortage += sqr(demand - inventory) / (2.0 * demand);
    }
    inventory      -= demand;
  }

  if (inventory < MAXIMUM) {               /* force the final inventory to */
    order           = MAXIMUM - inventory; /* match the initial inventory  */
    sum.setup++;
    sum.order      += order;
    inventory      += order;
  }

  printf("\nfor %ld time intervals ", index);
  printf("with an average demand of %6.2f\n", sum.demand / index);
  printf("and policy parameters (s, S) = (%d, %d)\n\n", MINIMUM, MAXIMUM);
  
  printf("   average order (o).......... = %6.2f: %f\n", sum.order / index,sum.order / index);
  printf("   setup frequency (u)........ = %6.2f: %f\n", sum.setup / index,sum.setup / index);
  printf("   average holding level (l+). = %6.2f: %f\n", sum.holding / index,sum.holding / index);
  printf("   average shortage level (l-) = %6.2f: %f\n\n", sum.shortage / index,sum.shortage / index);

  double itemCost = sum.order / index * 8000;
  double setupCost = sum.setup / index * 1000;
  double holdCost = sum.holding / index * 25;
  double shortCost = sum.shortage / index * 700.0;

  printf("   item cost ................. = %6.2f\n", itemCost);
  printf("   setup cost ................ = %6.2f\n", setupCost);
  printf("   holding cost .............. = %6.2f\n", holdCost);
  printf("   shortage cost ............. = %6.2f\n\n", shortCost);

  double dependentCost = setupCost + holdCost + shortCost;
  double totalCost = itemCost + setupCost + holdCost + shortCost;
  printf("   dependent cost............. = %6.2f\n", dependentCost);
  printf("   total cost................. = %6.2f\n", totalCost);

  fclose(fp);
  return (0);
}
