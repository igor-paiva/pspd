#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include <mpi.h>
#include <sys/types.h>
#include <unistd.h>
#include "float.h"

// #define VECTOR_SIZE 100

float * generate_vector(int size) {
  float * v;

  v = (float *) calloc (size, sizeof(float));

  for (int i = 0; i < size; i++) {
    v[i] = sqrt((i - (size / 2.0)) * (i - (size / 2.0)));
  }

  return v;
}

int main(int argc, char ** argv) {
  if (argc < 2) {
    printf("Por favor forneça o tamanho do vetor\n");
    exit(0);
  }

  MPI_Init(&argc, &argv);

  int process_rank, comm_size;

  MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_size);

  float min = FLT_MAX;
  float max = FLT_MIN;
  int list_size = atoi(argv[1]);
  int num_slices = comm_size - 1;

  if (process_rank == 0) {
    float * list = generate_vector(list_size);

    int slices[num_slices][2];

    for (int i = 0; i < num_slices; i++) {
      for (int j = 0; j < 2; j++) {
        slices[i][j] = ((i + j) * list_size) / num_slices;
      }
    }

    for (int i = 0; i < num_slices; i++) {
      MPI_Send(&(slices[i]), 2, MPI_INT, i + 1, 0, MPI_COMM_WORLD);

      MPI_Send(list, list_size, MPI_FLOAT, i + 1, 0, MPI_COMM_WORLD);
    }

    for (int i = 1; i < comm_size; i++) {
      float received[2];

      MPI_Recv(received, 2, MPI_FLOAT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

      for (int j = 0; j < 2; j++) {
        if (received[j] < min) min = received[j];

        if (received[j] > max) max = received[j];
      }
    }

    printf("Min: %f\n", min);
    printf("Max: %f\n", max);

  } else {
    int slice[2];
    float local_answer[2];
    float * list = (float *) malloc (list_size * sizeof(float));

    MPI_Recv(slice, 2, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    MPI_Recv(list, list_size, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    local_answer[0] = list[0];
    local_answer[1] = list[0];

    for (int i = slice[0]; i < slice[1]; i++) {
      if (list[i] < local_answer[0]) local_answer[0] = list[i];

      if (list[i] > local_answer[1]) local_answer[1] = list[i];
    }

    MPI_Send(local_answer, 2, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
  }

  MPI_Finalize();

  return 0;
}