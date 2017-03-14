#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <math.h>
#include <time.h>

// Be careful when changing this
// Keep in mind the size of the random numbers
#define COPRIME_MASK 0xFFF

long double generateProbability(uint64_t);
uint64_t isCoprime(uint32_t, uint32_t);

int main(int argc, char** argv)
{

	uint64_t trials = 10000;

	if(argc > 1)
		trials = atol(argv[1]);

	srand(time(NULL));

	long double prob = generateProbability(trials);

	long double pi = sqrtl(6.0l/prob);

	printf("Approximation: %Lf\n", pi);
	if(log(trials) < 17)
		printf("Trials:  %llu\n", (unsigned long long)trials);
	else
		printf("Trials (log): %f\n", log(trials));
	printf("Coprime argument mask: 0x%x\n", COPRIME_MASK);

	return 0;

}

long double generateProbability(uint64_t trials)
{
	register uint64_t numCoprime = 0;
	register uint64_t i;

	register uint32_t rand1;
	register uint32_t rand2;

	for(i=0 ; i<trials ; ++i) {

		rand1 = rand() & COPRIME_MASK;
		rand2 = rand() & COPRIME_MASK;

		numCoprime += isCoprime(rand1 + 1, rand2 + 1);

	}

	return (long double)(numCoprime)/trials;
}

uint64_t isCoprime(uint32_t rand1, uint32_t rand2)
{
	if(((rand1 | rand2) & 1) == 0) return 0;

	while((rand1 & 1) == 0) rand1 >>= 1;

	if(rand1 == 1) return 1;

	do {

		while((rand2 & 1) == 0) rand2 >>= 1;
		if(rand2 == 1) return 1;

		if(rand1 > rand2) {
			register uint32_t swap = rand1;
			rand1 = rand2;
			rand2 = swap;
		}
		rand2 -= rand1;

	} while(rand2 != 0);

	return 0;

}
