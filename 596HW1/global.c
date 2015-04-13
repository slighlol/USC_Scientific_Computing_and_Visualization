/*
 * Chieh Wen Chang
 * Assignment 1
 *
 */

#include "mpi.h"
#include <stdio.h>
int nprocs;
int myid;
MPI_Status status;


double global_sum(double partial){
		
		double mydone = partial;
		double hisdone;
		int partner;
		int bitmask;
		
	for(bitmask = 1; bitmask < nprocs; bitmask = bitmask*2){
		partner = myid ^ bitmask;
	
		MPI_Send(&mydone,1, 
				MPI_DOUBLE, 
				partner,
				bitmask,
				MPI_COMM_WORLD);
		
		MPI_Recv(&hisdone,1, 
				MPI_DOUBLE, 
				partner,
				bitmask,
				MPI_COMM_WORLD,
				&status);

		mydone = mydone + hisdone;
	}
	
	return mydone;
}

int main(int argc, char *argv[]){
	double partial, sum, avg;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	MPI_Comm_size(MPI_COMM_WORLD,&nprocs);
	
	partial = (double)myid;
	printf("Node %d has %le\n", myid, partial);
	
	sum = global_sum(partial);
	
	if (myid == 0) {
		avg = sum/nprocs;
		printf("Global sum = %le\n", avg);
	}
	
	MPI_Finalize();
	
	return 0;
}