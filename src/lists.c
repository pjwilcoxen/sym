/*--------------------------------------------------------------------*
 *  LISTS.C 2.40
 *  Mar 04 (PJW)
 *
 *  Basic routines for managing lists of strings
 *--------------------------------------------------------------------*/

#include "lists.h"

#include "error.h"
#include "str.h"
#include "sym.h"
#include "xmalloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SLBUFSIZE 8096

static int nlists=0;


/*--------------------------------------------------------------------*
 * newlist: create a new list
 *--------------------------------------------------------------------*/
List *newlist()
{
   List *new;

   new = (List *) xmalloc( sizeof(List) );
   new->obj   = LISTOBJ;
   new->id    = ++nlists;
   new->n     = 0;
   new->first = 0;
   new->sort  = 1;   
   return new;
}


/*--------------------------------------------------------------------*
 * newsequence: create a new ordered list
 *--------------------------------------------------------------------*/
List *newsequence()
{
   List *new;

   new = newlist();
   new->sort = 0;   
   return new;
}


/*--------------------------------------------------------------------*
 * duplist: copy a list, preserving its characteristics
 *--------------------------------------------------------------------*/
List *duplist( List *oldlist )
{
   List *new;
   validate( oldlist, LISTOBJ, "duplist" );
   if( oldlist->sort )
      new = newlist();
   else
      new = newsequence();
   catlist( new, oldlist );
   return new;
}


/*--------------------------------------------------------------------*
 * newlistitem: create a new list item
 *--------------------------------------------------------------------*/
Item *newlistitem(string)
char *string;
{
   Item *new;
   new = (Item *) xmalloc( sizeof(Item) );
   new->obj  = ITEMOBJ;
   new->str  = strdup( string );
   new->next = 0;
   return new;
}


/*--------------------------------------------------------------------*
 * catlist: concatenate lists
 *--------------------------------------------------------------------*/
List *catlist(target,source)
List *target,*source;
{
   Item *item;

   validate( target, LISTOBJ, "catlist" );
   validate( source, LISTOBJ, "catlist" );

   for( item=source->first ; item ; item=item->next )
      addlist( target, item->str );

   return target;
}


/*--------------------------------------------------------------------*
 * freeitem: delete a new list item
 *--------------------------------------------------------------------*/
void freeitem(list,string)
List *list;
char *string;
{
   Item *item,*prev;

   validate( list, LISTOBJ, "freeitem" );

   item = list->first;

   if( list->n < 1 )
      fatal_error("%s","internal error in freeitem");

   if( isequal(string,item->str) )
      {
      list->first = item->next;
      free(item->str);
      xfree(item);
      list->n--;
      return;
      }
   
   while( item->next )
      {
      prev = item ;
      item = item->next;
      if( isequal(string,item->str) )
         {
         prev->next = item->next;
         free(item->str);
         xfree(item);
         list->n--;
         return;
         }
      }

   fatal_error("%s","internal error in freeitem");
}


/*--------------------------------------------------------------------*
 * freelist: free a list and all of its items
 *--------------------------------------------------------------------*/
List *freelist(list)
List *list;
{
   Item *item,*next;

   if( list==0 )return 0;

   validate( list, LISTOBJ, "freelist" );

   item = list->first;
   while( item )
      {
      next = item->next ;
      free( item->str );
      xfree( item );
      item = next;
      }

   xfree( list );

   return 0;
}


/*--------------------------------------------------------------------*
 *  addlist
 *
 *  Add an item to a list.  If sort is set, ignores duplicates and 
 *  keeps the list sorted in alphabetical order.  If sort is not
 *  set, keep all elements in order added.
 *--------------------------------------------------------------------*/
List *addlist(list,string)
List *list;
char *string;
{
   Item *new,*cur,*nxt;
   int check;
   
   validate( list, LISTOBJ, "addlist" );

   cur = list->first;

   if( cur == 0 )
      {
      list->first = newlistitem(string);
      list->n++;
      return list;
      }

   if( list->sort == 0 )
      {
      while( cur->next )cur=cur->next;
      cur->next = newlistitem(string);
      list->n++;
      return list;
      }
      
   check = strcasecmp(string,cur->str);
   if( check == 0 )return list;

   if( check  < 0  )
      {
      new = newlistitem(string);
      list->first = new;
      new->next   = cur;
      list->n++;
      return list;
      }
   
   for( nxt=cur->next ; nxt ; nxt=nxt->next )
      {
      check = strcasecmp(string,nxt->str);
      if( check == 0 )return list;
      if( check  < 0 )
         {
         new = newlistitem(string);
         cur->next = new;
         new->next = nxt;
         list->n++;
         return list;
         }
      cur=nxt; 
      }
            
   cur->next = newlistitem(string);
   list->n++;
   return list;
}


/*--------------------------------------------------------------------*
 *  ismember 
 *--------------------------------------------------------------------*/
int ismember(string,list)
char *string;
List *list;
{
   Item *cur;
   int index;
   
   validate( list, LISTOBJ, "ismember" );

   for( index=1, cur=list->first ; cur ; index++, cur=cur->next )
      if( isequal(string,cur->str) )return index;

   return 0;
}


/*--------------------------------------------------------------------*
 *  printlist
 *--------------------------------------------------------------------*/
void printlist(list)
List *list;
{
   Item *cur;
   int n=0;
   
   validate( list, LISTOBJ, "printlist" );
   
   for( cur=list->first ; cur ; cur=cur->next )
      if( 0 == n++ )
         printf(cur->str);
      else
         printf(",%s",cur->str);

   return;
}


/*--------------------------------------------------------------------*
 *  slprint
 *
 *  Streamlined printing with built-in buffer.
 *--------------------------------------------------------------------*/
char *slprint(list)
List *list;
{
   static char obuf[SLBUFSIZE];
   Item *cur;
   int n=0;
   int newlen;
   
   validate( list, LISTOBJ, "slprint" );

   strcpy(obuf,"");

   for( cur=list->first ; cur ; cur=cur->next )
      {
      newlen = strlen(obuf)+strlen(cur->str)+2;
      if( newlen > SLBUFSIZE ) 
         fatal_error("%s","buffer size exceeded in slprint");
      if( 0 == n++ )
         strcpy(obuf,cur->str);
      else
         {
         strcat(obuf,",");
         strcat(obuf,cur->str);
         }
      }

   return obuf;
}

/*-------------------------------------------------------------------*
 *  intersect
 *
 *  Return the intersection of two lists
 *-------------------------------------------------------------------*/
List *intersect(List *a, List *b)
{
   List *result;
   Item *ai;
   
   validate( a, LISTOBJ, "intersect" );
   validate( b, LISTOBJ, "intersect" );
   
   result = newlist();

   for( ai=a->first ; ai ; ai=ai->next )
      if( ismember(ai->str,b) )
         addlist( result, ai->str );

   return result;
}

