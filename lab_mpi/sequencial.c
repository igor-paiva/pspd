#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include "float.h"

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
    printf("Você deve inserir o tamanho do vetor\n");
    exit(0);
  }

  int size = atoi(argv[1]);
  float * list = generate_vector(size);
  float min = FLT_MAX;
  float max = FLT_MIN;

  for (int i = 0; i < size + 1; i++) {
    if (list[i] < min) min = list[i];

    if (list[i] > max) max = list[i];
  }

  printf("Min: %f\n", min);
  printf("Max: %f\n", max);

  return 0;
}
