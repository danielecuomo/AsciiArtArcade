#ifndef G_PARSER_H_INCLUDED
#define G_PARSER_H_INCLUDED

#include <stdio.h>

#include "mazelib.h"
#include "huskygame.h"

	/* ==========  ========== ========== ========= */
	/*      Autore - Daniele Cuomo N8601346        */
	/* ========== ========== ========== ========== */

typedef enum { ESSE, EFFE, NUMB, COLON, WALL, STREET, ENEMIEv, ENEMIEw,
				POW, DOLLAR, CENTER, DOOR, RSPWN, KEY, TRAP } term;
/*Tipi per i simboli terminali*/

/*-------------- error handler ---------------*/
void catch_error( char * );
/*Stampa il parametro su stderr e abortisce*/
void syntax_error( char * , FILE * );
/*Stampa una stringa d'errore e abortisce*/
char *errorsymb( term );
/*Ritorna la stringa associata al parametro term*/
/*--------------------------------------------*/

/*------------ scanning terminal -------------*/
term match( FILE * );
/*Ritorna il simbolo corrispondente al carattere letto da file*/
bool is_digit( char );
/*Ritorna vero se l'offset punta ad una cifra*/
int get_int( FILE * );
/*Lettura intero da file*/
/*--------------------------------------------*/

/*----------- manage non-terminal ------------*/
level_t parse_maze( FILE * );
/*Costruzione di un labirinto mediante parsing*/
level_t get_maze( int , int , int , FILE * );
/*Restituisce un labirinto sotto forma di grafo implicito*/
/*--------------------------------------------*/

#endif
