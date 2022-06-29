/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "lab1_rpc_2_threads.h"

#include <time.h>

#define VECTOR_SIZE 500000

float f_aleat() {
  return ((float) rand() / (float) RAND_MAX) * 100;
}

void fill_array(float * array, long int size) {
  srand((unsigned int) time(NULL));

  for (int i = 0; i < size; i++) {
    float random_f = f_aleat();

		// sqrt was not working, I've tried to add the -lm flag in the Makefile but it didn't work either
    array[i] = (i - (random_f / 2)) * (i - (random_f / 2));
  }
}

void
prog_100(char *host)
{
	CLIENT *clnt;
	result  *result_1;
	params  min_max_100_arg;

	min_max_100_arg.n = VECTOR_SIZE;
	min_max_100_arg.v = (float *) malloc(VECTOR_SIZE * sizeof(float));

	fill_array(min_max_100_arg.v, min_max_100_arg.n);

#ifndef	DEBUG
	clnt = clnt_create (host, PROG, VERSAO, "tcp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */

	result_1 = min_max_100(&min_max_100_arg, clnt);
	if (result_1 == (result *) NULL) {
		clnt_perror (clnt, "call failed");
	}

	printf("Min: %f\n", result_1->min);
	printf("Max: %f\n", result_1->max);

#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}


int
main (int argc, char *argv[])
{
	char *host;

	if (argc < 2) {
		printf ("usage: %s server_host\n", argv[0]);
		exit (1);
	}
	host = argv[1];
	prog_100 (host);
exit (0);
}
