#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <math.h>
#include <time.h>

long double generateProbability(unsigned long long);
unsigned long long isCoprime(unsigned long long, unsigned long long);

int main(int argc, char** argv)
{

	unsigned long long trials = 10000;

	if(argc > 1)
		trials = atoi(argv[1]);

	srand(time(NULL));

	long double prob = generateProbability(trials);

	long double pi = sqrtl(6.0l/prob);

	printf("Approximation: %Lf\n", pi);
	printf("Trials:  %llu\n", trials);

	return 0;

}

long double generateProbability(unsigned long long trials)
{
	register unsigned long long numCoprime = 0;
	register unsigned long long i;

	register unsigned long long rand1;
	register unsigned long long rand2;

	for(i=0 ; i<trials ; ++i) {

		rand1 = rand();
		rand2 = rand();

		if(rand1 == 0 || rand2 == 0) {
			--i;
			continue;
		}

		numCoprime += isCoprime(rand1, rand2);

	}

	return (long double)(numCoprime)/trials;
}

unsigned long long isCoprime(unsigned long long rand1, unsigned long long rand2)
{
	if(((rand1 | rand2) & 1) == 0) return 0;

	while((rand1 & 1) == 0) rand1 >>= 1;

	if(rand1 == 1) return 1;

	do {

		while((rand2 & 1) == 0) rand2 >>= 1;
		if(rand2 == 1) return 1;

		if(rand1 > rand2) {
			register unsigned long long swap = rand1;
			rand1 = rand2;
			rand2 = swap;
		}
		rand2 -= rand1;

	} while(rand2 != 0);

	return 0;

}
