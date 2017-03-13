#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <math.h>
#include <time.h>
#include <mpi.h>

// Be careful when changing this
// Keep in mind the size of the random numbers
#define COPRIME_MASK 0xFFF

long double generateProbability(unsigned long long);
unsigned long long isCoprime(uint16_t, uint16_t);

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

	unsigned long long trials = 10000;

	if(argc > 1)
		trials = atoi(argv[1]);

	long double prob = 0.0l;

	int node;
	for(node = 1; node < nodes; ++node) {
		MPI_Send(&trials, 1, MPI_UNSIGNED_LONG_LONG, node, tag, MPI_COMM_WORLD);
	}

	do {

		long double recievedProb;
		MPI_Recv(&recievedProb, 1, MPI_LONG_DOUBLE, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &status);
		prob += recievedProb;

	} while(--node > 1);

	prob /= (long double)(nodes - 1);

	long double pi = sqrtl(6.0l/prob);

	printf("Approximation: %Lf\n", pi);
	printf("Trials:  %llu\n", trials);
	printf("Coprime argument mask: 0x%x\n", COPRIME_MASK);

	return 0;

}

int slave()
{

	unsigned long long trials;
	MPI_Recv(&trials, 1, MPI_UNSIGNED_LONG_LONG, 0, tag, MPI_COMM_WORLD, &status);
	long double prob = generateProbability(trials);
	MPI_Send(&prob, 1, MPI_LONG_DOUBLE, 0, tag, MPI_COMM_WORLD);

	return 0;

}

long double generateProbability(unsigned long long trials)
{
	srand(time(NULL));

	register unsigned long long numCoprime = 0;
	register unsigned long long i;

	register uint16_t rand1;
	register uint16_t rand2;

	for(i=0 ; i<trials ; ++i) {

		rand1 = rand() & COPRIME_MASK;
		rand2 = rand() & COPRIME_MASK;

		numCoprime += isCoprime(rand1 + 1, rand2 + 1);

	}

	return (long double)(numCoprime)/trials;
}

unsigned long long isCoprime(uint16_t rand1, uint16_t rand2)
{
	if(((rand1 | rand2) & 1) == 0) return 0;

	while((rand1 & 1) == 0) rand1 >>= 1;

	if(rand1 == 1) return 1;

	do {

		while((rand2 & 1) == 0) rand2 >>= 1;
		if(rand2 == 1) return 1;

		if(rand1 > rand2) {
			register uint16_t swap = rand1;
			rand1 = rand2;
			rand2 = swap;
		}
		rand2 -= rand1;

	} while(rand2 != 0);

	return 0;

}
