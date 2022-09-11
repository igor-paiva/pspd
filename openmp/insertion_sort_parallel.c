#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <omp.h>

#define VECTOR_SIZE 100000

#define less(A,B) ((A) < (B))
#define lesseq(A,B) ((A) <= (B))
#define exch(A,B) { int t; t=A;A=B;B=t; }
#define cmpexch(A,B) { if (less(B,A)) exch(A,B); }

int partition(int * v, int l, int r) {
  int j = l;
  int c = v[r];

  for (int k = l; k < r; k++) {
    if (lesseq(v[k], c)) {
      exch(v[k], v[j]);
      j++;
    }
  }

  exch(v[j], v[r]);

  return j;
}

void insertion_sort(int * v, int l, int r) {
  for(int i = r; i > l; i--) {
    cmpexch(v[i - 1], v[i]);
  }

  for(int i = l + 2; i <= r; i++) {
    int j = i;
    int tmp = v[j];

    while(less(tmp, v[j - 1])) {
      v[j] = v[j - 1];
      j--;
    }

    v[j] = tmp;
  }
}

int main(void) {
  int * v = malloc(VECTOR_SIZE * sizeof(int));

  for (int i = 0; i < VECTOR_SIZE; i++) {
    v[i] = rand();
  }

  int l = 0;
  int r = VECTOR_SIZE - 1;


  int pivo_idx = partition(v, 0, VECTOR_SIZE - 1);

#pragma omp parallel sections
{
  #pragma omp section
  {
    insertion_sort(v, l, pivo_idx - 1);
  }
  #pragma omp section
  {
    insertion_sort(v, pivo_idx + 1, r);
  }
}

  // printf("Ordenado\n");
  // for (int i = 0; i < VECTOR_SIZE; i++) {
  //   printf("%d ", v[i]);
  // }
  // printf("\n");

  return 0;
}
