#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <math.h>
#include <time.h>
#include <mpi.h>

// Be careful when changing this
// Keep in mind the size of the random numbers
#define COPRIME_MASK 0x7FFF

long double generateProbability(uint64_t);
uint64_t isCoprime(uint32_t, uint32_t);

int master(int, char**);
int slave();

static MPI_Status status;
static int tag = 0;
static int rank;
static int nodes;

int main(int argc, char** argv)
{


	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nodes);

	if(nodes == 1) {
		fprintf(stderr, "There must be more than 1 CPU available\n");
		return 1;
	}

	int code = 0;

	if(rank == 0)
		code = master(argc, argv);
	else
		code = slave();

	MPI_Finalize();

	return code;

}

int master(int argc, char** argv)
{

	uint64_t trials = 10000;

	if(argc > 1)
		trials = atoi(argv[1]);

	long double prob = 0.0l;

	int node;
	for(node = 1; node < nodes; ++node) {
		MPI_Send(&trials, 1, MPI_INTEGER8, node, tag, MPI_COMM_WORLD);
	}

	do {

		long double recievedProb;
		MPI_Recv(&recievedProb, 1, MPI_LONG_DOUBLE, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &status);
		prob += recievedProb;

	} while(--node > 1);

	prob /= (long double)(nodes - 1);

	long double pi = sqrtl(6.0l/prob);

	printf("Approximation: %Lf\n", pi);
	if(log(trials) < 17)
		printf("Trials:  %llu\n", (unsigned long long)trials);
	else
		printf("Trials (log): %f\n", log(trials));
	printf("Coprime argument mask: 0x%x\n", COPRIME_MASK);

	return 0;

}

int slave()
{

	uint64_t trials;
	MPI_Recv(&trials, 1, MPI_INTEGER8, 0, tag, MPI_COMM_WORLD, &status);
	long double prob = generateProbability(trials);
	MPI_Send(&prob, 1, MPI_LONG_DOUBLE, 0, tag, MPI_COMM_WORLD);

	return 0;

}

long double generateProbability(uint64_t trials)
{
	srand(time(NULL));

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
