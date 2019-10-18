/*--------------------------------------------------------------------*
 *  sets.h
 *
 *  $Id: sets.h 49 2018-06-16 17:15:39Z wilcoxen $
 *--------------------------------------------------------------------*/

#ifndef SETS_H
#define SETS_H

#include "lists.h"

List* setelements(char *);
int   isaliasof(char*,char*);
int   isimplicit(char*);
int   issubset(char*,char*);
int   setindex(char *,char *);
int   setsize(char *);
void  build_set_relationships();
void  listelements();
void  setalias(char*,char*);
void  setsubset(char*,char*);
char* findbase(char*);
List* find_immediate_sups(char *);

#endif /* SETS_H */
