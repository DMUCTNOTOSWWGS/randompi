(all):
	gcc -pedantic -Wall -Wextra -O2 -o randompi randompi.c -lm
parallel:
	mpicc -pedantic -Wall -Wextra -O2 -o randompi_mpi randompi_mpi.c -lm
clean:
	if [ -e randompi ]; \
		then rm randompi; fi
	if [ -e randompi_mpi ]; \
		then rm randompi_mpi; fi
