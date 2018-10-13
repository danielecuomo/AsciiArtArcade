#ifndef QARRAY_H_INCLUDED
#define QARRAY_H_INCLUDED
#include <stdbool.h>

#include "pdef.h" /*queue * ; #define*/

	/* ========== =========== ========== ========= */
	/*      Autore - Daniele Cuomo N8601346        */
	/* ========== =========== ========== ========= */
void *getval_A( void * );

void setval_A( void * , void * );

void *sx_A( queue * , void * );

void *dx_A( queue * , void * );

void *ft_A( queue * , void * );

void *getaddr_A( void * , int );

void swap_A( void * , void * );

void *resize_A( queue * , int );

void *deallocate_A( queue * , void * );

void *alloc_min_A( queue * );

void *createq_A( int );

void buildh_A( queue * );

#endif

