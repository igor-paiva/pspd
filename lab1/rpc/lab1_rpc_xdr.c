/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "lab1_rpc.h"
#include "limits.h"

bool_t
xdr_params (XDR *xdrs, params *objp)
{

	return xdr_array(xdrs, (char**)(&(objp->v)), &(objp->n), INT_MAX, sizeof(float), (xdrproc_t)xdr_float);

	// register int32_t *buf;

	//  if (!xdr_pointer (xdrs, (char **)&objp->v, sizeof (float), (xdrproc_t) xdr_float))
		//  return FALSE;
	//  if (!xdr_int (xdrs, &objp->n))
	// 	 return FALSE;
	//  return TRUE;
}

bool_t
xdr_result (XDR *xdrs, result *objp)
{
	register int32_t *buf;

	 if (!xdr_float (xdrs, &objp->min))
		 return FALSE;
	 if (!xdr_float (xdrs, &objp->max))
		 return FALSE;
	return TRUE;
}
