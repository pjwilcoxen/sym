/*--------------------------------------------------------------------*
 *  nodes.h
 *
 *  $Id: nodes.h 49 2018-06-16 17:15:39Z wilcoxen $
 *--------------------------------------------------------------------*/

#ifndef NODES_H
#define NODES_H

#include "lists.h"

/* Types of nodes */

typedef enum
   {
   nul, nam, num, lst, dom,
   add, sub, mul, dvd, pow, log, exp, neg,
   lag, led, sum, prd,
   sad, ssu,
   equ
   }
   Nodetype ;

/* Structure of a node */

typedef struct node_struct
   {
   int obj;    
   char *str;              // name or label
   Nodetype type;          // type
   struct node_struct *l;  // left child
   struct node_struct *r;  // right child
   List *domain;           // node's domain
   int undec;              // this or child is undeclared
   int lhs;                // context: on lhs
   int dt;                 // context: dt
   }
   Node ;

Node* newnode(Nodetype,char*,Node*,Node*);
char* node2string(Node*);
char* snprint(Node*);
void  freenode(Node*);

/* Handy macros */

#define isname(nde)	 ( nde && nde->type==nam )
#define isnumber(nde) ( nde && nde->type==num )
#define isone(nde)	 ( nde && nde->type==num && strcmp(nde->str,"1")==0 )
#define iszero(nde)	 ( nde && nde->type==num && strcmp(nde->str,"0")==0 )

#endif /* NODES_H */
