#ifndef MAZELIB_H_INCLUDED
#define MAZELIB_H_INCLUDED

#include "f_graph.h"

	/* ==========  ========== ========== ========= */
	/*    Labirinti - Libreria per la gestione     */
	/*      Autore - Daniele Cuomo N8601346        */
	/* ========== ========== ========== ========== */

#define wall	11		/*Muro: un nodo irragiungibile*/
#define center	22		/*Centro: la porta girevole e' composta da un nodo centro e 2 nodi porta*/
#define door	33		/*Porta: un nodo con proprietà specifiche*/
#define pow  	44		/*Power-up: bonus che da dei "poteri" al protagonista*/
#define dollar  55		/*Punti: bonus che aumenta il punteggio del giocatore*/
#define street  66		/*Strada: un nodo "navigabile" dai personaggi*/
#define end		77		/*Fine: nodo di fine livello*/
#define key		88		/*Chiave: permette di far comparire la porta*/
#define trap	99		/*Trappola: riduce la velocità del protagonista*/

typedef struct coordinates{
	int x;
	int y;
}coord_t;
/*---------- Coordinate: asse x e y ----------*/

typedef struct maze_elem{
	float weight;
	int elem;
}maze_t;
/*----------- Nodo di un labirinto -----------*/

float manhattan_distance( graph * , int , int );
/*Stima euristica per grafi impliciti*/
void free_maze( graph * );
/*Cancellazione del labirinto passato*/
int successor( int , int , int * );
/*Ritorna il nodo successore al primo input seguendo l'array dei predecessori*/
/*--------------------------------------------*/

/*---------- Funzioni per le visite ----------*/
int adiac_I( graph * , int , int );
/*Restituisce il nodo i-esimo tra i nodi adiacenti a quello in input*/
float weight_I( graph * , int , int );
/*Restituisce il peso dell'arco tra i due input*/
/*--------------------------------------------*/

/*-------------- Getter, Setter --------------*/
int get_index( int , int , int );
/*Ritorna l'indice di un array corrispondente alla codifica della matrice*/
int abscissa( int , int );
/*Ritorna l'ascissa del punto corrispondente all'indice*/
int ordinate( int , int );
/*Ritorna l'ordinata del punto corrispondente all'indice*/
coord_t get_coord( int , int );
/*Ritorna le coordinate della matrice decodificando l'indice corrispondente*/
coord_t get_next( int , coord_t , int , int );
/*Torna un nodo vicino al corrente nella direzione richiesta (adiacente e non)*/
void set_coord( graph , coord_t , int );
/*Inserisce l'elemento in input nel labirinto alla locazione richiesta*/
int get_elem( graph , coord_t );
/*Ritorna l'elemento della locazione del labirinto secondo l'input*/
/*--------------------------------------------*/

#endif