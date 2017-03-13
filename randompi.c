#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <math.h>
#include <time.h>

#define COPRIME_MASK 0xFF

long double generateProbability(uint32_t);
short isCoprime(long, long);

/*
 *
 * VERY ACCURATE GENERATOR FOR THE DIGITS OF PI
 *
 */

int main(int argc, char** argv)
{

	uint32_t trials = 10000;

	if(argc > 1)
		trials = atoi(argv[1]);

	srand(time(NULL));

	long double prob = generateProbability(trials);

	long double pi = sqrtl(6.0l/prob);

	printf("Approximation: %Lf\n", pi);
	printf("Trials:  %u\n", trials);
	printf("Coprime argument mask: 0x%x\n", COPRIME_MASK);

	return 0;

}

long double generateProbability(uint32_t trials)
{
	register uint32_t numCoprime = 0;
	register uint32_t i;

	long rand1;
	long rand2;

	for(i=0 ; i<trials ; ++i) {

		rand1 = rand() & COPRIME_MASK;
		rand2 = rand() & COPRIME_MASK;

		numCoprime += isCoprime(rand1 + 1, rand2 + 1);

	}

	return (long double)(numCoprime)/trials;
}

short isCoprime(long rand1, long rand2)
{
	if(((rand1 | rand2) & 1) == 0) return 0;

	while((rand1 & 1) == 0) rand1 >>= 1;

	if(rand1 == 1) return 1;

	do {

		while((rand2 & 1) == 0) rand2 >>= 1;
		if(rand2 == 1) return 1;

		if(rand1 > rand2) {
			long swap = rand1;
			rand1 = rand2;
			rand2 = swap;
		}
		rand2 -= rand1;

	} while(rand2 != 0);

	return 0;

}
