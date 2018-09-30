#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "f_heap.h"

	/* ==========  ========== ========== ========= */
	/*      Code - Libreria per la gestione        */
	/*      Autore - Daniele Cuomo N8601346        */
	/* ========== ========== ========== ========== */
	
void heapify( queue *Q , void *i ){
	void *max;
	void *l = SX( Q , i );						/*'SX' e 'DX' ritornano il puntatore al nodo*/
	void *r = DX( Q , i );
	void *l_val = get_val( l );					/*La funzione generica 'get_val' restituisce il valore associato al nodo*/
	void *r_val = get_val( r );
	void *i_val = get_val( i );
	if( l && (cmp(l_val , i_val , Q->p) > 0) )	/*La funzione generica 'cmp' torna un'intero in base alla priorita' degli elementi*/
		max = l;
	else
		max = i;
	void *m_val = get_val( max );
	if( r && (cmp(r_val , m_val , Q->p) > 0) )
		max = r;
	if( max != i ){
		if( swap_other )						/*Utile per le visite dei grafi, guardare 'swap_addr'*/
			swap_other( i, get_val(i), max, get_val(max) );
		swaph( i , max );						/*Scambio dei valori associati ai nodi*/
		heapify( Q , max );						/*Chiamata ricorsiva alla sottocoda con radice nodo 'max'*/
	}
}

void *extract_max( queue *Q ){
	if( Q->heapsize < 1 )
		{ printf("errore: heap underflow\n"), exit( 1 ); }
	void *max = Q->head;
	void *last = getaddr( Q->head , Q->heapsize );	/*Ritorna l'ultimo nodo della coda*/
	if( swap_other )								/*Utile per le visite dei grafi, guardare swap_addr */
		swap_other( last, get_val(last), max, get_val(max) );
	swaph( max , last );							/*Scambio dei valori. La coda non rispetta piu' la priorita'*/
	Q->head = remove_h( Q , last );					/*Stacca dalla coda il nodo col valore massimo*/
	if( Q->heapsize > 0 )
		heapify( Q , max );							/*Riordinamento con chiamata ad heapify*/
	else
		Q->head = NULL;
	return get_val(last);							/*Nel nodo 'last' vi e' ora il valore massimo*/
}

void decrease_key( queue *Q , void *i , void *new_val ){
	void *i_val = get_val( i );
	if( cmp(new_val , i_val , Q->p) > 0 )
		{ printf("errore: nuova chiave piu' grande"), exit( 1 ); }
	set_val( i , new_val );							/*Se il nuovo valore ha priorita' minore lo inserisce*/
	heapify( Q , i );								/*Dal nodo corrente e' necessario ri-ordinare*/
}

void increase_key( queue *Q , void *i , void *new_val ){
	void *i_val = get_val( i );
	if( cmp( new_val , i_val , Q->p) < 0 )
		{ printf("errore: nuova chiave piu' piccola"), exit( 1 ); }
	set_val( i , new_val );							/*Se il nuovo valore ha priorita' maggiore lo inserisce*/
	void *f = FT( Q , i );							/*E' necessario risalire la coda, quindi si ricava il padre di i*/
	void *f_val = get_val( f );
	i_val = get_val( i );
	while( f && (cmp( f_val , i_val , Q->p ) < 0) ){
		if( swap_other )							/*Utile per le visite dei grafi, guardare 'swap_addr'*/
			swap_other( i , get_val(i) , f , get_val(f) );
		swaph( i , f );								/*Effettua la risalita finche' la nuova chiave ha maggiore priorita'*/
		i = f;
		f = FT( Q , i );
		f_val = get_val( f );
	}
}

void insert_key( queue *Q , void *key ){
	void *min = alloc_min( Q );						/*Inserisce in fondo alla coda un nodo con valore a priorita' minima*/
	if( set_other )									/*Utile per le visite dei grafi, guardare 'set_addr'*/
		set_other( key , min );
	increase_key( Q , min , key );					/*Partendo dal fondo inserisce la nuova chiave ed effettua l'ordinamento*/
}

void delete_key( queue *Q , void *addr ){
	void *last = getaddr( Q->head , Q->heapsize );	/*Ritorna l'ultimo nodo della coda*/
	void *l_val = get_val( last );
	void *val = get_val( addr );
	if( cmp(l_val , val , Q->p) > 0 ) 				/*Scelta dell'ordinamento in base alla priorita'*/
		increase_key( Q , addr , l_val );
	else
		decrease_key( Q , addr , l_val );
	Q->head = deallocate( Q , last );				/*Elimina l'ultima foglia dallo Heap*/
}

