#include <stdlib.h>

#include "qarray.h"

	/* ========== =========== ========== ========= */
	/*      Autore - Daniele Cuomo N8601346        */
	/* ========== =========== ========== ========= */

void *getval_A( void *addr ){
	if( addr )
		return *(void **)addr;
	return NULL;
}

void setval_A( void *dest , void *srg ){
	*(void **)dest = allocate( dim , srg );
}

void *sx_A( queue *A , void *addr ){
	int i = (int)(addr - A->head + SZ_PNT);	/*#define SZ_PNT sizeof(void *)*/
    i = 2*i;
    if( (i/SZ_PNT) > A->heapsize )
    	return NULL;
	addr = A->head + i - SZ_PNT;
	return addr;
}

void *dx_A( queue *A , void *addr ){
	int i = (int)(addr - A->head + SZ_PNT);
	i = 2*i;								 /*2i + 1*/
	if( ((i+SZ_PNT)/SZ_PNT) > A->heapsize )	 /*i > heapsize*/
		return NULL;
	addr = i + A->head;
	return addr;
}

void *ft_A( queue *A , void *addr ){
	int i = (int)(addr - A->head + SZ_PNT);
	i = i/(2*SZ_PNT);						 /*tetto_basso(i/2)*/
	if( !i )
		return NULL;
	addr = i*SZ_PNT + A->head - SZ_PNT;
	return addr;
}

void *getaddr_A( void *addr , int path ){
	path--;
	addr = (addr + path*SZ_PNT);
	return addr;
}

void swap_A( void *addr_i , void *addr_j ){
	void *tmp = *(void **)addr_i;
	void **i = (void **)addr_i;
	void **j = (void **)addr_j;
	*i = *j;
	*j = tmp;
}

void *resize_A( queue *A , int redim ){
	A->heapsize += redim;
	return realloc( A->head , (A->heapsize * SZ_PNT) );
}

void *deallocate_A( queue *A , void *addr ){
	if( t_free )
		t_free( getval_A( addr ) );
	return resize_A( A , -1 );
}

void *alloc_min_A( queue *A ){
	A->head = resize_A( A , 1 );
	void *last = getaddr_A( A->head , A->heapsize );
	void *min = get_min( A->p );
	setval_A( last , min );
	free( min );
	return last;
}

void *createq_A( int n ){
	void *A = malloc(n * SZ_PNT);
	int i;
	void *dest;
	int j;
	for( i=0 ; i<n ; i++ ){
		dest = (A + i*SZ_PNT);
		setval_A( dest , get_random() );
	}
	return (void *)A;
}

void buildh_A( queue *A ){
	int i, n = A->heapsize;
	void *addr;
	for( i=(n/2)-1 ; i>=0 ; i-- ){
		addr = (A->head + i*SZ_PNT);
		heapify( A , addr );
	}
}

