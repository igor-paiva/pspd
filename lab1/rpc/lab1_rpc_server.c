/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "lab1_rpc.h"

result *
min_max_100_svc(params *argp, struct svc_req *rqstp)
{
	static result  result;

	result.min = argp->v[0];
	result.max = argp->v[0];

	for (int i = 1; i < argp->n; i++) {
    if (argp->v[i] < result.min)
      result.min = argp->v[i];

    if (argp->v[i] > result.max)
      result.max = argp->v[i];
  }

	return &result;
}
