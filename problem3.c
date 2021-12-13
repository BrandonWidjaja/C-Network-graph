/*
problem3.c

Driver function for Problem 3.

Skeleton written by Grady Fitzpatrick for COMP20007 Assignment 1 2021
*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <limits.h>

/* Constants */
#define OLDCHIP 0
#define NEWCHIP 1
#define MAXNUMERATOR 100
#define MAXDENOMINATOR 100

/* Used to store all the statistics for a single chip. */
struct statistics;

/* Used to store all the statistics for both chips for each problem. */
struct chipStatistics;

struct statistics {
  int operations;
  int instances;
  int minOperations;
  double avgOperations;
  int maxOperations;
};

struct chipStatistics {
  struct statistics oldChipEuclid;
  struct statistics newChipEuclid;
  struct statistics oldChipSieve;
  struct statistics newChipSieve;
};

/* Set all statistics to 0s */
void initialiseStatistics(struct statistics *stats);

/* Collects the minimum, average and maximum operations from running all
combinations of numerators from 1 to the given maxNumerator and 1 to the given
maxDenominator. */
void collectStatistics(struct chipStatistics *chipStats, int maxNumerator,
  int maxDenominator);

/* Divides the number of operations by the number of instances. */
void calculateAverage(struct statistics *stats);

/* Prints out the minimum, average and maximum operations from given
statistics. */
void printStatistics(struct statistics *stats);

/* Calculates the number of operations required for Euclid's algorithm given the
numerator and denominator when running on the given chip type (one of OLDCHIP
and NEWCHIP) by moving through the steps of the algorithm and counting each
pseudocode operation. */
void euclid(int numerator, int denominator, int chip, struct statistics *s);

/* Calculates the number of operations required for the sieve of Eratosthenes
given the numerator and denominator when running on the given chip type (one of
OLDCHIP and NEWCHIP) by moving through the steps of the algorithm and counting
each pseudocode operation. */
void eratosthenes(int numerator, int denominator, int chip,
  struct statistics *s);

/* returns the minimum of two given integers */
int min(int num1, int num2);

int main(int argc, char **argv){
  struct chipStatistics summaryStatistics;

  collectStatistics(&summaryStatistics, MAXNUMERATOR, MAXDENOMINATOR);

  printf("Old chip (Euclid):\n");
  printStatistics(&(summaryStatistics.oldChipEuclid));
  printf("\n");
  printf("New chip (Euclid)\n");
  printStatistics(&(summaryStatistics.newChipEuclid));
  printf("\n");
  printf("Old chip (Sieve)\n");
  printStatistics(&(summaryStatistics.oldChipSieve));
  printf("\n");
  printf("New chip (Sieve)\n");
  printStatistics(&(summaryStatistics.newChipSieve));
  printf("\n");

  return 0;
}

void collectStatistics(struct chipStatistics *chipStats, int maxNumerator,
  int maxDenominator){
  int numerator, denominator;
  /* Initialise all statistics */
  initialiseStatistics(&(chipStats->oldChipEuclid));
  initialiseStatistics(&(chipStats->newChipEuclid));
  initialiseStatistics(&(chipStats->oldChipSieve));
  initialiseStatistics(&(chipStats->newChipSieve));

  for(numerator = 1; numerator <= maxNumerator; numerator++){
    for(denominator = 1; denominator <= maxDenominator; denominator++){
      /* Run algorithms for all combinations of numerator and denominator. */
      euclid(numerator, denominator, OLDCHIP,
        &(chipStats->oldChipEuclid));
      euclid(numerator, denominator, NEWCHIP,
        &(chipStats->newChipEuclid));
      eratosthenes(numerator, denominator, OLDCHIP,
        &(chipStats->oldChipSieve));
      eratosthenes(numerator, denominator, NEWCHIP,
        &(chipStats->newChipSieve));
    }
  }
  calculateAverage(&(chipStats->oldChipEuclid));
  calculateAverage(&(chipStats->newChipEuclid));
  calculateAverage(&(chipStats->oldChipSieve));
  calculateAverage(&(chipStats->newChipSieve));
}

void calculateAverage(struct statistics *stats){
  stats->avgOperations = (double) stats->operations / stats->instances;
}

