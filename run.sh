#!/bin/bash

set -e

function run_mpi() {

	if [ -z "$1" ]
	then
		echo You must give the number of CPUs
		exit 1
	fi

	if ! [ -e randompi_mpi ]
	then
		make parallel
	fi

	if [ "$1" -gt "1000" ]
	then
		echo -ne "Are you sure you mean to start $1 threads? "
		read yes
		case $yes in
			[yY]*)
				;;
			*)
				exit 0
				;;
		esac
	fi
	mpirun -np $1 randompi_mpi $2

}

function run_serial() {

	if ! [ -e randompi ]
	then
		make
	fi

	./randompi $@

}

if [[ "$1" == "parallel" ]]
then
	echo Running in parallel
	run_mpi "$2" "$3"
else
	echo Running in serial
	run_serial $@
fi
