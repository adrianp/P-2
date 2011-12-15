//Compile using the makefile under Linux

//used only for Visual C++ (Windows)
//#include "stdafx.h"

#include <iostream>
#include <stdlib.h>
#include <math.h> //if using gcc, specify use of math to the linker (-lm)
#include <time.h>
#include <stdio.h>

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
	int i, j; //counters
	int err; //used for error codes
	
	//Initializing MPI
	err = MPI_Init (&argc, &argv);
	MPI_Comm_rank (MPI_COMM_WORLD, &pid);
	MPI_Comm_size (MPI_COMM_WORLD, &size);
	MPI_Status status;

	double execution_time = -MPI_Wtime();

	//Calculating the size of the chunk processed by each process
	min = 2 + pid*(limit-1)/size;
	max = 1 + (pid+1)*(limit-1)/size;
	no_primes = max - min + 1;
	master_size = (limit-1)/size;

	/*
	The master should process all the primes used for sieving; thus, we cannot
	have to many processes
	*/
	if(2 + master_size < (int) sqrt((double) limit))
	{
		if(pid == 0)
		{
			printf ("Too many processes, master will not find all the primes needed for sieving.\n");
		}
		MPI_Finalize();
		return 1;
	}

	//Initializing the arrays
	primes = (char*) malloc(no_primes);

	if (primes == NULL)
	{
		printf ("Memory allocation error.\n");
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

	/*
	The algorithm stops when the first process finds a primes greater 
	than sqrt(limit).
	*/
	while (curr_prime * curr_prime <= limit)
	{
		//determining where the sieving operation should start
		if (curr_prime * curr_prime > min)
		{
			/*
			Start from first unmarked yet multiple of curr_prime
			*/
			start = curr_prime * curr_prime - min; 
		}
		else 
		{
			if (min % curr_prime == 0)
			{
				start = 0; //start from the first element
			}
			else 
			{
				/*
					start from first multiple of curr_prime
				*/
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

	//sending/receveing primes used for sieving, may also use MPI_Bcast
	if(pid==0) {
		for(j = 1; j<size; j++) {
			MPI_Send(&curr_prime, 1, MPI_INT,j,1,MPI_COMM_WORLD);
		}
	}
	else {
		MPI_Recv(&curr_prime, 1, MPI_INT,0,1,MPI_COMM_WORLD, &status);
	}
  }//end while

	//counting the local number of primes
	local_count = 0;
	for (i = 0; i < no_primes; i++)
	{
		if(primes[i])
		{
			local_count += 1;
		}
	}

	free(primes);

	if (size > 1)
	{
		//aggregating the number of primes
		MPI_Reduce (&local_count, &global_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	}
	else //only one process sieved, get total count from it
	{
		global_count  = local_count;
	}

	//for debugging purposes, gather all primes
	//MPI_Gather(primes, no_primes, MPI_CHAR, all, no_primes, MPI_CHAR, 0, MPI_COMM_WORLD); 

	execution_time += MPI_Wtime();

	if(pid == 0)
	{
		printf ("We found %d primes\n", global_count);
		printf ("Execution time: %f seconds\n", execution_time);
	}

	/*
	In certain scenarios the application may hang at 
	MPI_Finalize() (e.g. asymmetric cluster in INF2?); 
	if this is the case, comment out the next line
	*/
	MPI_Finalize();
	return 0;
}
