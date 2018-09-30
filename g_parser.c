#include <string.h>
#include <limits.h>
#include <ncurses.h>

#include "g_parser.h"

	/* ==========  ========== ========== ========= */
	/*      Autore - Daniele Cuomo N8601346        */
	/* ========== ========== ========== ========== */

/*-------------- error handler ---------------*/
void catch_error( char *errstr ){
    endwin();								/*Chiusura della curses-mode*/
    write( 2 , errstr , strlen(errstr) );	/*Scrive su stderr la stringa in input*/
    exit( 1 );
}

void syntax_error( char *err , FILE *f ){
	char *stxerr = "errore di sintassi: %s previsto nella posizione %d.\n";	/*Format messaggio d'errore*/
 	char str[80];
	sprintf( str , stxerr , err , ftell( f ) );
	catch_error( str );
}

char *errorsymb( term st ){
    char *symb;
    switch( st ){
    	case NUMB:   	symb = "number"; break;
    	case COLON:		symb = ":"; break;
       	case STREET:	symb = "maze symbol"; break;
       	default: 		symb = NULL;
    }
    return symb;
}
/*--------------------------------------------*/

/*------------ scanning terminal -------------*/
term match( FILE *f ){
	term rp;
	char c;
	while( ((c = fgetc( f )) == '\t') || (c == '\n') || (c == ' ') );	/*Il parse ignora i caratteri: '\t', '\n', ' '*/
    switch( c ){
    	case 'O': rp = WALL; break;
		case 'x': rp = STREET; break;
    	case 's': rp = ESSE; break;
    	case 'f': rp = EFFE; break;
    	case 'v': rp = ENEMIEv; break;
    	case 'w': rp = ENEMIEw; break;
    	case 'b': rp = POW; break;
    	case '+': rp = CENTER; break;
    	case '#': rp = DOOR; break;
    	case '$': rp = DOLLAR; break;
    	case 'R': rp = RSPWN; break;
    	case 'k': rp = KEY; break;
    	case 't': rp = TRAP; break;
    	default : rp = COLON; break;
    }
    ungetc( c , f );													/*Reinserisce il carattere appena letto*/
 	return rp;
}

bool is_digit( char c ){
	return (c >= 48) && (c <= 57);						/*Vero se il caratte in input rappresenta una cifra [0,9]*/
}

int get_int( FILE *f ){
	int c = 0, num = 0, l = 0;
	while( ((c = fgetc( f )) == '\t') || (c == '\n') || (c == ' ') );
	ungetc( c , f );
	while ( (c = fgetc(f)) != EOF && is_digit(c) ){
		if( num <= ((INT_MAX - (c-'0'))/10) ){
			num = num*10 + (c-'0');
			l++;
		}
	    else
			syntax_error( "il numero dev'essere <= 2147483647" , f );
	}
	if(l == 0)
		syntax_error( errorsymb(NUMB) , f );
	if(c != EOF)
		ungetc( c , f );
	return num;
}
/*--------------------------------------------*/

/*----------- manage non-terminal ------------*/
level_t parse_maze( FILE *f ){
	int l, h, nchar;
	l = get_int( f );							/*Il file avra' prima un intero che codifica la largezza del labirinto*/
	if( match( f ) != COLON )					/*Ogni intero e' separato da un simbolo separatore ':'*/
		syntax_error( errorsymb(COLON) , f );
	else
		fseek( f , 1 , SEEK_CUR );
	h = get_int( f );							/*Il secondo numero indica l'altezza del labirinto*/
	if( match( f ) != COLON )
		syntax_error( errorsymb(COLON) , f );
	else
		fseek( f , 1 , SEEK_CUR );
	nchar = get_int( f );						/*Il terzo numero indica il numero di personaggi in gioco*/
	return get_maze( l , h , nchar , f );
}

level_t get_maze( int l , int h , int n , FILE *f ){
	maze_t **prs = (maze_t **)malloc( h * sizeof(maze_t *) );
	int i, j, count = 0;
	level_t lvl = create_level( l , h , n );						/*Inizializzazione livello*/
	character *ch = (character *)malloc( n * sizeof(character) );	/*Allocazione array dei personaggi*/
	for( i=0 ; i<h ; i++ ){
		prs[i] = (maze_t *)malloc( l * sizeof(maze_t) );			/*Allocazione riga i-esima del labirinto*/
		for( j=0 ; j<l ; j++ ){
			switch( match( f ) ){
				case ENEMIEv: 	ch[++count] = create_character( l, i, j, 'v', 5, 1, follow_target );
								prs[i][j].elem = street; break;
				case ENEMIEw: 	ch[++count] = create_character( l, i, j, 'w', 5, 1, follow_target );
				case STREET:	prs[i][j].elem = street; break;
				case ESSE: 		ch[0] = create_character( l, i, j , 'c' , 1 , 2 , follow_command );
								prs[i][j].elem = street; break;
				case EFFE:		prs[i][j].elem = street; lvl.exit = get_index( l , i , j ); break;
				case WALL:		prs[i][j].elem = wall;	 break;
				case DOOR:		prs[i][j].elem = door;	 break;
				case CENTER:	prs[i][j].elem = center; break;
				case POW:		prs[i][j].elem = pow;	 break;
				case RSPWN:		lvl.respawn = get_index( l , i , j );
								prs[i][j].elem = street; break;
				case DOLLAR:	prs[i][j].elem = dollar; break;
				case KEY:		prs[i][j].elem = key; break;
				case TRAP:		prs[i][j].elem = trap; break;
				default:		syntax_error( errorsymb(STREET) , f );
			}
			prs[i][j].weight = 1.0;
			fseek( f , 1 , SEEK_CUR );
		}
	}
	ch[0].t = (int *)malloc(sizeof(int));
	ch[0].powerup = false;
	for( i=1 ; i<n ; i++ )
		if( ch[i].id == 'w' )		/*In base all'id dei nemici, gli viene assegnata una strategia di inseguimento differente*/
			ch[i].t = ch[0].t;		/*I nemici 'w' inseguono anticipando la direzione del protagonista*/
		else
			ch[i].t = ch[0].pos;	/*I nemici 'v' inseguono il protagonista andando verso la sua posizione*/
	lvl.G->nodes = (void **)prs;	/*Assegnazione del grafo al livello*/
	lvl.ch = ch;					/*Assegnazione dei personaggi al livello*/
	return lvl;
}
/*--------------------------------------------*/