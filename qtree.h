#ifndef QTREE_H_INCLUDED
#define QTREE_H_INCLUDED

#include "f_heap.h"

	/* ==========  ========== ========== ========= */
	/*      Code - Libreria per la gestione        */
	/*      Autore - Daniele Cuomo N8601346        */
	/* ========== ========== ========== ========== */
	
void *getval_T( void * );
/*Ritorna l'elemento conservato dal nodo*/
void setval_T( void * , void * );
/*Assegna un elemento al campo 'info' nodo*/
void *sx_T( queue * , void * );
/*Ritorna il figlio sinistro del nodo corrente*/
void *dx_T( queue * , void * );
/*Ritorna il figlio destro del nodo corrente*/
void *ft_T( queue * , void * );
/*Ritorna il padre del nodo corrente*/
void *getaddr_T( void * , int );
/*Ritorna l'indirizzo del nodo i-esimo sfruttando la codifica del lato*/
void swap_T( void * , void * );
/*Inverte i campi 'info' dei due nodi in input*/
void *resize_T( queue * , int );
/*Modifica il parametro 'heapsize' in base all'input*/
void *remove_T( queue * , void * );
/*Rimuove dalla coda il nodo in input*/
void *deallocate_T( queue * , void * );
/*Rimuove ed elimina il nodo in input dalla coda*/
void *alloc_min_T( queue * );
/*Inserisce nella coda un nuovo elemento con priorita' minima*/

#endif
