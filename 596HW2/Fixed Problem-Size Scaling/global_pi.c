/*
 * Chieh Wen Chang
 * Assignment 2
 * 
 * Fixed Problem-Sized Scaling
 */

#include "mpi.h"
#include <stdio.h>
int nprocs;
int myid;
MPI_Status status;
#define NBIN 10000000


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
	
	double partial;
	double sum = 0.0;
	double x;
	double pi;
	int i;
	double start, end;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	MPI_Comm_size(MPI_COMM_WORLD,&nprocs);
	
	int NBINp = 10000000*nprocs;
	double step = 1.0/NBIN;
	
	start = MPI_Wtime();
	
	partial = (double)myid;
	
	for(i=myid; i<NBIN; i = i+nprocs){
		x = (i+0.5)*step;
		partial = partial + 4.0/(1.0+x*x);
	}
	
	partial = partial * step;
	pi = global_sum(partial);
	
	end = MPI_Wtime();
	
	if (myid == 0){
		printf("Pi = %f   Used Time = %f\n", pi, (end-start));
	}
	
	MPI_Finalize();
	
	return 0;
}