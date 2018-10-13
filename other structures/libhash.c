#include <stdlib.h>
#include <stdio.h>

#include "libhash.h"
#include "libstack.h"

int prime_num( int input , int *p ){
	if( input > *p )
		return prime_num( input , p+1 );
	else if( input <= *p );
		return *p;
}

table *alloc_table( int n , int domain ){
	table *TB = (table *)malloc(sizeof(table));
	int p;
	int prime[] = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43,
					  47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101 };
	if( n > 50 )
		p = 101;
	else
		p = prime_num( n*2 , prime );
	TB->TSIZE = p;
	TB->TDOMAIN = domain;
	TB->HT = (hash_t **)calloc( TB->TSIZE , sizeof(void *) );
	return TB;
}

void modify_tb( table *TB , int u , int last ){
	/*Il vertice associato ad u è stato eliminato.
	  Il vertice associato a last è ora associato ad u.*/
	int i;
	hash_t *tmp1 = NULL, *tmp2 = NULL, *curr;
	for( i=0 ; i<TB->TSIZE ; i++ ){
		curr = TB->HT[i];
		while( curr && (!tmp1 || !tmp2) ){
			if( curr->index == last )
				tmp1 = curr;
			if( curr->index == u )
				tmp2 = curr;
			curr = curr->next;
		}
		if( tmp1 && tmp2 )	break;
	}
	tmp1->index = u;
	hdelete( tmp2->str , TB );
}

/*-------- Funzioni hashing --------*/
int horner( int x , char *str ){
	if( *str != '\0' )
		return *str + x * horner( x , (str + 1) );
	else
		return 0;
}

int hash( char *str , int DOMAIN , int TSIZE ){
	return horner( DOMAIN , str )%TSIZE;
}

int hsearch( char *str , table *TB ){
	int j = hash( str , TB->TDOMAIN , TB->TSIZE );
	hash_t *tmp = visit_list( str , TB->HT[j] );
	if( tmp )
		return tmp->index;
	else
		return -1;
}

void hinsert( char *str , table *TB , int i ){
	int j = hash( str , TB->TDOMAIN , TB->TSIZE );
	TB->HT[j] = hpush( TB->HT[j] , str , i );
}

void hdelete( char *str , table *TB ){
	int j = hash( str , TB->TDOMAIN , TB->TSIZE );
	hash_t *curr = visit_list( str , TB->HT[j] );
	hash_t *pred = curr->pred;
	if( !pred )
		TB->HT[j] = hpop( curr );
	else
		pred->next = hpop( curr );
}
/*-------- ---------------- --------*/

/*----- Array delle associazioni intero<->vertice ------*/
void print_vlist( char **vl ){
	if( *vl ){
		printf(" %s ,", *vl );
		print_vlist( vl + 1 );
	}
}

char **add_vlist( char **vl , char *str , int last ){
	vl = (char **)realloc( vl , last + 1 );
	vl[last-1] = str;
	vl[last] = NULL;
	return vl;
}

char **modify_vlist( char **vl , int u , int last ){
	/*Il vertice associato ad u è stato eliminato.
	  Il vertice associato a last è ora associato ad u.*/
	vl[u] = vl[last];
	vl = (char **)realloc( vl , last + 1 );
	vl[last] = NULL;
	return vl;
}
/*----- ------------------------------------------ -----*/
