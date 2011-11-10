//compile with mpic++
//to run use: mpirun -np <no of procs> /full/path/to/exe
//may need to run mpd

//#include "mpi.h"

//used only for Visual C++ (Windows)
//#include "stdafx.h"

#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <time.h>

using namespace std;

int main(int argc, char** argv) {
	
	const int limit = atoi(argv[1]);
	bool* primes = new bool[limit];
	time_t startTime, endTime;
	
	cout << "Execution started..." << endl;
	
	time(&startTime);

	//loop to initialize the number array
	//cout << "Initializing array..." << endl;
	for(int i=0; i<limit; i++)
	{
		if(i>2 && i%2 == 1)
		{
			primes[i] = true;
		}
		else
		{
			if(i == 2)
			{
				primes[i] = true;
			}
			else
			{
				primes[i] = false;
			}
		}
	}


	//loop for eliminating composite numbers
	//cout << "Sieving..." << endl;
	for(int i=3; i<sqrt((double)limit); i++)
	{
		if(primes[i])
		{
			for(int j=2*i; j<limit; j+=i)
			{
				primes[j] = false;
			}
		}
	}
	time(&endTime);
	cout << "Execution time: " << difftime(endTime, startTime) << " seconds" << endl;
	
	cout << "Now printing (if anything to print)..." << endl;
	switch(atoi(argv[2]))
	{
		case 1: //print all prime numbers
			for(int i=0; i<limit; i++)
			{
				if(primes[i]) 
				{
					cout << i << endl;
				}
			}
			break;
		case 2: //print largest prime number
			int i = limit-1;
			while(true)
			{
				if(primes[i])
				{
					cout << i << endl;
					break;
				}
				else
				{
					i -= 1;
				}
			}
			break;
	}

	return 0;
}
