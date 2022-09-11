#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <omp.h>

#define VECTOR_SIZE 10000000

// int main(void) {
//   srand(42);

//   int num_zeros = 0;
//   int * v = malloc(VECTOR_SIZE * sizeof(int));

//   for (int i = 0; i < VECTOR_SIZE; i++) {
//     if (rand() % 11 == 0) {
//       v[i] = 0;
//     } else {
//       v[i] = rand();
//     }
//   }

// // shared(v, num_zeros)
// #pragma omp parallel
//   {
//     int qtd = 0;
//     #pragma omp for
//     for (int i = 0; i < VECTOR_SIZE; i++) {
//       if (v[i]) {
//         qtd++;
//       }
//     }

//     #pragma omp critical (name)
//     #pragma omp critical
//       num_zeros += qtd;
//   }

//   printf("Num zeros: %d\n", num_zeros);

//   return 0;
// }

int main(void) {
  srand(42);

  int num_zeros = 0;
  int * v = malloc(VECTOR_SIZE * sizeof(int));

  for (int i = 0; i < VECTOR_SIZE; i++) {
    if (rand() % 11 == 0) {
      v[i] = 0;
    } else {
      v[i] = rand();
    }
  }

#pragma omp parallel for reduction(+ : num_zeros)
  for (int i = 0; i < VECTOR_SIZE; i++)
    if (v[i] == 0) num_zeros++;

  printf("Num zeros: %d\n", num_zeros);

  return 0;
}
