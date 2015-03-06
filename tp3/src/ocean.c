
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stddef.h>
#include <mpi.h>
#include "ocean.h"

/* constants for the ocean */
#define N 40
#define M 20
#define WALL 100
#define STEP 150000
#define RATIO 10

int main (int argc, char ** argv)
{
  MPI_Datatype MPI_FISH;

  int i,rank,n;

  // Description of the type fish_t
  int fish_lengths[2];
  fish_lengths[0] = sizeof(char);
  fish_lengths[1] = sizeof(char);

  MPI_Aint fish_offsets[2];
  fish_offsets[0] = offsetof(fish_t,type);
  fish_offsets[1] = offsetof(fish_t,moved);

  MPI_Datatype fish_types[2] = {MPI_CHAR, MPI_CHAR};

  fish_t * ocean = (fish_t *)malloc(N*M*sizeof(fish_t));
  // Receiver
  fish_t * sea = (fish_t *)malloc(N*M*sizeof(fish_t));

  // Initialization
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &n);

  // Creation of the type MPI_FISH
  MPI_Type_create_struct(2, fish_lengths, fish_offsets, fish_types,&MPI_FISH);  
  MPI_Type_commit(&MPI_FISH);

  // Initialization and display of the ocean
  if (rank == 0) {
    init_ocean(ocean, N, M, RATIO);
    printf(CLS "\n");
    display_ocean(ocean, N, M);
  }

  MPI_Scatter(ocean, N*M/n, MPI_FISH, sea, N*M/n, MPI_FISH, 0, MPI_COMM_WORLD);

  for (i = 0; i < WALL; i++) {
    usleep(STEP);
    printf(CLS "\n");
    update_ocean(ocean, N, M);

    if (rank == 0) display_ocean(ocean, N, M);
  }

  MPI_Finalize();
  free(ocean);
  free(sea);
  return 0;
} /* main */

