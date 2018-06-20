/*-------------------------------------------------------------------*
 *  numsub()
 *
 *  Convert a subscript list to a numerical equivalent.  The
 *  numerical form may be either of two styles: 
 *
 *     'tuples'
 *        A straight numerical translation of the subscripts 
 *        into locations within the corresponding sets (based 
 *        at zero);
 *                
 *     'offsets'
 *        A single number giving an offset from the beginning 
 *        of the variable.
 *
 *  As an example, suppose a three-dimensional variable is 
 *  defined over the following sets:
 *
 *     {a} 
 *     {b,c,d}
 *     {e,f} 
 *
 *  The tuple translations of typical subscript lists would 
 *  look like the following:
 *
 *     (a,b,e) -> (0,0,0)
 *     (a,c,e) -> (0,1,0)
 *     (a,d,f) -> (0,2,1)
 *
 *  The offset translation relies on an explicit ordering of 
 *  elements within arrays.  It follows the C standard in 
 *  which elements of the right-most set vary most rapidly.  
 *  Given the sets above, the elements will be stored as 
 *  follows:
 *
 *     offset   element
 *       0      (a,b,e)
 *       1      (a,b,f)
 *       2      (a,c,e)
 *       3      (a,c,f)
 *       4      (a,d,e)
 *       5      (a,d,f)
 *  
 *  Offset translations would thus look like this:
 *  
 *     (a,b,e) -> 0
 *     (a,c,e) -> 2
 *     (a,d,f) -> 5
 * 
 *  Under offset translation, the 'offset' argument is added to 
 *  the result.
 *-------------------------------------------------------------------*/

#include "error.h"
#include "lists.h"
#include "sets.h"
#include "sym.h"
#include "symtable.h"
#include <stdio.h>

enum substyle { tuples, offsets };

static List *numsub( char*, List*, enum substyle, int );


/*-------------------------------------------------------------------*
 *  sub_offset
 *
 *  Generate an offset-style subscript.
 *-------------------------------------------------------------------*/
List *sub_offset( char *name, List *subs, int offset )
{
   return numsub(name,subs,offsets,offset);
}


/*-------------------------------------------------------------------*
 *  sub_offset
 *
 *  Generate an offset-style subscript.
 *-------------------------------------------------------------------*/
List *sub_tuple( char *name, List *subs )
{
   return numsub(name,subs,tuples,0);
}


/*-------------------------------------------------------------------*
 *  numsub
 *
 *  Private routine to do the actual work.
 *-------------------------------------------------------------------*/
static List *numsub( char *name, List *subs, enum substyle style, int offset )
{   
   void *sym;
   List *sets,*result;
   Item *set,*sub;
   int i,n,actual[20],max[20],size[20],loc;
   char buf[10];
   
   //
   //  find the symbol we're writing
   //

   sym = lookup(name);
   if( isident(sym)==0 )
      FAULT("Symbol is not an identifier in numsub");
   
   //
   //  find number of elements in each set of declaration
   //
   
   sets = symvalue(sym);
   validate( sets, LISTOBJ, "numsub" );

   if( sets->n != subs->n )
      FAULT("Incorrect number of subscripts for variable in numsub");

   n = sets->n;

   if( n > 20 )
      FAULT("Exceeded maximum subscripts (20) in numsub");

   result = newsequence();

   //
   //  handle scalars
   //
   
   if( n == 0 )
      {
      loc = (style == tuples) ? 0 : offset ;
      sprintf(buf,"%d",loc);
      addlist(result,buf);
      freelist(sets);
      return result;
      }

   //
   //  step through sets looking up subscripts
   //
   
   set = sets->first;
   sub = subs->first;
   for( i=0 ; i < n ; i++ )
      {
      max[i]    = setsize( set->str );
      actual[i] = setindex( set->str, sub->str );
   
      if( style == tuples )
         {
         sprintf(buf,"%d",actual[i]);
         addlist(result,buf);
         }
         
      set = set->next;
      sub = sub->next;
      }
   freelist(sets);

   //
   //  if style is tuples, we're done
   //

   if( style == tuples )return result;

   //
   //  ok, have an array reference that needs to be converted to
   //  its element (ravel) equivalent.  storage follows C convention
   //  and is row-major: elements at the right vary most rapidly.
   //

   size[n-1] = 1;
   for( i=n-1 ; i > 0 ; i-- )size[i-1] = size[i]*max[i];
   
   loc = 0;
   for( i=0 ; i<n ; i++ )loc += actual[i]*size[i];
   
   loc += offset;

   sprintf(buf,"%d",loc);
   addlist(result,buf);
   
   return result;
}



