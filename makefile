all: primes_parallel.cpp
	mpic++ primes_parallel.cpp -lm -o primes_parallel


