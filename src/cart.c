/*-------------------------------------------------------------------*
 *  cart.c
 *  Dec 04 (PJW)
 *
 *  Convenience routines for generating and then walking through a
 *  Cartesian product of a list of sets.  
 *
 *  The public functions are cart_build, which causes the internal
 *  table of tuples to be built; and cart_first and cart_next, 
 *  do exactly what one would expect.  The functions should be 
 *  used as follows:
 *
 *     List *cur;
 *     List *sets;
 *     
 *     cart_build(sets);
 *     for( cur=cart_first() ; cur ; cur=cart_next() )
 *        {
 *        ...
 *        }
 *
 *  Everything else is handled internally via static buffers.
 *  Because static buffers are used, only one Cartesian product
 *  can be generated at a time.
 *  
 *  $Id: cart.c 57 2018-06-16 19:50:13Z wilcoxen $
 *-------------------------------------------------------------------*/

#include "cart.h"

#include "error.h"
#include "sets.h"
#include "sym.h"
#include "xmalloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define  myDEBUG 1

#define SUBTOBJ 2005

//
//  Internal objects used to keep track of what we're doing
//

struct tuple {
   int obj;
   List *subs;
   struct tuple *next;
   };

static struct tuple *head=0;
static struct tuple *last=0;
static struct tuple *next=0;

static void gen_sublist(List*,List*);


//----------------------------------------------------------------------//
//  cart_build
//
//  Initialize the list of tuples and then call gen_tuples to fill
//  it recursively.
//----------------------------------------------------------------------//

void cart_build(List *setlist)
{
   struct tuple *cur,*nxt;
   List *myelements;
    
   validate( setlist, LISTOBJ, "cart_build" );

   if( DBG )
      printf("cart_build on sets: %s\n",slprint(setlist));

   //  
   //  clear out the current table
   //  

   for( cur=head ; cur ; cur=nxt )
      {
      validate( cur, SUBTOBJ, "build_sublist" );
      nxt = cur->next;
      freelist( cur->subs );
      free(cur);
      }
      
   //
   //  build the table recursively
   //

   head = (struct tuple *) xmalloc( sizeof(struct tuple) );
   head->subs = 0;
   head->obj  = SUBTOBJ;
   head->next = 0;
   last = head;
    
   myelements = newsequence();
   gen_sublist(myelements,setlist);
   freelist(myelements);

   if( head==0 )
      FAULT("Failed to build subscript list in build_sublist");

   next = head->next;
}


//----------------------------------------------------------------------//
//  cart_next()
//
//  Get the next tuple.
//----------------------------------------------------------------------//

List *cart_next()
{
   List *thislist;
   
   if( head==0 )
      FAULT("Cart before horse: cart_next called before cart_build");

   if( next == 0 )return 0;
   
   validate( next, SUBTOBJ, "get_element" );

   thislist = next->subs;
   next = next->next;

   return thislist;
}

//----------------------------------------------------------------------//
//  cart_first()
//
//  Reset the internal pointer to the first tuple and return it.
//----------------------------------------------------------------------//

List *cart_first()
{
   next = head->next;
   return cart_next();
}



//----------------------------------------------------------------------//
//  gen_sublist
//
//  Recursively generates subscipts for consecutive elements of an 
//  identifier.  
//
//  Given a list of subscripts already chosen from some of the sets 
//  over which a variable is defined, and a list of remaining sets, 
//  gen_sublist operates as follows:
//
//     * shift the first of the remaining sets off the list
//     
//     * for each element of the shifted set:
//     
//       - create a new element list with the element concatenated onto
//         cur_elements;
//         
//       - call ourself on the new list and the tail of the set list
//----------------------------------------------------------------------//

static void gen_sublist(List *cur_elements, List *remaining_sets )
{
   struct tuple *new;
   Item *first_set,*cur;
   List *elements,*rest_sets,*new_elements;
   
   // check for stuffed arguments

   validate( cur_elements,   LISTOBJ, "gen_varmap" );
   validate( remaining_sets, LISTOBJ, "gen_varmap" );
  
   // check for an empty list; if so, we're at the bottom of the 
   // recursion and have a complete list of subscripts.  add the 
   // list to the table and return.
   
   if( remaining_sets->n == 0 )
      {
      validate( last, SUBTOBJ, "gen_sublist" );
      
      new = (struct tuple *) xmalloc( sizeof(struct tuple) );
      new->subs = duplist( cur_elements );
      new->obj  = SUBTOBJ;
      new->next = 0;

      last->next = new;
      last = new;
      return;
      }
   
   // shift the first set off the list of remaining sets
   
   first_set = remaining_sets->first;
   
   rest_sets = newsequence();
   for( cur=first_set->next ; cur ; cur=cur->next )
      addlist( rest_sets, cur->str );

   // get the elements of first_set
   
   elements = setelements(first_set->str);

   // loop through the elements and call ourself
   // recursively to process remaining sets.

   for( cur=elements->first ; cur ; cur=cur->next )      
      {
      new_elements = newsequence();
      catlist( new_elements, cur_elements );
      addlist( new_elements, cur->str );
      gen_sublist( new_elements, rest_sets );
      freelist( new_elements );
      }

   freelist(elements);
   freelist(rest_sets);
}


