/*--------------------------------------------------------------------*
 *  SYMTABLE.C 2.50
 *  27 Mar 90 (PJW)
 *
 *  Symbol table management routines.
 *--------------------------------------------------------------------*/

#include "symtable.h"

#include "error.h"
#include "nodes.h"
#include "options.h"
#include "sets.h"
#include "str.h"
#include "sym.h"
#include "xmalloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define  myDEBUG 0

//=====================================================================
//
//  Private data structures and routines
//  
//=====================================================================

#define NOSIZE -1

struct symbol
   {
   int  obj;
   char *str;
   Symboltype type;
   char *desc;
   List *value;
   List *attr;
   int  size;
   int  used;
   List *leqns;
   List *reqns;
   struct symbol *next;
   };

typedef struct symbol Symbol;

Symbol st_head;
int st_init=0;
int n_symtab=0;


/*-------------------------------------------------------------------*
 *  initsymbol
 *
 *  Initialize the symbol table itself.
 *-------------------------------------------------------------------*/
static void initsymbol()
{
   st_head.str  = 0;
   st_head.next = 0;
   st_init      = 1;
   n_symtab     = 0;
}


/*-------------------------------------------------------------------*
 *  newsymbol
 *
 *  Create a new entry in the symbol table.
 *-------------------------------------------------------------------*/
static Symbol *newsymbol( char *name, Symboltype type)
{
   int check;
   Symbol *new,*cur,*pre;

   if( name==0 )
      FAULT("Null string passed to newsymbol" );
   
   if( !st_init )
      initsymbol();

   if( DBG )
      printf("defining new symbol %s\n",name);

   for( pre = &st_head ; pre ; pre=cur )
      {
      cur = pre->next;
      check = (cur) ? strcasecmp(name,cur->str) : -1 ;

      if( check > 0 )
         continue;

      if( check == 0 )
         fatal_error("Multiple definitions of '%s'",name);

      new = (Symbol *) xmalloc( sizeof(Symbol) );
      new->obj   = SYMBOBJ;
      new->str   = strdup(name);
      new->next  = cur;
      pre->next  = new;
      new->type  = type;
      new->desc  = strdup("");
      new->value = newsequence();
      new->attr  = newsequence();
      new->size  = NOSIZE;
      new->used  = 0;
      new->leqns = newlist();
      new->reqns = newlist();

      n_symtab++;

      if( DBG )printf("   ok\n");
      return new;
      }

      FAULT("Invalid state reached in do_newsymbol");
      return 0;
}


//=====================================================================
//
//  Public routines
//  
//=====================================================================


/*-------------------------------------------------------------------*
 *  lookup 
 *
 *  Find a symbol given its name.
 *-------------------------------------------------------------------*/
void *lookup(char *name)
{
   Symbol *cur;

   if( !st_init )
      FAULT("Symbol table not built before lookup");

   if( name==0 )
      FAULT("Null string passed to lookup");

   for( cur = st_head.next ; cur ; cur=cur->next )
      if( cur->str && isequal(name,cur->str) )
         return (void*) cur;

   return 0 ;
}


/*-------------------------------------------------------------------*
 *  firstsymbol
 *
 *  Find and return the first symbol of a given type.
 *-------------------------------------------------------------------*/
void *firstsymbol(Symboltype type)
{
   Symbol *cur;

   if( !st_init )
      FAULT("Symbol table not built before firstsymbol");

   for( cur=st_head.next ; cur ; cur=cur->next )
      if( type==cur->type )return cur ;

   return 0;
}


/*-------------------------------------------------------------------*
 *  nextsymbol
 *
 *  Find and return the next symbol of the same type as the argument
 *-------------------------------------------------------------------*/
void *nextsymbol(void *cur)
{
   Symboltype type;
   Symbol *sym;

   if( !st_init )
      FAULT("Symbol table not built before nextsymbol");

   validate( cur, SYMBOBJ, "nextsymbol" );

   sym = (Symbol *) cur;
   type = sym->type;

   while( sym=sym->next )
      if( type==sym->type )return sym ;

   return 0;
}


