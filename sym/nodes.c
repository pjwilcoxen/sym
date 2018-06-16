/*--------------------------------------------------------------------*
 *  NODES.C 2.40
 *  26 Mar 90 (PJW)
 *
 *  Basic routines for managing the nodes of parse trees.
 *--------------------------------------------------------------------*/

#include "nodes.h"

#include "error.h"
#include "sym.h"
#include "xmalloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// turn myDEBUG on to get the parse tree listed on stdout

#define  myDEBUG 0

static int n_alloc;
static int n_free;
static Node *last=0;

/*--------------------------------------------------------------------*
 * NEWNODE: create a new node
 *--------------------------------------------------------------------*/
Node *newnode(Nodetype kind, char *string, Node *left, Node *right)
{
   Node *new;

   if( ! string )FAULT("Null string passed to newnode");
   if( left     )validate( left,  NODEOBJ, "newnode" );
   if( right    )validate( right, NODEOBJ, "newnode" );

#if myDEBUG
    printf("node: op=%s",string);
    if( left && left->str )printf(", l=%s",left->str);
    if( right && right->str )printf(", r=%s",right->str);
    printf("\n");
    if( kind==equ )printf("\n");
#endif

   new = (Node *) xmalloc( sizeof(Node) );
   new->obj    = NODEOBJ;
   new->l      = left;
   new->r      = right;
   new->type   = kind;
   new->str    = strdup( string );
   new->domain = 0;
   new->undec  = 0;
   new->lhs    = 0;
   new->dt     = 0;

   last = new;
   n_alloc++;
   return new;
}


/*--------------------------------------------------------------------*
 * freenode: remove (delete) a node and everything below it
 *--------------------------------------------------------------------*/
void freenode(Node *cur)
{
   if( cur==0 )return;

   validate( cur, NODEOBJ, "freenode" );

   if( cur->l   )freenode( cur->l );
   if( cur->r   )freenode( cur->r );

   if( cur->str )free( cur->str );
   freelist( cur->domain );
   xfree( cur );

   if( cur==last )last = 0;

   n_free++;
}


/*--------------------------------------------------------------------*
 *  getlastnode()
 *
 *  Return the string value of the 'last' node pointer, which is the 
 *  last node constructed by newnode.  This can be used when dealing 
 *  with syntax errors because it gives the portion of the
 *  equation that had been parsed correctly before the error 
 *  occurred.
 *--------------------------------------------------------------------*/
char *getlastnode()
{
   if( last==0 )return 0;
   return snprint(last);
}


/*--------------------------------------------------------------------*
 *  resetlastnode()
 *
 *  Reset the last pointer to null.  This routine should be used
 *  to reset 'last' at the beginning of each new equation.
 *--------------------------------------------------------------------*/
void resetlastnode()
{
   last = 0;
}
