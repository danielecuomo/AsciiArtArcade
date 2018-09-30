#include <stdio.h>
#include <string.h>
#include <ncurses.h>
#include <unistd.h>

#include "huskygame.h"
#include "g_parser.h"
#include "f_graph.h"

	/* ==========  ========== ========== ========= */
	/*      Autore - Daniele Cuomo N8601346        */
	/* ========== ========== ========== ========== */

/*------------- Gestione livello -------------*/
level_t init_level( char *str ){
	char l[50];
	FILE *f;
	sprintf( l, "labirinti/%s", str );		/*I file sono salvati in una directory labirinti*/
	if( !(f = fopen( l , "r" )) )
		catch_error("error on fopen\n");
	level_t lvl = parse_maze( f );
	if( fclose( f ) )
		catch_error("error on fclose\n");
	return lvl;
}

void free_level( graph *G , character *ch , int CHSIZE ){
	free_maze( G );											/*Deallocazione grafo*/
	int i;
	for( i=0 ; i<CHSIZE ; i++ )								/*Deallocazione personaggi*/
		free( ch[i].pos );
	free( ch );
}

bool play_level( char *level ){
	level_t lvl = init_level( level );						/*Inizializzazione livello: parsing e allocazione*/
	preface( level , lvl.G->h , lvl.G->l );
	return game_routine( lvl );								/*Tutto il singolo livello e' eseguito in questa chiamata*/
}

bool game_routine( level_t lvl ){
	int round = 0;																/*Contatore dei giri*/
	bool alive = true, finish = false;
	turnstamp( *lvl.G , lvl.ch , lvl.CHSIZE , round );							/*Prima stampa del labirinto, inizia il turno 0*/
	while( !finish && alive ){
		alive = shifts( lvl.G , lvl.ch , lvl.CHSIZE , round , lvl.respawn );	/*Fase 1: spostamenti*/
		finish = bonus(  *lvl.G , lvl.ch , lvl.CHSIZE , round , lvl.exit );		/*Fase 2: gestione bonus*/
		turnstamp( *lvl.G , lvl.ch , lvl.CHSIZE , round );						/*Fase 3: stampa*/
		round++;
	}
	free_level( lvl.G , lvl.ch , lvl.CHSIZE );
	return finish;
}

level_t create_level( int l , int h , int n ){
	level_t lvl;
	lvl.G = (graph *)malloc(sizeof(graph));
	lvl.G->size = l*h;
	lvl.G->l = l; lvl.G->h = h;
	lvl.CHSIZE = n;								/*Numero di personaggi presenti nel livello*/
	lvl.respawn = -1;							/*Locazione di rinascita dei nemici, -1 di default*/
	return lvl;
}

character create_character( int l , int y , int x , char id , int speed , int clr , MOVECH movech ){
	character ch;
	ch.pos = (int *)malloc(sizeof(int));		/*Con un puntatore, piu' personaggi potranno accedere alla stessa locazione*/
	*ch.pos = get_index( l , y , x );			/*Torna l'indice corrispondente alle cordinate*/
	ch.id = id;
	ch.speed = speed;
	ch.color = clr;
	ch.movech = movech;
	return ch;
}
/*--------------------------------------------*/

/*----------- Gestione spostamenti -----------*/
bool shifts( graph *G, character *ch, int CHSIZE, int round, int rspwn ){
	int i;
	for( i=0 ; i<CHSIZE ; i++ ){
		if( !(round%ch[i].speed) && *ch[i].pos != -1 ){				/*Piu' speed v piccolo, piu' turni giocherà il personaggio i-esimo*/
			*ch[i].pos = ch[i].movech( G , *ch[i].pos , ch[i].t );	/*Aggiorna la propria posizione calcolata*/
			if( collide( ch , CHSIZE , rspwn ) )					/*Ad ogni aggiornamento e' necessario verificare se ci sono collisioni*/
				return false;
		}
	}
	return true;													/*Nessuna collisione*/
}

