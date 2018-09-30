#ifndef F_GRAPH_H_INCLUDED
#define F_GRAPH_H_INCLUDED

#include <stdbool.h>
#include <stdlib.h>

#include "f_heap.h"

#define WHITE 0
#define GREY -1
#define BLACK 1

	/* ==========  ========== ========== ========= */
	/*      Grafi - Libreria per la gestione       */
	/*      Autore - Daniele Cuomo N8601346        */
	/* ========== ========== ========== ========== */

typedef struct G_graph{
	int size;
	int l;			/*Larghezza, utile per i labirinti*/
	int h;			/*Altezza, utile per i labirinti*/
	void **nodes;	/* list ** || float ** || maze_t ** */
}graph;

/*-------- Funzioni per grafi generici -------*/
typedef int (* ADIAC)( graph * , int , int );
/*Restituisce l'i-esimo vertice adiacente*/
typedef float (* WEIGHT)( graph * , int , int );
/*Ritorna il peso dell'arco: ( input1 , input2 )*/
typedef float (* EURISTIC)( graph * , int , int );
/*Stima euristica, dipende dal grafo. Esempio: distanza Euclidea*/
/*--------------------------------------------*/

ADIAC 		adiac;
WEIGHT		weight;
EURISTIC	h;

/*------- Strutture dati per le visite -------*/
int *colour;	/*Array per tenere traccia dei nodi visitati (BFS, A*)*/
float *d;		/*Stima delle distanze (Dijkstra, A*)*/
float *g;		/*Peso totale del miglior percorso trovato (A*)*/
void **addr;	/*Associa il vertice i-esimo al nodo nello heap, permette di accedere a tempo costante alla locazione della coda (A*)*/
queue *Open;	/*Coda di priorita' (Dijkstra, A*)*/
/*--------------------------------------------*/

/*-------- Visite: BFS, Dijkstra, A* ---------*/
int *bfs_visit( graph * , int , int );
/*Visita in ampiezza: Breadth First Search*/
int *dijkstra( graph * , int , int );
/*Visita di grafi a pesi non negativi*/
int *astar( graph * , int , int );
/*Algoritmo di visita derivante dalla BFS*/
/*--------------------------------------------*/

/*------ Funzioni chiamate dalle visite ------*/
void relax( graph * , int , int , int * );
/*Verifica se e' possibile ottenere un percorso migliore, algoritmo di Dijkstra*/
int extract_index( queue * );
/*Risale all'indice del vertice sfruttando le proprietà dell'array*/
void insert( queue * , int , int , float , int * );
/*(re)Inserisce la nuova stima*/
void update_estimate( queue * , int , int , float , int * );
/*Aggiorna la stima migliorata*/
	/*Inizializzazione visite*/
int *init_bfs( int );
int *init_visit( int );
int *init_dij( int , int );
int *init_astar( graph * , int , int );
/*--------------------------------------------*/

/*----------- Funzioni di supporto -----------*/
int get_random( int , int );
/*Ritorna un numero pseudo-random compreso tra a e b*/
void swap( void ** , void ** );
/*Scambia i valori puntati dai parametri*/
bool is_occ( double p );
/*Ritorna vero con probabilita' p:[0,1]*/
int *free_and_exit( int , int * , bool );
/*Dealloca gli array per le visite e ritorna l'array dei predecessori*/
/*--------------------------------------------*/

/*------ Gestione stack dedicati a BFS -------*/
typedef struct queue_list{
	int id;
	struct queue_list *next;
}q_list;

q_list *append( q_list * , int );
/*Inserimento in coda*/
q_list *dequeue( q_list * );
/*Elimina la coda della lista*/
/*--------------------------------------------*/

/*---------- Gestione array 'addr' -----------*/
void set_addr( void * , void * );
/*Assegna il nodo della coda alla locazione corrispondente, calcolandone l'indice*/
void swap_addr( void * , void * , void * , void * );
/*Assegna i nodi della coda ai corrispondenti indici secondo la nuova configurazione*/
/*--------------------------------------------*/

#endif