/*-------------------------------------------------------------------*
 *  istype()
 *
 *  Check the type of a symbol.  Some derivative functions also
 *  provided for convenience: isident, isset
 *-------------------------------------------------------------------*/
int istype(void *sym, Symboltype type)
{
   if( sym==0 )return 0;
   validate( sym, SYMBOBJ, "istype" ); 
   return ((Symbol*) sym)->type == type;
}

//  
//  isident
//  

int isident(void *sym)
{
   return ( istype(sym,par) || istype(sym,var) );
}

//
//  validatetype
//

void validatetype(void *sym, Symboltype type, char *where )
{
   validate( sym, SYMBOBJ, "validatetype" ); 
   if( istype(sym,type) )return;
   fatal_error("Invalid symbol type encountered in %s",where);
}


/*-------------------------------------------------------------------*
 *  symname()
 *
 *  Return a duplicate of the symbol name.
 *-------------------------------------------------------------------*/
char *symname(void *sym)
{
   validate( sym, SYMBOBJ, "getvalue" ); 
   return strdup( ((Symbol*) sym)->str );
}


/*-------------------------------------------------------------------*
 *  symvalue()
 *
 *  Return a duplicate of the symbol's value.
 *-------------------------------------------------------------------*/
List *symvalue(void *sym)
{
   validate( sym, SYMBOBJ, "symvalue" ); 
   return duplist( ((Symbol*) sym)->value );
}


/*-------------------------------------------------------------------*
 *  symattrib()
 *
 *  Return a duplicate of the symbol's attributes.
 *-------------------------------------------------------------------*/
List *symattrib(void *sym)
{
   validate( sym, SYMBOBJ, "symattrib" ); 
   return duplist( ((Symbol*) sym)->attr );
}


/*-------------------------------------------------------------------*
 *  isattrib()
 *
 *  Check whether a given symbol has a particular attribute.
 *-------------------------------------------------------------------*/
int isattrib(void *sym, char *attr)
{
   validate( sym, SYMBOBJ, "isattrib" ); 
   return ismember( attr, ((Symbol*) sym)->attr );
}


/*-------------------------------------------------------------------*
 *  symdescrip()
 *
 *  Return a duplicate of the symbol's description string.
 *-------------------------------------------------------------------*/
char *symdescrip(void *sym)
{
   validate( sym, SYMBOBJ, "symdescrip" ); 
   return strdup( ((Symbol*) sym)->desc );
}


/*-------------------------------------------------------------------*
 *  symsize()
 *
 *  Return the size of the symbol.  The symbol must exist.
 *-------------------------------------------------------------------*/
int symsize(void *sym)
{
   Symbol *cur;
   List *setlist;
   Item *ci;
   int count;

   validate( sym, SYMBOBJ, "symsize" ); 
   cur = (Symbol *) sym;

   if( cur->size != NOSIZE )
      return cur->size;
   
   switch( cur->type )
      {
      case set:
         count = (cur->value)->n;
         break;

      case par:
      case var:
         count = 1;
         setlist = cur->value;
         validate( setlist, LISTOBJ, "getsize" );
         for( ci=setlist->first ; ci ; ci=ci->next )
            count = count*setsize(ci->str);
         break;

      default:
         FAULT("Invalid symbol type for getsize");
         break;
      }

   cur->size = count;
   
   return count;
}

 
/*--------------------------------------------------------------------*
 *  symdeclare()
 *
 *  Create a new symbol; called from declare() in declare.c after
 *  nodes have been parsed.  Arguments can be freed after symdeclare 
 *  returns. 
 *--------------------------------------------------------------------*/

