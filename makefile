all: primes

primes: primes.cpp
	mpic++ primes.cpp -lm -o primes


