(all):
	gcc -pedantic -Wall -Wextra -O2 -o randompi randompi.c -lm
parallel:
	mpicc -pedantic -Wall -Wextra -O2 -o randompi_mpi randompi_mpi.c -lm
