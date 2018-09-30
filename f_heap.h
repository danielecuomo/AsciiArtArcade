#ifndef F_HEAP_H_INCLUDED
#define F_HEAP_H_INCLUDED

#include <stdbool.h>
#include <float.h>

	/* ==========  ========== ========== ========= */
	/*      Code - Libreria per la gestione        */
	/*      Autore - Daniele Cuomo N8601346        */
	/* ========== ========== ========== ========== */

#define SZ_PNT sizeof(void *)	/*Grandezza utile ad allocare un array di puntatori*/

typedef enum { MAX_P , MIN_P } prior;

#define SIDE_SX 0
#define SIDE_DX 1

	/* ==========  ========== ========== ========= */
	/*      Code - Libreria per la gestione        */
	/*      Autore - Daniele Cuomo N8601346        */
	/* ========== ========== ========== ========== */
	
typedef struct priority_queue{
	void *head;
	int p;						/*Priorita': { MIN_P , MAX_P }*/
	int heapsize;
}queue;
/*---------------- Coda Heap -----------------*/

typedef struct heap_node{
	void *info;
	int side;                 	/*Codifica del lato: { 0 , 1 }*/
	struct heap_node *father;
	struct heap_node *left;
	struct heap_node *right;
}node;
/*------------- Nodo per alberi --------------*/

typedef int (* LENGHT)( void * );
/*Ritorna la lunghezza di un elemento (es: strlen)*/
typedef void *(* GETVAL)( void * );
/*Ritorna il valore di posto i (per array) e il campo info (per nodi)*/
typedef void (* SETVAL)( void * , void * );
/*Assegna un valore alla locazione data in input*/
typedef void *(* LEFT)( queue * , void * );
/*Figlio sinistro dell'elemento corrente*/
typedef void *(* RIGHT)( queue * , void * );
/*Figlio destro dell'elemento corrente*/
typedef void *(* FATHER)( queue * , void * );
/*Padre dell'elemento corrente*/
typedef void *(* GETADDR)( void * , int );
/*Ottiene un indirizzo specifico della coda*/
typedef void (* SWAPH)( void * , void * );
/*Inverte i valori "puntati" dai due parametri*/
typedef void *(* RESIZE)( queue * , int );
/*Ridimensiona la coda*/
typedef void *(* REMOVE_H)( queue * , void * );
/*Stacca un elemento dalla coda*/
typedef void (* T_FREE)( void * );
/*Elimina l'elemento allocato dinamicamente*/
typedef void *(* DEALLOCATE)( queue * , void * );
/*Ritorna la coda dopo avere eliminato l'ultimo elemento*/
typedef void *(* ALL_MIN)( queue * );
/*Ritorna l'indice/indirizzo di un nuovo elemento minimo*/
typedef int (* COMPARE)( void * , void * , int );
/*Ritorna 1 se a ha priorita' su b, -1 se b ha priorità su a, 0 altrimenti*/
typedef void *(* RANDOM)( void );
/*Alloca un elemento random*/

/*------- Funzioni per visite dei grafi ------*/
typedef void (* SWAP_OTHER)( void * , void * , void * , void * );
typedef void (* SET_OTHER)( void * , void * );
/*--------------------------------------------*/

GETVAL get_val;
SETVAL set_val;
LEFT SX;
RIGHT DX;
FATHER FT;
GETADDR getaddr;
SWAPH swaph;
ALL_MIN alloc_min;
REMOVE_H remove_h;
DEALLOCATE deallocate;
COMPARE cmp;
LENGHT lenght;
T_FREE t_free;
int dim;
prior p;

SWAP_OTHER swap_other;
SET_OTHER set_other;

typedef void (* PRINT)( void * );
/*Visualizza a video un input di qualsiasi tipo*/

void heapify( queue * , void * );
/*Ordinamento con visita ricorsiva in profondita'*/
void *extract_max( queue * );
/*Stacca dalla coda il nodo con priorita' massima garantendo l'ordinamento*/
void decrease_key( queue * , void * , void * );
/*Modifica la chiave del nodo in input, con una chiave piu' 'piccola', garantendo l'ordinamento*/
void increase_key( queue * , void * , void * );
/*Modifica la chiave del nodo in input, con una chiave piu' 'grande', garantendo l'ordinamento*/
void insert_key( queue * , void * );
/*Inserisce una nuova chiave nella coda, garantendo l'ordinamento*/
void delete_key( queue * , void * );
/*Elimina una chiave dalla coda, garantendo l'ordinamento*/
queue *create_heap( int );
/*Crea una coda vuota*/
bool verify_heap( queue * , void * , void * );
/*Verifica che la coda in input rispetti le proprieta' di ordinamento*/
void print_queue( queue * , void * , PRINT );
/*Stampa a video della coda*/
/*--------------------------------------------*/

/*----------- Funzioni di supporto -----------*/
void copy_data( void * , void * , int , int );
/*Copia un dato sorgente in una destinazione byte per byte*/
void *allocate( int , void * );
/*Alloca un elemento di qualsiasi tipo*/
void *get_min( int );
/*Ritorna il valore minimo in base alla priorità.*/
void *get_max( int );
/*Ritorna il valore massimo in base alla priorità.*/
void *search_key( queue * , int );
/*Ritorna l'indirizzo di un elemento della coda, se esiste*/
node *allocate_node( node * , int );
/*Alloca spazio per un nodo, gli assegna il padre e la codifica del lato*/
/*--------------------------------------------*/

/*------------------ :NEW: -------------------*/
int float_cmp( void * , void * , int );
/*Ritorna 1 se il 1° elemento e' piu' grande del secondo, 0 se sono uguali, -1 altrimenti*/
void set_heap_callbacks( void );
/*Imposta le callback per le funnzioni generiche*/
/*--------------------------------------------*/

#endif