int follow_target( graph *G , int pos , int *t ){
	if( pos == *t )								/*Target e posizione corrente coincidiono*/
		return pos;
	int dst, *pred = astar( G , pos , *t );		/*Visita del grafo*/
	if( pred ){
		dst = successor( pos , *t , pred );		/*Ritorna l'indice del nodo successore a pos*/
		free(pred);
	}
	else
		dst = follow_random( G , pos , t );		/*Se non c'e' alcun cammino, esegue uno spostamento casuale*/
	return dst;
}

int follow_random( graph *G , int pos , int *t ){
	int cmd = get_random( 258 , 261 );						/*{258, 259, 260, 261} = {KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT}*/
	return next_pos( *G , pos , cmd , false );				/*Ritorna la posizione in base al comando (casuale) e al grafo*/
}

int follow_command( graph *G , int pos , int *t  ){
	int cmd = get_command( G->h , G->l );					/*Prende dal buffer un comando*/
	*t = get_target( *G , pos , cmd );						/*In base al comando calcola la posizione di arrivo*/
	return next_pos( *G , pos , cmd , true );				/*Ritorna la posizione in base al comando e al grafo*/
}

	/*----- Funzioni di supporto -----*/
void pause_game( int h , int l ){
	mvprintw( h/2 + 2, (l - 5)/2, "PAUSE" );				/*Stampa al centro del labirinto*/
	refresh();
	nodelay(stdscr,false);
	while( getch() != ESC );
	nodelay(stdscr,true);
}

bool is_command( int cmd , int h , int l ){
	if( cmd == ESC )
		pause_game( h , l );
	return (cmd == KEY_UP) || (cmd == KEY_DOWN) || (cmd == KEY_LEFT) || (cmd == KEY_RIGHT);
}

int get_command( int h , int l ){
	int dir, cmd;
	while( (dir = getch()) != ERR )		/*Salvataggio dell'ultimo comando inserito*/
		cmd = dir;
	if( !is_command( cmd , h , l ) )	/*Se non e' valido diventa SPACE di default*/
		cmd = SPACE;
	ungetch( cmd );						/*Reinserisce in cima al buffer il comando*/
	return cmd;
}

int get_target( graph G , int pos , int cmd ){
	int srg, dst;
	srg = pos;
	while( (dst = next_pos( G , srg , cmd , true )) != pos && dst != srg )
		srg = dst;						/*Salva la nuova posizione finche' non trova un muro o la locazione di partenza (ciclo)*/
	return srg;
}

int next_pos( graph G , int pos , int cmd , bool open ){
	coord_t srg, dst;
	bool next = false;
	srg = get_coord( G.l , pos );								/*Ritorna le coordinate della locazione corrente*/
	dst = get_next( cmd , srg , G.l , G.h );					/*Ritorna le coordinate in base alla locazione corrente e al comando*/
	int elem = get_elem( G , dst );								/*Ritorna l'elemento presente nel grafo alle coordinate di dst*/
	if( open && elem == door )									/*Se open e' falso le porte sono indistinguibili dai muri*/
		if( door_manage( srg , dst , G ) ) 						/*Vero quando e' possibile passare*/
			next = true;
	if( elem != wall && elem != center && elem != door )
		next = true;
	if( next )
		return get_index( G.l , dst.y , dst.x );
	else
		return pos;												/*Nessun movimento: ritorna la locazione corrente*/
}
/*--------------------------------------------*/