void symdeclare(symtype, name, defn, label, attr)
Symboltype symtype;
char *name;
List *defn;
char *label;
List *attr;
{
   Symbol *sym;

   //
   //  check arguments
   //
   
   if( defn )validate( defn, LISTOBJ, "symdeclare" );
   if( attr )validate( attr, LISTOBJ, "symdeclare" );
   
   //  
   //  the defn argument is required for sets 
   //  
   
   if( symtype==set && defn==0 )
      FAULT("No definition for set in symdeclare");
   
   //
   //  create the symbol
   //

   sym = newsymbol(name,symtype);

   //
   //  set its value
   //
   
   if( defn )catlist( sym->value, defn );

   //
   //  add a time set to each variable if the target language 
   //  requires it and one wasn't specified
   //

   if( symtype==var && is_explicit_time() )
      if( ! (ismember("time",defn) || ismember("first",defn) || ismember("last",defn)) )
         addlist( sym->value, "time" );
         
   //
   //  set its label or comment string
   //

   if( label )
      {
      free( sym->desc );
      sym->desc = strdup( label );
      }

   //
   //  set its attribute list
   //

   if( attr )catlist( sym->attr, attr );
}


/*-------------------------------------------------------------------*
 *  check_identifiers
 *
 *  Look for problems in the domains of identifiers.  Should be
 *  called after build_set_relationships().
 *-------------------------------------------------------------------*/
void check_identifiers()
{
   Symbol *cur;
   Item *a,*b;
   int ok;
   
   for( cur = st_head.next ; cur ; cur=cur->next )
      {
      if( cur->type != par && cur->type != var )continue;
      if( cur->value->n < 2 )continue;
      
      for( a=cur->value->first ; a ; a=a->next )
         for( b=a->next ; b ; b=b->next )
            {
            ok = 1;
            if( issubset(a->str,b->str) || isaliasof(a->str,b->str) )ok = 0;
            if( issubset(b->str,a->str) || isaliasof(b->str,a->str) )ok = 0;
            if( ok==0 )
               error_front("Domain of %s is invalid: some sets are subsets or aliases of others",cur->str);
            }
      }
}


/*-------------------------------------------------------------------*
 *  setused
 *
 *  Mark a symbol as being used in an equation.
 *-------------------------------------------------------------------*/
void setused(void *sym, int eq_num, int is_lhs )
{
   static char buf[6];
   if( sym==0 )return;
   validate( sym, SYMBOBJ, "setused" ); 
   ((Symbol *)sym)->used = 1;
   
   if( eq_num>99999 )
      FAULT("Equation number exceeds 99999 in setused");
   sprintf(buf,"%d",eq_num);

   if( is_lhs )
      addlist( ((Symbol *)sym)->leqns, buf );
   else
      addlist( ((Symbol *)sym)->reqns, buf );
}


/*-------------------------------------------------------------------*
 *  isused
 *
 *  Check whether a symbol has been used in an equation.
 *-------------------------------------------------------------------*/
int isused(void *sym)
{
   if( sym==0 )return 0;
   validate( sym, SYMBOBJ, "isused" ); 
   return ((Symbol *)sym)->used ;
}


/*-------------------------------------------------------------------*
 *  symdef
 *
 *  Return a string listing equations where a symbol is defined.
 *-------------------------------------------------------------------*/
char *symdef(void *sym)
{
   if( sym==0 )return 0;
   validate( sym, SYMBOBJ, "symdef" );
   if( ((Symbol *)sym)->leqns->n == 0 )return "none";
   return slprint( ((Symbol *)sym)->leqns );
}


/*-------------------------------------------------------------------*
 *  symuse
 *
 *  Return a string listing equations where a symbol is used
 *-------------------------------------------------------------------*/
char *symuse(void *sym)
{
   if( sym==0 )return 0;
   validate( sym, SYMBOBJ, "symuse" );
   if( ((Symbol *)sym)->reqns->n == 0 )return "none";
   return slprint( ((Symbol *)sym)->reqns );
}


/*-------------------------------------------------------------------*
 *  symall
 *
 *  Return a string listing all equations where a symbol is used
 *-------------------------------------------------------------------*/
char *symall(void *sym)
{
   List *merged;
   char *str;
   if( sym==0 )return 0;
   validate( sym, SYMBOBJ, "symall" );
   
   merged = duplist( ((Symbol *)sym)->leqns );
   catlist( merged, ((Symbol *)sym)->reqns );

   if( merged->n == 0 )
      str = "none";
   else
      str = slprint( merged );

   freelist(merged);
   return str;
}
