/*--------------------------------------------------------------------*
 *  wprint.h
 *
 *  $Id: wprint.h 49 2018-06-16 17:15:39Z wilcoxen $
 *--------------------------------------------------------------------*/

#ifndef WPRINT_H
#define WPRINT_H

#include "lists.h"
#include <stdio.h>

void  free_wprint( void *wp );
void  wprint_add( void *wp, char *str );
void  wprint_break( void *wp );
void  wprint_end( void *wp );
void  wprint_list( void *wp, List *list, char *join );
void  wprint_start( void *wp, char *str );
void* new_wprint( FILE *fp, int margin );

#endif /* WPRINT_H */