/*-------------- Gestione turno --------------*/
bool bonus( graph G , character *ch , int CHSIZE , int round , int exitpos ){
	coord_t pos = get_coord( G.l , *ch[0].pos );
	int elem = get_elem( G , pos );									/*Ritorna l'elemento presente nel grafo alle coordinate di pos*/
	if( elem == dollar )
		SCORE += 50;
	if( elem == pow )
		set_powerup( ch, CHSIZE, round, follow_random, true, 3 );	/*Attivando il powerup, i nemici si muoveranno in maniera casuale*/
	if( ch[0].powerup && (round - ch[0].time) == 60 )				/*Se sono passati 60 turni, i nemici tornano in modalità inseguimento*/
		set_powerup( ch, CHSIZE, round, follow_target, false, 2 );
	if( elem == door )
		rotate_door( pos , G );
	if( elem == trap )
		ch[0].speed++;
	if( elem == key ){
		SCORE += 150;
		coord_t tmp = get_coord( G.l , exitpos );
		set_coord( G , tmp , end );									/*Compare la porta di fine livello*/
	}
	if( !(round%200) )												/*Ogni 200 turni i nemici diventano piu' veloci*/
		speed_up( ch , CHSIZE );
	if( !(round%8) )
		TIME--;
	if( elem == end )												/*Il tempo avanzato si somma al punteggio*/
		SCORE += TIME;
	set_coord( G , pos , street );
	return elem == end || !TIME;									/*Vero se il giocatore e' arrivato all'uscita o se e' scaduto il tempo*/
}

void set_powerup( character *ch, int CHSIZE, int round, MOVECH fun, bool state, int clr ){
	ch[0].powerup = state;
	ch[0].time = round;
	ch[0].color = clr;
	int i;
	for( i=1 ; i<CHSIZE ; i++ )
		ch[i].movech = fun;
}

void speed_up( character *ch , int CHSIZE ){
	int i;
	for( i=1 ; i<CHSIZE ; i++ ){
		if( ch[i].speed-1 )			/*Al decrescere di speed aumenta la velocità (max 1).*/
			ch[i].speed--;
	}
}

bool collide( character *ch , int n , int rspwn ){
	if( --n ){
		bool tmp = (*ch[0].pos == *ch[n].pos);						/*Vero se c'e' una collisione*/
		if( ch[0].powerup && tmp ){									/*Se c'e' una collisione durante il powerup, il nemico viene eliminato*/
			*ch[n].pos = rspwn;
			SCORE += 100;
		}
		return tmp && !ch[0].powerup || collide( ch , n , rspwn );	/*La collisione fa perdere il giocatore solo se non ha il powerup*/
	}
	else
		return false;
}
/*--------------------------------------------*/

/*-------------- Gestione porte --------------*/
bool door_manage( coord_t srg , coord_t pivot , graph G ){
	pivot = get_center( pivot , G );
	return !(pivot.y == srg.y || pivot.x == srg.x); 					/*Vero se cambiano entrambi gli assi*/
}

coord_t get_center( coord_t pivot , graph G ){
	pivot.y = center_y( pivot , (maze_t **)G.nodes , G.l , G.h );		/*Ordinata del cardine della porta*/
	pivot.x = center_x( pivot , (maze_t **)G.nodes , G.l , G.h );		/*Ascissa del cardine della porta*/
	return pivot;
}

int center_y( coord_t pivot , maze_t **mz , int l , int h ){
	if( mz[(pivot.y + h - 1)%h][pivot.x].elem == center )
		return (pivot.y + h - 1)%h;
	else if( mz[(pivot.y + 1)%h][pivot.x].elem == center )
		return (pivot.y + 1)%h;
	else
		return pivot.y;													/*Se il cardine non si trova ne' sopra ne' sotto il nodo porta, l'asse x e' invariato*/
}

int center_x( coord_t pivot , maze_t **mz , int l , int h ){
	if( mz[pivot.y][(pivot.x + l - 1)%l].elem == center )
		return (pivot.x + l - 1)%l;
	else if( mz[pivot.y][(pivot.x + 1)%l].elem == center )
		return (pivot.x + 1)%l;
	else
		return pivot.x;													/*Se il cardine non si trova ne' a sinistra ne' a destra del nodo porta, l'asse x e' invariato*/
}

void rotate_door( coord_t pivot , graph G ){
	maze_t **mz = (maze_t **)G.nodes;
	pivot = get_center( pivot , G );									/*Ritorna il cardine della porta girevole*/
	if( mz[pivot.y][(pivot.x + G.l - 1)%G.l].elem == door )				/*La rotazione avviene in base alla posizione corrente*/
		set_direction( pivot , mz , G.l , G.h , door , street );
	else
		set_direction( pivot , mz , G.l , G.h , street , door );
}

