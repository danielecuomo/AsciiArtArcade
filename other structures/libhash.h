#ifndef LIBHASH_H_INCLUDED
#define LIBHASH_H_INCLUDED

#define CDOM 60				/*Cardinalita' caratteri alfanumerici*/

typedef struct hash_t{
	char *str;
	int index;
	struct hash_t *next;
	struct hash_t *pred;
}hash_t;					/*Tabella hash con liste*/

typedef struct table_size{
	int TSIZE;				/*Grandezza tabella*/
	int TDOMAIN;			/*Cardinalita' del dominio da codificare*/
	hash_t **HT;			/*Tabella*/
}table;

int prime_num( int , int * );
/*Ritorna il numero primo più piccolo, successivo all'input (max 101)*/
table *alloc_table( int , int );
/*Allocazione tabella hash con liste per le collisioni*/
void modify_tb( table * , int , int );
/*Gestione tabella in seguito a modifica del grafo*/

/*-------- Funzioni hashing --------*/
int horner( int , char * );
/*Algoritmo di Horner*/
int hash( char * , int , int );
/*Ritorna l'indice destinato alla stringa in input*/
int hsearch( char * , table * );
/*Cerca in tabella la stringa in input*/
void hinsert( char * , table * , int );
/*Inserisce in tabella l'associazione vertice <-> indice*/
void hdelete( char * , table * );
/*Elimina un vertice dalla tabella.*/
/*-------- ---------------- --------*/

/*----- Funzioni per le associazioni indice -> vertice ------*/
char **v_list;
/*Array di associazioni indice -> vertice*/
void print_vlist( char ** );
/*Stampa i nomi esterni dei vertici*/
char **add_vlist( char ** , char * , int );
/*Aggiunge all'array un vertice*/
char **modify_vlist( char ** , int , int );
/*Gestione array in seguito a modifica del grafo*/
/*----- ------------------------------------------ -----*/

#endif
