#include <float.h>

#include "f_graph.h"

	/* ==========  ========== ========== ========= */
	/*      Grafi - Libreria per la gestione       */
	/*      Autore - Daniele Cuomo N8601346        */
	/* ========== ========== ========== ========== */

/*-------- Visite: BFS, Dijkstra, A* ---------*/
int *bfs_visit( graph *G , int s , int t ){
	int v, i, u;
	int *pred = init_bfs( G->size );				/*Inizializzazione visita*/
	colour[s] = GREY;								/*Primo nodo da visitare*/
	q_list *queue = append( NULL , s );				/*Inserimento in coda del primo vertice*/
	q_list *head = queue;							/*Un elemento di una coda di grandezza 1 e' sia testa che coda*/
	while( head ){
		u = head->id;
		for( i=0 ; (v = adiac( G , u , i )) != -1 ; i++ ){
			if( !colour[v] ){						/*O(|Eu|), dove Eu e' l'insieme dei vertici adiacenti ad u*/
				colour[v] = GREY;
				pred[v] = head->id;
				queue = append( queue , v );
			}
		}
		colour[head->id] = BLACK;
		head = dequeue( queue );					/*Stacca la testa della lista*/
	}
	return free_and_exit( G->size , pred , true );	/*Elimina i dati usati per la visita e ritorna l'array dei predecessori*/
}

int *dijkstra( graph *G , int s , int t ){
	int i, u, v;
	int *pred = init_dij( G->size , s );			/*Inizializzazione visita*/
	while( Open->head ){
		u = extract_index( Open );					/*Estrae il massimo e ne ricava l'indice*/
		for( i=0 ; (v = adiac( G , u , i )) != -1 ; i++ )
				relax( G , u , v , pred );			/*Ricerca di un percorso migliore*/
	}
	return free_and_exit( G->size , pred , true );	/*Elimina i dati usati per la visita e ritorna l'array dei predecessori*/
}

int *astar( graph *G , int s , int t ){
	int *pred = init_astar( G , s , t );
	float cost;
	int x, y, i;
	while( Open->head ){
		x = extract_index( Open );								/*Estrae il massimo e ne ricava l'indice*/
		if( x == t )
			return free_and_exit( G->size , pred , true );
		for( i=0 ; (y = adiac( G , x , i )) != -1 ; i++ ){
			g[y] = g[x] + weight( G , x , y );
			cost = g[y] + h( G , y , t );
			if( (colour[y] == BLACK && cost < d[y]) || colour[y] == WHITE )
			/*(colour[y] == BLACK && cost < d[y]) sempre falso se h() e' consistente*/
				insert( Open , x , y , cost , pred );			/*Inserimento in coda della nuova stima*/
			else if( colour[y] == GREY && cost < d[y] )
				update_estimate( Open , x , y , cost , pred );	/*Aggiorna la stima presente in coda*/
			colour[x] = BLACK;
		}
	}
	return free_and_exit( G->size , pred , false );				/*Elimina i dati usati per la visita e ritorna l'array dei predecessori*/
}
/*--------------------------------------------*/

/*------ Funzioni chiamate dalle visite ------*/
void relax( graph *G , int u , int v , int *pred ){
	float key = d[u] + weight( G , u , v );
	if( d[v] > key ){
		d[v] = key;
		increase_key( Open , addr[v] , (void *)(d + v) );		/*addr[v] permette un accesso a tempo costante al nodo v*/
		pred[v] = u;
	}
}

int extract_index( queue *Q ){
	float *max = (float *)extract_max( Q );						/*Il valore puntato e' la miglior stima nella coda*/
	return max - d;												/*Il puntatore codifica l'indice del nodo corrispondente*/
}

void insert( queue *Q , int x , int y , float cost, int *pred ){
	colour[y] = GREY;
	pred[y] = x;
	d[y] = cost;
	insert_key( Q , (void *)(d + y) );							/*Inserimento nella coda dell'indirizzo dell'array con la stima di y*/
}

void update_estimate( queue *Q , int x , int y , float cost , int *pred ){
	pred[y] = x;
	d[y] = cost;
	increase_key( Q , addr[y] , (void *)(d + y) );				/*addr[y] permette un accesso a tempo costante al nodo y*/
}

	/*----- Inizializzazione visite -----*/
