//Compile using the makefile under Linux

//used only for Visual C++ (Visual Studio)
//#include "stdafx.h"

#include <iostream>
#include <stdlib.h>
#include <math.h> //if using g++, specify use of math to the linker (-lm)
#include <time.h>
#include <stdio.h>

int main(int argc, char** argv) 
{

	if(!argv[1])
	{
		printf("Specify the limit as a command line argument.\n");
		return 1; //"echo $?" to get returned value
	}	
	
	const int limit = atoi(argv[1]);
	char* primes = new char[limit]; //processed numbers;
	time_t startTime, endTime; //used for measuring execution time
	int global_count = 0; //total number of primes
	int i, j; //counters

	//start measuring execution time
	time(&startTime);

	//loop to initialize the number array
	for(i=0; i<limit; i++)
	{
		if(i > 1)
		{
			primes[i] = 1;
		}
		else
		{
			primes[i] = 0;
		}
	}

	//sieving loop
	for(i=2; i<sqrt((double)limit); i++)
	{
		if(primes[i])
		{
			for(j=2*i; j<limit; j+=i)
			{
				primes[j] = 0; 
			}
		}
	}

	time(&endTime);

	//counting the number of primes
	for(i=0; i<limit; i++)
	{
		if(primes[i])
		{
			global_count += 1;
		}
	}

	free(primes);

	printf("We found %d primes\n",global_count);
	printf("Execution time: %f seconds\n", difftime(endTime, startTime));

	return 0;
}
