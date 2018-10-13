#include "g_list.h"
#include "libstack.h"

	/* ==========  ========== ========== ========= */
	/*      Grafi - Libreria per la gestione       */
	/*      Autore - Daniele Cuomo N8601346        */
	/* ========== ========== ========== ========== */

void create_adj_L( graph *G , int u , double p ){
	int v;
	list *L = NULL;
	for( v=0 ; v<G->size ; v++ ){
		if( is_occ( p ) )
			L = push( L , v );
	}
	G->nodes[u] = (void *)L;
}

void *start_conv_L( void *u , int v ){
	float *M = (float *)u;
	M[v] = 1;		/* esercizio 6 -> M[v] = w; */
	return u;
}

void delete_v_L( void **v ){
	list *L = *(list **)v;
	while( L && (L = pop( L )) );
	free( L );
	*v = NULL;
}

void del_rmnt_L( void **u , int n , int v ){
	/*Se u == adiac(v) -> delete
	  Se u == adiac(n) -> u = adiac(v)*/
	list *curr = NULL;
	list *succ = *(list **)u;
	while( succ ){
		if( top(succ) == v ){
			succ = pop( succ );
			if( curr )
				curr->next = succ;
			else
				*u = (void *)succ;
		}
		if( !succ )
			continue;
		if( top(succ) == n )
			succ->id_node = v;
		curr = succ;
		succ = succ->next;
	}
}

void delete_e_L( void **v1 , int v2 ){
	int u, i = 0;
	list *curr = NULL;
	list *succ = *(list **)v1;
	while( succ && (top(succ) != v2) ){
		curr = succ;
		succ = succ->next;
	}
	if( !curr )
		*v1 = (void *)pop( succ );
	else
		curr->next = pop( succ );
}

bool is_adj_L( void **v1 , int v2 ){
	list *curr = *(list **)v1;
	while( curr ){
		if( curr->id_node == v2 )
			return true;
		else
			curr = curr->next;
	}
	return false;
}

void update_e_L( void **v1 , int v2 ){
	list *L = *(list **)v1;
	*v1 = (void *)push( L , v2 );
}

int adiac_L( void *u , int GSIZE , int count ){
	list *curr = (list *)u;
	while( count && curr ){
		curr = curr->next;
		count--;
	}
	if( curr )
		return curr->id_node;
	else
		return -1;
}

void invert_L( graph *G , void **V , int u ){
	int v, i = 0;
	list **T = (list **)V;
	while( (v = adiac_L(G->nodes[u] , G->size , i++)) != -1 ){
		if( !T[v] )
			T[v] = (list *)calloc( 1 , sizeof(list));
		T[v] = push( T[v] , u );
	}
}

void *nulledge_L( int n ){
	return NULL;
}

/* ------- Funzione per il parsing ------- */
void set_edge_L( void **E , float w , int id ){
	*E = (void *)push( *(list **)E , id );
	/**E = (void *)push( *(list **)E , id , w );*/
}
/* ------- ----------------------- ------- */
