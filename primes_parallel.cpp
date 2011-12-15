#include <math.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "mpi.h"

int main (int argc, char *argv[])
{
  const int limit = atoi(argv[1]);
  char *primes; //processed numbers; used CHAR instead of BOOL because of MPI limitations
  //char *all = (char*) malloc(limit); //global array
  int local_count; //number of primes in process
  int global_count; //global number of primes
  int start; /* Index of first multiple */
  int min; //lowest value in primes[]
  int max; //highest value in primes[]
  int pid;
  int index; /* Index of current prime */
  int size; //number of processes
  int master_size; //number of elements processed by master
  int curr_prime; /* Current prime */
  int no_primes; //sizeof(primes)
  int i;

  //Initializing MPI
  MPI_Init (&argc, &argv);
  MPI_Comm_rank (MPI_COMM_WORLD, &pid);
  MPI_Comm_size (MPI_COMM_WORLD, &size);
  //MPI_Barrier(MPI_COMM_WORLD);
  double execution_time = -MPI_Wtime();

  //Calculating the chunk processed by each process
  min = 2 + pid*(limit-1)/size;
  max = 1 + (pid+1)*(limit-1)/size;
  no_primes = max - min + 1;
  master_size = (limit-1)/size;

  /*
  The master should process all the primes used for sieving; thus, we cannot
  have to many processes
  */
  if((2 + master_size) < (int) sqrt((double) limit))
  {
    if(pid == 0)
    { 
      printf ("Too many processes\n");
    }
    MPI_Finalize();
    return 1;
  }

  //Initializing the arrays
  primes = (char*) malloc(no_primes);

  if (primes == NULL)
  {
    printf ("Cannot allocate enough memory\n");
    MPI_Finalize();
    return 2;
  }

  for(i = 0; i < no_primes; i++)
  {
    primes[i] = 1;
  }

  if (pid == 0)
  {
    index = 0;
  }
  curr_prime = 2;
  
  while (curr_prime <= sqrt(limit)) 
  {
    //determining where the sieving operation should start
    if (pow(curr_prime, 2) > min)
    {
      start = pow(curr_prime, 2) - min;
    }
    else 
    {
      if (!(min % curr_prime))
      {
        start = 0;
      }
      else 
      {
        start = curr_prime - (min % curr_prime);
      }
    }

    //Actual sieving operation
    for (i = start; i < no_primes; i += curr_prime) 
    {
      primes[i] = 0;
    }

    if(pid == 0) //only on master
    {
      //getting the next prime that will be used for sieving
      while(!primes[++index]);
      curr_prime = index + 2;
    }
    
    if (size > 1) //if only one process this does not make any sense
    {
      //broadcasting the next prime from master to all "slave" processes
      MPI_Bcast (&curr_prime,  1, MPI_INT, 0, MPI_COMM_WORLD);
    }
  } //end while

  //counting the local number of primes
  local_count = 0;
  for (i = 0; i < no_primes; i++)
  {
    if(primes[i])
    {
      local_count++;
    }
  }

  if (size > 1)
  {
    //aggregating the number of primes
    MPI_Reduce (&local_count, &global_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
  }
  else //only one process sieved, get total count from it
  {
    global_count  = local_count;
  }

  //TODO: fix inequal array size
  //MPI_Gather(primes, no_primes, MPI_CHAR, all, no_primes, MPI_CHAR, 0, MPI_COMM_WORLD); 




  execution_time += MPI_Wtime();


   /* Print the results */

    //printf("SIZE: %d\n",sizeof(all));

  if (pid == 0) { //id==0
         //TODO: this should be optional
         //for(i=0; i<n-2; i++)
         //{
         //   if(!all[i])
         //   {
         //     printf("%d\n",i+2);
         //   }
   
      printf ("There are %d primes less than or equal to %d\n",
         global_count, limit);
      printf ("SIEVE (%d) %10.6f\n", size, execution_time);
   }
   MPI_Finalize ();
   return 0;
}
