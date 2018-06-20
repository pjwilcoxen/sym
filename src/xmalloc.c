/*-------------------------------------------------------------------*
 *  XMALLOC.C 1.10
 *  9 Apr 90 (PJW)
 *
 *  Careful memory allocation routine to try to control leaks
 *-------------------------------------------------------------------*/

#include "xmalloc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static long x_alloc=0;
static long x_check=0;

void fatal_error(char*,char*);

void *xmalloc(int size)
{
   void *c;
   c = malloc( size );
   if( c==0 ) fatal_error("%s","insufficient memory in xmalloc()");
   x_alloc++;
   return c;
}


char *xstrdup(char *str)
{
   char *new;
   if( str==0 ) fatal_error("%s","null pointer passed to xstrdup()");
   new = (char *) xmalloc( strlen(str)+1 );
   strcpy(new,str);
   return new;
}


void xfree(void *ptr)
{
   if( ptr==0 ) fatal_error("%s","null pointer passed to xfree()");
   free( ptr );
   x_alloc--;
}


void xcheck(char *where)
{
   printf("xcheck at %s: %ld\n",where,x_alloc-x_check);
   x_check = x_alloc;
}


long xmark()
{
   return x_alloc;
}


void xleak(long mark, char *where)
{
   if( x_alloc == mark )return;
   printf("Memory leak detected in %s, allocation change was %ld\n",where,x_alloc-mark);
   exit(0);
}

