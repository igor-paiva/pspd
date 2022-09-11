#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <omp.h>
#include "limits.h"

#define VECTOR_SIZE 10000000

int main(void) {
  srand(42);

  int min = INT_MAX, max = INT_MIN;
  int * v = malloc(VECTOR_SIZE * sizeof(int));

  for (int i = 0; i < VECTOR_SIZE; i++) {
    v[i] = rand();
    // printf("%d ", v[i]);
  }

  // printf("\n");

  // copyin(name, name2) copies the values of main thread vars to the threadprivate vars

#pragma omp parallel for reduction(min : min) reduction(max : max)
  for (int i = 0; i < VECTOR_SIZE; i++) {
    if (v[i] < min) min = v[i];

    if (v[i] > max) max = v[i];
  }

  printf("Min: %d\n", min);
  printf("Max: %d\n", max);

  return 0;
}