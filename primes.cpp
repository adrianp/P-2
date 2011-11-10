//compile with mpic++
//to run use: mpirun -np <no of procs> /full/path/to/exe
//may need to run mpd

//#include "mpi.h"
#include <iostream>
#include <stdlib.h>
#include <math.h>

using namespace std;

int main(int argc, char** argv) {
	int limit = atoi(argv[1]);
	bool primes[limit];
	int end = sqrt(limit);

	//loop to initialize the number array
	for(int i=0; i<limit; i++)
	{
		if(i>1)
		{
			primes[i] = true;
		}
		else
		{
			//TODO: get rid of this hack (0 and 1 not primes)
			primes[i] = false;
		}
	}


	//loop for eliminating composite numbers
	for(int i=2; i<end; i++)
	{
		if(primes[i])
		{
			for(int j=2*i; j<limit; j+=i)
			{
				primes[j] = false;
			}
		}
	}

	//loop for printing the prime numbers
	for(int i=0; i<limit; i++)
	{
		if(primes[i])
		cout << i << endl;
	}
	return 0;
}
