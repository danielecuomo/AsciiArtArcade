#include <stdlib.h>
#include <math.h>

#include "qtree.h"

	/* ==========  ========== ========== ========= */
	/*      Code - Libreria per la gestione        */
	/*      Autore - Daniele Cuomo N8601346        */
	/* ========== ========== ========== ========== */
	
void *getval_T( void *addr ){
	if( addr ){
		node *curr = (node *)addr;
		return curr->info;
	}
	return NULL;
}

void setval_T( void *addr , void *srg ){
	node *dest = (node *)addr;
	dest->info = srg;
}

void *sx_T( queue *T , void *addr ){
	node *curr = (node *)addr;
	return (void *)curr->left;
}

void *dx_T( queue *T , void *addr ){
	node *curr = (node *)addr;
	return (void *)curr->right;
}

void *ft_T( queue *T , void *addr ){
	node *curr = (node *)addr;
	return (void *)curr->father;
}

void *getaddr_T( void *addr , int path ){
	node *curr = (node *)addr;
	int mask = 1; 								/*000...001*/
	mask <<= (int)((log(path)/log(2)) + 1);		/*00010...0*/
	while( !(mask & path) )
		mask >>= 1;
	mask >>= 1;									/*maschera pronta*/
	for( ; mask>0 ; mask>>=1 ){
		if( mask & path )
			curr = curr->right;
		else
			curr = curr->left;
	}
	return (void *)curr;
}

void swap_T( void *addr_i , void *addr_j ){
	node *curr_i = (node *)addr_i;
	node *curr_j = (node *)addr_j;
	void *tmp = curr_i->info;
	curr_i->info = curr_j->info;
	curr_j->info = tmp;
}

void *resize_T( queue *T , int redim ){
	T->heapsize += redim;
	return T->head;
}

void *remove_T( queue *T , void *addr ){
	node *f;
	if( (f = (node *)ft_T(T->head , addr)) ){
		if( f->left == addr )
			f->left = NULL;
		else
			f->right = NULL;
	}
	else
		T->head = NULL;
	return resize_T( T , -1 );
}

void *deallocate_T( queue *T , void *addr ){
	T->head = remove_T( T , addr );				/*1° passo: staccare l'elemento dalla coda*/
	node *tmp = (node *)addr;
	if( t_free )
		t_free( tmp->info );					/*2° passo: eliminazione dell'elemento*/
	free( addr );								/*3° passo: eliminazione del nodo*/
	return T->head;
}

void *alloc_min_T( queue *T ){
	T->head = resize_T( T , 1 );
	if( !T->head ){
		T->head = (void *)allocate_node( NULL , SIDE_DX );
		void *min = get_min( T->p );
		setval_T( T->head , min );
		return T->head;
	}
	node *curr = (node *)T->head;
	int path = T->heapsize;						/*E' necessario ricavarsi l'ultimo nodo*/
	int mask = 1;
	mask <<= (int)((log(path)/log(2)) + 1);
	while( !(mask & path) )
		mask >>= 1;
	mask >>= 1;
	for( ; mask>1 ; mask>>=1 ){
		if( mask & path )
			curr = curr->right;
		else
			curr = curr->left;
	}
	if( mask & path ){
		curr->right = allocate_node( curr , SIDE_DX );
		curr = curr->right;
	}
	else{
		curr->left = allocate_node( curr , SIDE_SX );
		curr = curr->left;
	}
	void *min = get_min( T->p );
	setval_T( (void *)curr , min );
	return (void *)curr;
}