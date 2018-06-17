/*--------------------------------------------------------------------*
*  ASSOC.C 2.40
*  Dec 04 (PJW)
*
*  Basic routines for managing associative arrays.
*--------------------------------------------------------------------*/

#include "assoc.h"

#include "error.h"
#include "str.h"
#include "sym.h"
#include "xmalloc.h"
#include <stdio.h>
#include <string.h>

static int nassoc=0;

/*--------------------------------------------------------------------*
* newarray
*--------------------------------------------------------------------*/
Array *newarray()
{
Array *new;

new = (Array *) xmalloc( sizeof(Array) );
new->obj   = ARRAYOBJ;
new->id    = ++nassoc;
new->n     = 0;
new->first = 0;

return new;
}


/*--------------------------------------------------------------------*
* newelement
*--------------------------------------------------------------------*/
static Element *newelement(char *name, void *value)
{
Element *new;

new = (Element *) xmalloc( sizeof(Element) );
new->obj   = ELEOBJ;
new->name  = strdup(name);
new->value = value;
new->next  = 0;

return new;
}


/*--------------------------------------------------------------------*
*  addvalue()
*
*  Add an element.  The element must not already exist.  To add or 
*  create an element, use setvalue() instead.
*--------------------------------------------------------------------*/
int addvalue(Array *array, char *name, void *value)
{
Element *cur,*new,*nxt;
int check;

validate( array, ARRAYOBJ, "addvalue" );

cur = array->first;

if( cur == 0 )
   {
   array->first = newelement(name,value);
   array->n++;
   return 1;
   }

check = strcasecmp(name,cur->name);
if( check == 0 )
   FAULT("Element already exists when addvalue called");
   
if( check  < 0  )
   {
   new = newelement(name,value);
   array->first = new;
   new->next    = cur;
   array->n++;
   return 1;
   }

for( nxt=cur->next ; nxt ; nxt=nxt->next )
   {
   check = strcasecmp(name,nxt->name);
   if( check == 0 )
      FAULT("Element already exists when addvalue called");
   if( check  < 0 )
      {
      new = newelement(name,value);
      cur->next = new;
      new->next = nxt;
      array->n++;
      return 1;
      }
   cur=nxt; 
   }
         
cur->next = newelement(name,value);
array->n++;
return 1;
}


/*--------------------------------------------------------------------*
*  getvalue()
*--------------------------------------------------------------------*/
void *getvalue(Array *array, char *name)
{
Element *cur;

validate( array, ARRAYOBJ, "getelement" );

for( cur=array->first ; cur ; cur=cur->next )
   if( strcasecmp(name,cur->name)==0 )
         return cur->value;

   return 0;
}


/*--------------------------------------------------------------------*
 *  setvalue()
 *--------------------------------------------------------------------*/
void setvalue(Array *array, char *name, void *value)
{
   Element *cur;

   validate( array, ARRAYOBJ, "getelement" );

   for( cur=array->first ; cur ; cur=cur->next )
      if( strcasecmp(name,cur->name)==0 )
         {
         cur->value = value;
         return;
         }

   addvalue(array,name,value);
}

