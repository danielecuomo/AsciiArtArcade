#include <ncurses.h>
#include <string.h>

#include "huskygame.h"

	/* ==========  ========== ========== ========= */
	/*      Autore - Daniele Cuomo N8601346        */
	/* ========== ========== ========== ========== */

void start_game( void );
/*Inizializzazione partita*/
void game_over( bool );
/*Gestione fine partita*/
void set_cb( void );
/*Istanzia le callback*/
void show_help( void );
/*Stampa a video di informazioni varie*/
void show_bar( int , int );
/*Mostra sullo schermo una barra di difficolta'*/
void menu( void );
/*Mostra il menu di gioco*/
int get_choice( int , int , char * );
/*Rende navigabile il menu*/

int main( int argc , char **argv ){
	int i;
	start_game();										/*Inizializzazione curses-mode*/
	for( i=1 ; i<argc && play_level( argv[i] ) ; i++ );
	game_over( i == argc );								/*Stampa delle informazioni di fine partita*/
}

void start_game( ){
	initscr();											/*Inizia la modalità curses*/
	keypad(stdscr, TRUE);								/*Abilita la lettura dei tasti speciali*/
	start_color();										/*Inizializzazione funzionalita' dei colori*/

	/*Definizione coppie di colori*/
	init_pair(1, COLOR_BLUE, COLOR_BLACK);
	init_pair(2, COLOR_YELLOW, COLOR_BLACK);
	init_pair(3, COLOR_RED, COLOR_BLACK);
	init_pair(4, COLOR_WHITE, COLOR_WHITE);
	DELAY = 120000;
	noecho();											/*Stampa a video del buffer disabilitata*/
	curs_set(0);										/*Visualizzazione del cursore disabilitata*/
	menu();												/*Mostra il menu pre-gioco*/
	set_cb();											/*Imposta le callback specifiche per le visite dei labirinti*/
	ungetc( SPACE , stdin ); 							/*Prima mossa, SPACE di default*/
	SCORE = 0;
	nodelay(stdscr,TRUE); 								/*Lettura da tastiera non-bloccante*/
}

void game_over( bool has_win ){
	clear();											/*Libera lo schermo*/
	char *res, out[100];
	if( has_win )
		res = "Vittoria!";
	else
		res = "Sconfitta!";
	sprintf( out , "%s - Punteggio = %d" , res , SCORE );
	attron( A_BOLD );
	mvprintw( LINES/2 , (COLS - strlen(out))/2 , out );	/*Stampa al centro dello schermo*/
	attroff( A_BOLD );
	refresh();
	nodelay(stdscr,FALSE);								/*Lettura da tastiera bloccante*/
	flushinp();											/*Svuota il buffer*/
	getch();											/*Aspetta un input dall'utente prima di chiudersi*/
	endwin();
}

void set_cb( ){
	adiac = adiac_I;
	weight = weight_I;
	h = manhattan_distance;								/*h -> stima euristica*/
	set_heap_callbacks();								/*Imposta le callback per le code a priorita'*/
}

void menu(){
	bool start = false;
	char *copyr = " HuskyGame: Daniele Cuomo N8601346";
	int y = (LINES/2), x = (COLS - strlen(copyr) - 1)/2;
	while( !start ){
		switch(get_choice( y , x , copyr )){
			case 0: start = true; break;			/*L'utente ha scelto l'opzione 'Start game'*/
			case 1: show_help(); break;				/*L'utente ha scelto l'opzione 'Help'*/
			case 2: show_bar( y+2 , x+14 ); break;
			case 3: endwin(); exit(1); break;		/*L'utente ha scelto l'opzione 'Exit'*/
		}
	}

}

int get_choice( int y , int x , char *copyr ){
	clear();
	move( y - 2, x );
	attron( A_BOLD );
	addch( 169 | A_ALTCHARSET );
	printw( copyr );
	attroff( A_BOLD );
	char *menu[] =  { "Start game", "Help", "Difficulty", "Exit" };
	int i, c;
	for( i=0 ; i<4 ; i++ )
		mvprintw( y + i, x + 2 , menu[i] );

	int cur = 0;										/*Cursore*/
	mvaddch( y + cur , x , 187 | A_ALTCHARSET );
	while( (c = getch()) != '\n' ){						/*Continua finche' l'utente non preme invio*/
		mvaddch( y + cur , x , ' ' );
		switch( c ){
			case KEY_DOWN: 	cur = (cur + 1)%4; break;
			case KEY_UP:	cur = (cur + 3)%4; break;
		}
		mvaddch( y + cur , x , 187 | A_ALTCHARSET );	/*Movimento cursore*/
	}
	return cur;
}

void show_help( ){
	clear();
	char *info1 = "Goal: survive and exit the level.\nTake the key (";
	char *info2 = ") and through the door (";
	char *info3 = ") to go to next level.\n\nESC to pause\nARROW KEYS to move\nSPACE to stop\nCTRL+C to quit the game";
	printw( info1 ); addch( 231 | A_ALTCHARSET );
	printw( info2 ); addch( ACS_PI ); printw( info3 );
	refresh();											/*Stampa effettiva*/
	getch();											/*Attesa di un input dall'utente*/
}

void set_delay( int cur ){
	switch( cur ){
		case 1: DELAY = 200000; break;
		case 2: DELAY =	150000; break;
		case 3: DELAY = 120000; break;
		case 4: DELAY = 95000; break;
		case 5: DELAY = 70000; break;
	}
}

int get_cur(){
	switch( DELAY ){
		case 200000: return 1;
		case 150000: return 2;
		case 120000: return 3;
		case 95000: return 4;
		case 70000: return 5;
	}
}

void print_bar( int y , int x , int cur ){
	int i;
	attron(COLOR_PAIR(4));						/*Bianco su bianco*/
	for( i=0 ; i<cur ; i++ )
		mvaddch( y , x+(i*2)+1 , ACS_CKBOARD);
	attroff(COLOR_PAIR(4));
	for( ; i<5 ; i++ )
		mvaddch( y , x+(i*2)+1 , ' ');
}

void show_bar( int y , int x ){
	int c, i, cur = get_cur();
	mvaddch(y, x-1, '-' | A_BOLD);
	mvaddch(y, x+11, '+' | A_BOLD);
	print_bar( y , x , cur );
	refresh();
	while( (c = getch()) != '\n' ){
		switch( c ){
			case KEY_LEFT: 	if(cur > 1) cur--; break;
			case KEY_RIGHT:	if(cur < 5) cur++; break;
		}
		print_bar( y , x , cur );
	}
	set_delay( cur );
}

