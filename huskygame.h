#ifndef HUSKYGAME_H_INCLUDED
#define HUSKYGAME_H_INCLUDED

#include "mazelib.h"

	/* ==========  ========== ========== ========= */
	/*      Autore - Daniele Cuomo N8601346        */
	/* ========== ========== ========== ========== */

/*Fare riferimento alla tabella ASCII*/
#define SPACE 	32
#define ESC 	27

typedef int (* MOVECH)( graph * , int , int * );
/*Tipo di funzione generica: spostamento personaggio*/

typedef struct character{
	char id;		/*Carattere che identifica il personaggio*/
	int color;		/*Colore del personaggio*/
	int *pos;		/*Intero che codifica la posizione del personaggio*/
	int *t;			/*Intero che codifica il bersaglio da raggiungere*/
	int speed;		/*Velocità del personaggio: si muove ogni x turni, dove x dipende dal valore di speed*/
	bool powerup;	/*Nel caso del protagonista, powerup e' vero se il giocatore e' passato sul nodo pow*/
	int time;		/*La variabile time memorizza il turno in cui viene attivato il powerup*/
	MOVECH movech;
}character;
/*----- Personaggio generico del gioco ------*/

typedef struct level{
	graph *G;		/*Grafo con i nodi del labirinto*/
	character *ch; 	/*La prima locazione e' riservata al protagonista*/
	int CHSIZE;		/*Numero di personaggi*/
	int respawn;	/*Intero che codifica la posizione di respawn (-1 = !respawn)*/
	int exit;		/*Intero che codifica la posizione dell'uscita*/
}level_t;
/*----- Struttura per un livelo di gioco ----*/

int SCORE;		/*Punteggio accumulato giocando i livelli*/
int TIME;		/*Tempo rimasto per finire il livello*/
int DELAY;		/*Pausa in microsecondi, definisce la durata minima di un turno*/

/*------------ Gestione livello -------------*/
level_t init_level( char * );
/*Inizializzazione del livello*/
void free_level( graph * , character * , int );
/*Eliminazione del livello*/
bool play_level( char * );
/*Prepara i livelli da giocare ricorsivamente, ad ogni vittoria*/
bool game_routine( level_t  );
/*Muove i personaggi nel livello finche' l'utente non vince o viene sconfitto*/
level_t create_level( int , int , int );
/*Creazione di un livello - chiamata durante il parsing*/
character create_character( int, int, int, char, int, int, MOVECH );
/*Creazione di un personaggio - chiamata durante il parsing*/
/*-------------------------------------------*/

/*----------- Gestione spostamenti -----------*/
bool shifts( graph * , character * , int , int , int );
/*Sposta tutti i personaggi, se la loro velocità glielo consente in quel turno*/
int follow_target( graph * , int , int * );
/*Sposta il personaggio seguendo un'array di predecessori*/
int follow_random( graph * , int , int * );
/*Sposta il personaggio in una locazione casuale, se legale*/
int follow_command( graph * , int , int * );
/*Sposta il personaggio seguendo i comandi, se legale*/
	/*----- Funzioni di supporto -----*/
	void pause_game( int , int );
	/*Ferma il gioco finche' l'utente non vuole continuare*/
	bool is_command( int , int , int );
	/*Torna vero se il comando inserito dall'utente e' una freccia direzionale*/
	int get_command( int , int );
	/*Prende un valore dal buffer, se non e' valido torna SPACE di default*/
	int get_target( graph , int , int );
	/*Segue la direzione richiesta nel labirinto finche' non incontra un muro*/
	int next_pos( graph , int , int , bool );
	/*Ritorna le coordinate in base alla posizione corrente e alla direzione richiesta*/
/*--------------------------------------------*/

/*-------------- Gestione turno --------------*/
bool bonus( graph , character * , int , int , int );
/*Gestione dei bonus presi dal protagonista*/
void set_powerup( character *, int, int, MOVECH, bool, int );
/*Modifica le caratteristiche dei personaggi in base allo stato di powerup*/
void speed_up( character * , int );
/*Aumenenta la velocità degli avversari, finche' possibile (max = 1)*/
bool collide( character * , int , int );
/*Torna vero se un nemico si e' scontrato col protagonista (!powerup)*/
/*--------------------------------------------*/

/*-------------- Gestione porte --------------*/
bool door_manage( coord_t , coord_t  , graph );
/*Torna vero se la porta può girare, falso altrimenti*/
coord_t get_center( coord_t , graph );
/*Ritorna il cardine di una porta girevole*/
int center_y( coord_t , maze_t ** , int , int );
/*Ritorna l'ordinata del nodo centrale della porta girevole*/
int center_x( coord_t , maze_t ** , int , int );
/*Ritorna l'ascissa del nodo centrale della porta girevole*/
void set_direction( coord_t , maze_t ** , int , int , int , int );
void rotate_door( coord_t , graph );
/*Rotazione di una porta girevole*/
/*--------------------------------------------*/

/*------------- Gestione output --------------*/
void turnstamp( graph , character * , int , int );
/*Stampa a video lo stato del labirinto nell'ultimo turno giocato*/
void print_info( void );
/*Stampa a video del punteggio accumulato e del tempo rimanente*/
void print_maze( graph );
/*Stampa a video dei nodi del labirinto*/
void print_character( character , int );
/*Stampa a video del personaggio passato in input*/
void chmanage( int , int , int );
/*Associa e stampa un elemento alla sua rappresentazione in curses mode*/
void preface( char * , int , int );
/*Mostra al centro del terminale il titolo del livello*/
/*--------------------------------------------*/

#endif