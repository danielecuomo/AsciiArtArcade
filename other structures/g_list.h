#ifndef G_LIST_H_INCLUDED
#define G_LIST_H_INCLUDED

#include "f_graph.h"

	/* ==========  ========== ========== ========= */
	/*      Grafi - Libreria per la gestione       */
	/*      Autore - Daniele Cuomo N8601346        */
	/* ========== ========== ========== ========== */

void create_adj_L( graph * , int , double );

void *start_conv_L( void * , int );

void delete_v_L( void ** );

void del_rmnt_L( void ** , int , int );

void delete_e_L( void ** , int );

bool is_adj_L( void ** , int );

void update_e_L( void ** , int );

int adiac_L( void * , int , int );

void invert_L( graph * , void ** , int );

void *nulledge_L( int );

/* ------- Funzione per il parsing ------- */
void set_edge_L( void ** , float , int );
/* ------- ----------------------- ------- */

#endif
