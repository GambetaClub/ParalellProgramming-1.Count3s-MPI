// Author: Mariano Augusto Jalif
// Course: CIS-363-82A
// Assignment: Count 3's
//
// I haven't neither shown nor copied code from/to anybody.
// I also state that I typed everything in this code on my own.
// I haven't watched anyone's code.
//
// This program counts the number of numbers 3 there are in a file
// The file should have the number of elements (numbers) in this file.
// This program uses MPI tool with a super computer and different processes
// with the goal of speeding the up the latency (total time it takes to run
// the program). For details about the resources used from the MPI, plese see
// bash file.

#include <stdio.h>
#include <fstream>
#include <iostream>
#include "mpi.h"

#define RootProcess 0

//Initializing variables.
int length;
int length_per_process;
int myStart;
int myCount = 0;
int globalCount = 0;
MPI_Status status;
int tag = 1;

int main(int argc, char **argv) {
  int myID, value, numProcs;
  MPI_Status status;

  // Getting MPI variables.
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
  MPI_Comm_rank(MPI_COMM_WORLD, &myID);

  // Reading file.
  FILE * fp;
  if((fp=fopen(argv[1], "r")) == NULL){
      std::cout << "Open failed." << std::endl;
      return EXIT_FAILURE;
    }
  // Setting the lenght of the list of numbers, 
  // which is the first number in the first line in the file.
  fscanf(fp, "%d", &length);

  length_per_process = length / numProcs;
  int *myArray = new int[length_per_process];
  int i; 

  // Read the data, distribute it among the various processes
  if (myID == RootProcess) {
    for(int p=0; p<numProcs-1; p++){
      for(i=0; i<length_per_process; i++){
        fscanf(fp, "%d", myArray+i);
      }
      MPI_Send(myArray, length_per_process, MPI_INT, p+1, tag, MPI_COMM_WORLD);
    }

    // Now reads the numbers
    for(i=0; i< length_per_process; i++) { 
      fscanf(fp, "%d", myArray+i);
    }

  }else { // If not the root process, other processes receive the data.
    MPI_Recv(myArray, length_per_process, MPI_INT, RootProcess, tag, MPI_COMM_WORLD, &status);
  }

  // Do the actual work

  // Counts 3's
  for (i = 0; i < length_per_process; i++) {
    if (myArray[i] == 3) {
      myCount++;
    }
  }

  // Sum sall the counts.
  MPI_Reduce(&myCount, &globalCount, 1, MPI_INT, MPI_SUM, RootProcess,
             MPI_COMM_WORLD);

  // Since I want to print this just once I just do it in the root process
  if (myID == RootProcess) {
    printf("Number of 3's: %d\n", globalCount);
  }
  
  // Wait for every process to finish.
  MPI_Finalize();

  // Deletes the dynamic allocated array.
  delete[] myArray;

  return EXIT_SUCCESS;
}