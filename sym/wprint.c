/*--------------------------------------------------------------------*
 *  wprint.c
 *  Apr 08 (PJW)
 *
 *  Print with wrapping and hanging margins.
 *--------------------------------------------------------------------*/

#include "wprint.h"

#include "error.h"
#include "sym.h"
#include "xmalloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WPRINT_OBJ 404
#define MAXLINE    1024
#define MAXPAD     80

typedef struct  
   {
   int obj;
   FILE *fp;
   int margin;
   char buf[MAXLINE+1];
   char pad[MAXPAD+1];
   int len;
   int bk;
   }
   WPrinter ;


/*--------------------------------------------------------------------*
 *  wprint_flush: print out anything in the buffer
 *--------------------------------------------------------------------*/
static void wprint_flush(void *vp)
{
   WPrinter *wp;

   validate( vp, WPRINT_OBJ, "wprint_flush" );
   wp = (WPrinter *) vp;
   
   if( wp->len == 0 )return ;

   fprintf(wp->fp,"%s",wp->buf);
   fprintf(wp->fp,"\n");
   strcpy(wp->buf,"");

   wp->len = 0;
   wp->bk  = 0;
}


/*--------------------------------------------------------------------*
 *  new_wprint: construct a new WPrinter object
 *--------------------------------------------------------------------*/
void *new_wprint(FILE *fp, int margin)
{
   WPrinter *new;

   if( margin > MAXLINE )
      fatal_error("%s","line too long in new_wprint");

   new = (WPrinter *) xmalloc( sizeof(WPrinter) );
   new->obj    = WPRINT_OBJ;
   new->fp     = fp;
   new->margin = margin;
   strcpy(new->buf,"");
   strcpy(new->pad,"");
   new->len    = 0;
   new->bk     = 0;

   return new;
}


/*--------------------------------------------------------------------*
 *  free_wprint: destroy a WPrinter object
 *--------------------------------------------------------------------*/
void free_wprint(void *vp)
{
   WPrinter *wp;

   validate( vp, WPRINT_OBJ, "free_wprint" );
   wp = (WPrinter *) vp;

   if( wp->len )
      fatal_error("%s","buffer not empty at call to free_wprint");

   xfree(wp);
}


/*--------------------------------------------------------------------*
 *  wprint_start: the first part of a line
 *--------------------------------------------------------------------*/
void wprint_start(void *vp, char *str)
{
   int i;
   WPrinter *wp;

   validate( vp, WPRINT_OBJ, "wprint_start" );
   wp = (WPrinter *) vp;
   
   wprint_flush(wp);

   strcpy(wp->buf,str);
   wp->len = strlen(str);
   wp->bk  = wp->len;

   if( wp->len == strlen(wp->pad) )
      return;

   if( wp->len > MAXPAD )
      {
      strcpy(wp->pad,"");
      return;
      }

   for( i=0 ; i < wp->len ; i++ )
      wp->pad[i] = ' ';

   wp->pad[i] = '\0';
}


/*--------------------------------------------------------------------*
 *  wprint_add: add something to the end of a line
 *--------------------------------------------------------------------*/
void wprint_add(void *vp, char *str)
{
   int newlen;
   char *out,*tail;
   WPrinter *wp;

   validate( vp, WPRINT_OBJ, "wprint_add" );
   wp = (WPrinter *) vp;

   newlen = wp->len + strlen(str);

   if( newlen < wp->margin )
      {
      strcat( wp->buf, str);   
      wp->len = newlen;
      return;
      }

   out = strdup(wp->buf);
   out[wp->bk] = '\0';
   fprintf(wp->fp,"%s",out);
   fprintf(wp->fp,"\n");
   free(out);

   tail = strdup(wp->buf);
   strcpy( wp->buf, wp->pad );
   strcat( wp->buf, (tail+wp->bk) );
   strcat( wp->buf, str );
   free(tail);

   wp->len = strlen(wp->buf);
   wp->bk = wp->len;
}


/*--------------------------------------------------------------------*
 *  wprint_break: set the high-water break location
 *--------------------------------------------------------------------*/
void wprint_break(void *vp)
{
   WPrinter *wp;
   validate( vp, WPRINT_OBJ, "wprint_break" );
   wp = (WPrinter *) vp;
   wp->bk = wp->len;
}


/*--------------------------------------------------------------------*
 *  wprint_end: end this line
 *--------------------------------------------------------------------*/
void wprint_end(void *vp)
{
   validate( vp, WPRINT_OBJ, "wprint_end" );
   wprint_flush(vp);
}


/*--------------------------------------------------------------------*
 *  wprint_list: add a list
 *--------------------------------------------------------------------*/
void wprint_list(void *vp, List *list, char *join)
{
   Item *item;

   validate( vp  , WPRINT_OBJ, "wprint_list" );
   validate( list, LISTOBJ   , "wprint_list" );
   validate( join, 0         , "wprint_list" );

   for( item=list->first ; item ; item=item->next )
      {
      wprint_add(vp,item->str);
      if( item->next )wprint_add(vp,join);
      wprint_break(vp);
      }
}