int *init_bfs( int n ){
	colour = (int *)calloc( n , sizeof(int) );					/*Inizializzazione a WHITE(0), (bianco == da-visitare)*/
	int *pred = (int *)malloc( n * sizeof(int) );
	int i;
	for( i=0 ; i<n ; i++ )
		pred[i] = -1;											/*-1 == nessun cammino*/
	return pred;
}

int *init_visit( int n ){
	d = (float *)malloc( n * sizeof(float) );
	addr = (void **)calloc( n , sizeof(void *) );
	Open = create_heap( p );
	return (int *)malloc( n * sizeof(int) );					/*Allocazione array predecessori*/
}

int *init_dij( int n , int s ){
	int *pred = init_visit( n );
	d[s] = 0;
	int u;
	for( u=0 ; u<n ; u++ ){
		pred[u] = -1;											/*-1 == nessun cammino*/
		if( u != s )
			d[u] = FLT_MAX;										/*FLT_MAX == infinito*/
		insert_key( Open , (void *)(d + u) );
	}
	return pred;
}

int *init_astar( graph *G , int s , int t ){
	int u, n = G->size;
	int *pred = init_visit( n );
	colour = (int *)calloc( n , sizeof(int) );					/*Inizializzazione a WHITE(0), (bianco == da-visitare)*/
	g = (float *)malloc( n * sizeof(float) );
	for( u=0 ; u<n ; u++ ){
		pred[u] = -1;											/*-1 == nessun cammino*/
		d[u] = FLT_MAX;											/*FLT_MAX == infinito*/
	}
	d[s] = h( G , s , t );										/*Stima euristica da s a t*/
	g[s] = 0;
	insert_key( Open , (void *)(d + s) );
	return pred;
}
/*--------------------------------------------*/

/*----------- Funzioni di supporto -----------*/
int get_random( int a , int b ){
	return a + rand()%(b - a + 1);		/*Numero pseudo-random compreso tra a e b*/
}

void swap( void **a , void **b ){
	void *tmp = *a;
	*a = *b;
	*b = tmp;
}

bool is_occ( double p ){
	double d = (double)rand() / (double)RAND_MAX;
	return d <= p;						/*Vero con probabilita' p:[0,1]*/
}

int *free_and_exit( int GSIZE , int *pred , bool found ){
	if( colour )	free( colour );
	if( d )			free( d );
	if( g )			free( g );
	if( addr ){							/*Deallocazione albero sfruttando l'array 'addr'*/
		int i;
		for( i=0 ; i<GSIZE ; i++ ){
			if( addr[i] )
				free( addr[i] );
		}
		free( addr );
	}
	colour = NULL;
	d = g = NULL; addr = NULL;
	if( !found ){
		free( pred );
		pred = NULL;
	}
	return pred;
}
/*--------------------------------------------*/

/*------ Gestione stack dedicati a BFS -------*/
q_list *append( q_list *queue , int v ){
	q_list *new_head = (q_list *)malloc(sizeof(q_list));
	new_head->id = v;
	new_head->next = queue;
	return new_head;
}

q_list *dequeue( q_list *Q ){
	if( Q->next && Q->next->next )
		return dequeue( Q->next );
	else if( !Q->next ){				/*queue == head*/
		free( Q );
		return NULL;
	}
	else{								/*Q->next && !Q->next->next*/
		free( Q->next );
		Q->next = NULL;
		return Q;
	}
}
/*--------------------------------------------*/

/*---------- Gestione array 'addr' -----------*/
void set_addr( void *key , void *e ){
	int k = (key - (void *)d) / sizeof(float);		/*Si ricava l'indice*/
	addr[k] = e;									/*Assegnazione del nodo alla locazione corrispondente*/
}

void swap_addr( void *addr_i , void *i , void *addr_j , void *j ){
	int ind_i = (i - (void *)d) / sizeof(float);	/*Si ricava l'indice sfruttando l'array globale 'd'*/
	int ind_j = (j - (void *)d) / sizeof(float);
	void *tmp = addr[ind_i];						/*Scambio dei nodi sull'array globale 'addr'*/
	addr[ind_i] = addr_j;
	addr[ind_j] = tmp;
}
/*--------------------------------------------*/