void initialiseStatistics(struct statistics *stats){
  stats->operations = 0;
  stats->instances = 0;
  stats->minOperations = INT_MAX;
  stats->avgOperations = 0;
  stats->maxOperations = 0;
}

void euclid(int numerator, int denominator, int chip, struct statistics *s){
  /* operations in each calculation */
  int operations_local = 0;
  
  int b= numerator;
  int a= denominator;
  int t;
  /* the cost of the two assignments */
  operations_local += 2;
  
  while (b!=0) {
    /* cost of each while branch */
    operations_local += 1;

    t= b;
    b= a % b;
    a= t;
    /* cost of the mod (5) and the three assignments (3) */
    operations_local += 8;
  }

  /* add one more operation in the case where b==0 since operation counter
  in the loop is not executed but a while loop check is still being executed */
  operations_local++;

  /* update maxOperations in operations in this run is larger */
  if (operations_local > s->maxOperations) {
    s->maxOperations = operations_local;
  }
  /* update minOperations in operations in this run is smaller */
  if (operations_local < s->minOperations) {
    s->minOperations = operations_local;
  }

  /* add the operations from this run to total operations and an instance */
  s->operations += operations_local;
  s->instances++;
}

void eratosthenes(int numerator, int denominator, int chip,
  struct statistics *s){
  
  /* operations in each calculation */
  int operations_local = 0;

  int numCandidates;
  int num = numerator;
  int den = denominator;
  /* cost of two assignments */
  operations_local += 2;
  numCandidates = min(num, den);
  /* cost of numCandidates assignment */
  operations_local++;
  int primes[numCandidates];
  /* cost of array assignment */
  operations_local++;
  
  int i;
  /* initalise all values in primes array to 1 */
  for (i=0; i<numCandidates; i++){
    primes[i]= 1;
  }
  int j;
  i=1;
  /* cost of assignment */
  operations_local++;

  while (i<numCandidates){
    i++;
    /* cost of one while loop and assignment for 'i' */
    operations_local += 2;
    /* cost of 'if' check */
    operations_local++;
    if (primes[i]){
      j=i+i;
      if (chip==OLDCHIP){
        /* cost of assignment of 'j' */
        operations_local++;
      }
      while (j<=numCandidates){
        /* cost of while check */
        if (chip==OLDCHIP) {
          operations_local++;
        }
        /* cost of if check plus the two modulus */
        if (chip==OLDCHIP){
          operations_local += 11;
        }
        if ((j/i)>1 && (j % i)==0) {
          primes[j]=0;
          /* cost of assignment to 0 */
          if (chip==OLDCHIP) {
            operations_local++;
          }
        }
        j+=i;
        /* cost of j update */
        if (chip==OLDCHIP){
          operations_local++;
        }
      }

      /* counter for while loop check is in loop so we need to add one for
      the last check where loop isn't executed */
      if (chip==OLDCHIP) {
        operations_local++;
      }
      /* if new chip, previous while loop only costs 1 operation total */
      if (chip==NEWCHIP){
        operations_local++;
      }
      
      while (((num % i)==0) && ((den % i)==0)){
        /* cost of the two modulus (total 10) and while loop (1) */
        operations_local += 11;
        num = num/i;
        den = den/i;
        /*cost of two divisions and two assignments */
        operations_local += 12;
      }
      /* add one more while check cost with the two modulus for when the loop 
      isn't executed */
      operations_local += 11;
    } 
  }

  /* counter for number of the above while loop is in the while loop so we
  add one more for when the loop isn't executed but is still checked */
  operations_local++;
  
  /* update maxOperations in operations in this run is larger */
  if (operations_local > s->maxOperations) {
    s->maxOperations = operations_local;
  }
  /* update minOperations in operations in this run is smaller */
  if (operations_local < s->minOperations) {
    s->minOperations = operations_local;
  }

  /* add the operations from this run to total operations and an instance */
  s->operations += operations_local;
  s->instances++;
}

void printStatistics(struct statistics *stats){
  printf("Minimum operations: %d\n", stats->minOperations);
  printf("Average operations: %f\n", stats->avgOperations);
  printf("Maximum operations: %d\n", stats->maxOperations);
}

int min(int num1, int num2){
  return (num1 > num2 ) ? num2 : num1;
}