void set_direction( coord_t pivot, maze_t **mz, int l, int h, int elem1, int elem2 ){
	mz[(pivot.y + h - 1)%h][pivot.x].elem = elem1;
	mz[(pivot.y + 1)%h][pivot.x].elem = elem1;
	mz[pivot.y][(pivot.x + l - 1)%l].elem = elem2;
	mz[pivot.y][(pivot.x + 1)%l].elem = elem2;
}
/*--------------------------------------------*/

/*------------- Gestione output --------------*/
void turnstamp( graph G , character *ch , int CHSIZE , int round ){
	int i;
	print_info();
	print_maze( G );									/*Stampa del labirinto vuoto*/
	char id[] = {'{', '(', 'c', '<'};
	ch[0].id = id[round%4];

	for( i=0 ; i<CHSIZE ; i++ )							/*Stampa dei personaggi nel labirinto*/
		print_character( ch[i] , G.l );
	refresh();
	usleep( DELAY );									/*Pausa in microsecondi, permette l'utente di seguire il gioco*/
}

void print_info(){
	move(0, 0);
	clrtoeol();											/*Eliminazione della riga in cui e' posizionato il cursore*/
	mvprintw(0, 0, "Score: %d\tTime: %d", SCORE, TIME);
}

void print_maze( graph G ){
	maze_t **Mz = (maze_t **)G.nodes;
	int i, j;
	for(i=0 ; i<G.h ; i++ ){
		for(j=0 ;j<G.l ; j++ )
			chmanage( Mz[i][j].elem , i + 2 , j );		/*Stampa del carattere corrispondente all'elemento*/
		mvaddch( i + 2 , j , '\n' );
	}
}

void print_character( character ch , int l ){
	if( *ch.pos != -1 ){								/*-1 -> Personaggio morto*/
		coord_t tmp = get_coord( l , *ch.pos );
		attron( COLOR_PAIR(ch.color) );
		mvaddch( tmp.y + 2 , tmp.x , ch.id | A_BOLD );	/*Il labirinto e' shiftato di due rige per far spazio alle info*/
		attroff( COLOR_PAIR(ch.color) );
	}
}

void chmanage( int elem , int i , int j ){
	switch( elem ){
		case end:		mvaddch( i, j, ACS_PI | A_BOLD ); break;				/*Pi greco, può ricordare una porta*/
		case dollar:	mvaddch( i, j, ACS_DEGREE | A_BOLD ); break;
		case street:	mvaddch( i, j, ' ' ); break;
		case pow:		attron(COLOR_PAIR(3)); mvaddch( i, j, ACS_DIAMOND | A_BOLD); attroff(COLOR_PAIR(3)); break;
		case door:		mvaddch( i, j, '+'); break;
		case key:		mvaddch( i, j, 231 | A_BOLD | A_ALTCHARSET ); break;	/* (231 | A_ALTCHARSET) = 'ç'*/
		case trap:		mvaddch( i, j, 164 | A_BOLD | A_ALTCHARSET ); break;
		default:		attron(COLOR_PAIR(4)); mvaddch( i, j, ACS_CKBOARD); attroff(COLOR_PAIR(4));
	}
}

void preface( char *name, int h, int l ){
	int i, y = LINES/2, x = (COLS - strlen(name))/2;
	clear();
	attron( A_BOLD );
	mvprintw( y, x, name );		/*Stampa al centro del terminale del titolo del livello*/
	attroff( A_BOLD );
	refresh();
	sleep(1);
	move(y,x);
	clrtoeol();
	TIME = 250;
	for(i=0; i<l ; i++)			/*Stampa di una linea separatrice*/
		mvaddch(1, i, '-' );
	flushinp();					/*Il buffer potrebbe essere "sporco"*/
}
/*--------------------------------------------*/