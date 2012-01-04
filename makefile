all: sequential parallel

sequential: primes_sequential.cpp
	g++ primes_sequential.cpp -lm -o primes_sequential

parallel: primes_parallel.cpp
	mpic++ primes_parallel.cpp -lm -o primes_parallel


