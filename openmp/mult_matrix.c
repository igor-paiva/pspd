#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// #define MATRIX_DIM 500
// #define NUM_THREADS 4

void print_matrix(int ** matrix, int dim) {
  for (int i = 0; i < dim; i++) {
    for (int j = 0; j < dim; j++) {
      if (j == dim - 1) {
        printf("%d", matrix[i][j]);
      } else {
        printf("%d ", matrix[i][j]);
      }
    }

    printf("\n");
  }
}

void mult_matrices(int ** m1, int ** m2, int ** result, int dim, int start, int end) {
  for (int i = start; i < end; i++) {
    for (int k = 0; k < dim; k++) {
      for (int j = 0; j < dim; j++) {
        result[i][j] += m1[i][k] * m2[k][j];
      }
    }
  }
}

void fill_matrices(int ** m1, int ** m2, int ** result, int dim) {
  for (int i = 0; i < dim; i++) {
    for (int j = 0; j < dim; j++) {
      m1[i][j] = rand();
      result[i][j] = 0;
    }
  }

  for (int i = 0; i < dim; i++) {
    for (int j = 0; j < dim; j++) {
      m2[i][j] = rand();

    }
  }
}

int ** allocate_matrix(int dim) {
  int ** m = (int **) malloc(dim * sizeof(int *));

  for (int i = 0; i < dim; i++) {
    m[i] = (int *) malloc (dim * sizeof(int));
  }

  return m;
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("Correct usage: program <MATRIX DIM> <NUM THREADS>\n");
    exit(-1);
  }

  int dim = atoi(argv[1]);
  int num_threads = atoi(argv[2]);

  int ** m1 = allocate_matrix(dim);
  int ** m2 = allocate_matrix(dim);
  int ** result = allocate_matrix(dim);

  srand(42); // to be predictable

  fill_matrices(m1, m2, result, dim);

  omp_set_num_threads(num_threads);

  // here it prints only 1
  // printf("Number of threads: %d\n", omp_get_num_threads());

  int slices[num_threads][2];

  for (int i = 0; i < num_threads; i++) {
    for (int j = 0; j < 2; j++) {
      slices[i][j] = ((i + j) * dim) / num_threads;
    }
  }

#pragma omp parallel
{
  // here it prints correctly
  printf("Number of threads: %d\n", omp_get_num_threads());

  #pragma omp for
    for (int i = 0; i < num_threads; i++) {
      mult_matrices(m1, m2, result, dim, slices[i][0], slices[i][1]);
      printf("thread num: %d\n", omp_get_thread_num());
    }
}

  return 0;
}