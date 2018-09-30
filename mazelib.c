#include <math.h>
#include <ncurses.h>

#include "mazelib.h"

	/* ==========  ========== ========== ========= */
	/*    Labirinti - Libreria per la gestione     */
	/*      Autore - Daniele Cuomo N8601346        */
	/* ========== ========== ========== ========== */

float manhattan_distance( graph *G , int u , int v ){
	coord_t U = get_coord( G->l , u ), V = get_coord( G->l , v );
	return abs( U.x - V.x ) + abs( U.y - V.y );						/*Distanza tra due punti su un piano*/
}

void free_maze( graph *GMAZE ){
	int i;
	for( i=0 ; i<GMAZE->h ; i++ )
		free( GMAZE->nodes[i] );
	free( GMAZE );
}

int successor( int s , int v , int *pred ){
	if( pred[v] == -1 )
		return -1;
	if( pred[v] == s )
		return v;				/*v e' il successore del nodo di partenza s*/
	else
		return successor( s , pred[v] , pred );
}

/*-------------------------------*/

/*-------- Funzioni per le visite ---------*/
int adiac_I( graph *G , int u , int i ){
	int count = 0;									/*count serve a restituire il nodo i-esimo tra i nodi adiacenti (imax 4)*/
	coord_t U1, U2;
	U1 = get_coord( G->l , u );
	U2 = get_next( KEY_LEFT , U1 , G->l , G->h );	/*Nodo a sinistra di U1*/
	if( get_elem( *G , U2 ) > 33 ) 					/* != wall && != door && != center*/
		if( i == count++ )
			return get_index( G->l , U2.y , U2.x );

	U2 = get_next( KEY_RIGHT , U1 , G->l , G->h );	/*Nodo a destra di U1*/
	if( get_elem( *G , U2 ) > 33 )
		if( i == count++ )
			return get_index( G->l , U2.y , U2.x );

	U2 = get_next( KEY_UP , U1 , G->l , G->h );		/*Nodo sopra U1*/
	if( get_elem( *G , U2 ) > 33 )
		if( i == count++ )
			return get_index( G->l , U2.y , U2.x );

	U2 = get_next( KEY_DOWN , U1 , G->l , G->h );	/*Nodo sotto U1*/
	if( get_elem( *G , U2 ) > 33 )
		if( i == count++ )
			return get_index( G->l , U2.y , U2.x );
	return -1;
}

float weight_I( graph *GMAZE , int u , int v ){
	maze_t **mz = (maze_t **)GMAZE->nodes;			/*Casting a labirinto*/
	coord_t V = get_coord( GMAZE->l , v );
	return mz[V.y][V.x].weight;
}
/*--------------------------------------------*/

/*-------------- Getter, Setter --------------*/
int get_index( int l , int y , int x ){
	return (y*l) + x;						/*Una coordinata a due valori ha un indice corrispondente che dipende dalla larghezza*/
}

int abscissa( int l , int index ){
	return index % l; 						/*Asse x*/
}

int ordinate( int l , int index ){
	return index / l; 						/*Asse y*/
}

coord_t get_coord( int l , int pos ){
	coord_t tmp;
	tmp.y = ordinate( l , pos ); tmp.x = abscissa( l , pos );
	return tmp;
}

coord_t get_next( int cmd , coord_t dst , int l , int h ){
	switch( cmd ){
		case KEY_UP:
			dst.y = (dst.y + h - 1)%h; break;
		case KEY_DOWN:
			dst.y = (dst.y + 1)%h; break;
		case KEY_LEFT:
			dst.x = (dst.x + l - 1)%l; break;
		case KEY_RIGHT:
			dst.x = (dst.x + 1)%l; break;
	}
	return dst;
}

void set_coord( graph G , coord_t pos , int elem ){
	maze_t **Mz = (maze_t **)G.nodes;
	Mz[pos.y][pos.x].elem = elem;
}

int get_elem( graph G , coord_t pos ){
	maze_t **Mz = (maze_t **)G.nodes;
	return Mz[pos.y][pos.x].elem; 
}
/*--------------------------------------------*/