queue *create_heap( int prior ){
	queue *new_Q = (queue *)malloc(sizeof(queue));
	new_Q->p = prior;
	new_Q->heapsize = 0;
	new_Q->head = NULL;
	return new_Q;
}

bool verify_heap( queue *Q , void *addr , void *f ){
	if( addr ){
		void *i = get_val( addr );
		if( (cmp(i , f , Q->p) <= 0) ){
			void *left = SX( Q , addr );
			void *right = DX( Q , addr );
			return verify_heap(Q , left , i) && verify_heap(Q , right , i);
		}
		else
			return false;
	}
	else
		return true;
}

void print_queue( queue *Q , void *addr , PRINT print ){
	if( addr ){
		print( get_val(addr) );
		printf(" ; ");
		void *left = SX( Q , addr );
		void *right = DX( Q , addr );
		print_queue( Q , left , print );
		print_queue( Q , right , print );
	}
}
/*--------------------------------------------*/

/*----------- Funzioni di supporto -----------*/
void copy_data( void *dest , void *srg , int pos , int dim ){
	void *dst_addr = dest + (pos*dim);
	void *srg_addr = srg + (pos*dim);
	memcpy( dst_addr , srg_addr , dim );	/*Copia un dato sorgente in una destinazione byte per byte*/
}

void *allocate( int dim , void *srg ){
	int i, len = 1;
	if( lenght )
		len = lenght( srg ) + 1;
	void *dest = malloc(len * dim);			/*Alloca un elemento di qualsiasi tipo*/
	for (i=0 ; i<len ; i++)
		copy_data( dest , srg , i , dim );
	return dest;
}

void *get_min( int p ){
	void *ret;
	if( p == MAX_P ){
		ret = malloc(sizeof(float));
		*(float *)ret = FLT_MIN;
	}
	else if( p == MIN_P ){
		ret = malloc(sizeof(float));
		*(float *)ret = FLT_MAX;
	}
	return ret;								/*Ritorna il valore minimo in base alla priorità.*/
}

void *get_max( int p ){
	void *ret;
	if( p == MIN_P ){
		ret = malloc(sizeof(int));
		*(float *)ret = FLT_MIN;
	}
	else if( p == MAX_P ){
		ret = malloc(sizeof(int));
		*(float *)ret = FLT_MAX;
	}
	return ret;								/*Ritorna il valore massimo in base alla priorità.*/
}

void *search_key( queue *Q , int path ){
	if( (path < 1) || (path > Q->heapsize) )
		{ printf("errore: locazione assente\n"); return 0; }
	return getaddr( Q->head , path );		/*Ritorna l'indirizzo di un elemento della coda, se esiste*/
}

node *allocate_node( node *f , int s ){
	node *new_node = (node *)malloc(sizeof(node));
	new_node->father = f;
	new_node->side = s;
	new_node->left = NULL;
	new_node->right = NULL;
	return new_node;						/*Alloca spazio per un nodo, gli assegna un padre e la codifica del lato*/
}

/*--------------------------------------------*/

/*------------------ :NEW: -------------------*/
int float_cmp( void *a , void *b , int p ){
	float fl_a = *(float *)a;
	float fl_b = *(float *)b;
	if( p == MAX_P )
		return ( (fl_a > fl_b) ? 1 : ((fl_a < fl_b) ? -1 : 0) );
	else if( p == MIN_P )
		return ( (fl_b > fl_a) ? 1 : ((fl_b < fl_a) ? -1 : 0) );
}

#include "qtree.h"
#include "f_graph.h"
void set_heap_callbacks( ){		/*Le funzioni scelta lavorano su alberi, nei sorgenti completi per le code,
								 *sono presenti le funzioni che lavorano su array*/
	get_val = getval_T;	
	set_val = setval_T;
	SX = sx_T;
	DX = dx_T;
	FT = ft_T;
	getaddr = getaddr_T;
	swaph = swap_T;
	alloc_min = alloc_min_T;
	remove_h = remove_T;
	deallocate = deallocate_T;
	cmp = float_cmp;
	lenght = NULL;
	t_free = free;
	dim = sizeof(float);
	p = MIN_P;
	
	swap_other = swap_addr;
	set_other = set_addr;
}
