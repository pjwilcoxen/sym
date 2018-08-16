/*--------------------------------------------------------------------*
 *  eqns.h
 *
 *  $Id: eqns.h 49 2018-06-16 17:15:39Z wilcoxen $
 *--------------------------------------------------------------------*/

#ifndef EQNS_H
#define EQNS_H

#include "nodes.h"

Node* getnode(void*);
char* eqnlabel(void*);
int   eqncount(void*);
int   hasundec(void*);
int   iseqnattr(void*,char*);
int   islvalue(void*);
int   istimeok(void*);
int   num_eqns(void);
void  build_context(void);
void  check_equations();
void  neweqn(Node*,Node*,Node*,Node*,Node*,Node*);
void* firsteqn(void);
void* nexteqn(void*);

#endif /* EQNS_H */
