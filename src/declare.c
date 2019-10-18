/*--------------------------------------------------------------------*
 *  declare.c
 *  Dec 04 (PJW)
 *
 *  Connect parser to symbol table via declare() and decset().
 *--------------------------------------------------------------------*/
  
#include "declare.h"

#include "error.h"
#include "lists.h"
#include "nodes.h"
#include "options.h"
#include "sets.h"
#include "str.h"
#include "sym.h"
#include "symtable.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#ifndef __WATCOMC__
#include <strings.h>
#endif /* __WATCOM__ */

#define myDEBUG 0

void symdeclare(Symboltype,char*,List*,char*,List*);


//----------------------------------------------------------------------
//  declare()
//  
//  Handle most declaration statments.  Frees all nodes passed 
//  to it after their information has been extracted.
//----------------------------------------------------------------------

void declare(type,name,defn,label,attr)
Node *type,*name,*defn,*label,*attr;
{
   Symboltype symtype;
   Node *cur;
   List *values,*attribs;
   char *labelstr;
   Item *e;
   
   // 
   //  the name and type arguments are required for all declarations
   //

   validate( name, NODEOBJ, "declare" );
   validate( type, NODEOBJ, "declare" );
   
   //  
   //  check declaration type
   //  
   
   symtype = und;
   if( isequal(type->str,"set"      ) )symtype = set;
   if( isequal(type->str,"parameter") )symtype = par;
   if( isequal(type->str,"variable" ) )symtype = var;

   if( symtype==und )
      FAULT("Unexpected declaration type in declare.c");
  
   //
   //  where are we?
   //
   
   if( DBG )
      {
      printf("declaration of %s\n",type->str);
      printf("   name is %s\n",name->str);
      printf("   defn  is null: %d\n",defn ==0);
      printf("   label is null: %d\n",label==0);
      printf("   attr  is null: %d\n",attr ==0);
      }
   
   //
   //  check the name against the optional reserved word list
   //

   if( is_reserved_word(name->str) )
      error_back("Cannot use reserved word '%s' as a name",name->str);

   //  
   //  the defn argument is required for sets 
   //  
   
   if( symtype==set )
      validate( defn, NODEOBJ, "declare @ set" );
   
   values = newsequence();
   for( cur=defn ; cur ; cur=cur->r )
      if( ismember( cur->str, values ) )
         error_front("Declaration of '%s': duplicate element(s)",name->str);
      else
         addlist( values, cur->str );
      
   //
   //  check set elements for backend problems.  note that we won't 
   //  need to do this in decset() because all element problems will 
   //  already have been detected and reported here.
   //
   
   if( symtype==set )
      {
      for( e=values->first ; e ; e=e->next )
         if( is_reserved_word(e->str) )
            error_back("Cannot use reserved word '%s' as an element name",e->str);
   
      if( is_alpha_elements() )
         for( e=values->first ; e ; e=e->next )
            if( isalpha(*e->str)==0 )
               {
               error_back("Elements of '%s' do not begin with a letter",name->str);
               break;
               }
      }
      
   //
   //  save the attributes
   //
   
   attribs = newlist();
   for( cur=attr ; cur ; cur=cur->r )
      addlist( attribs, cur->str );

   labelstr = label ? label->str : 0 ;

   //
   //  create the symbol
   //

   symdeclare(symtype,name->str,values,labelstr,attribs);

   //
   //  free the working lists
   //

   freelist( values );
   freelist( attribs );

   //
   //  free all the nodes; don't worry that some of them 
   //  may be null because freenode handles that.
   //

   freenode( type  );
   freenode( name  );
   freenode( defn  );
   freenode( label );
   freenode( attr  );
}


//----------------------------------------------------------------------
//  decset()
//
//  Handle declaration statements involving set arithmetic.
//----------------------------------------------------------------------

void decset(Node *name, Node *source, Nodetype op, Node *mods, Node *desc)
{
   List *newset,*srcset,*altset;
   Node *m;
   int  alias=0;
   char *labelstr,*decfmt,*altname;
   Item *e;
   void *sym;

   newset = 0;
   srcset = 0;
   altset = 0;

   //
   //  defining an alias?
   //
   
   alias = 1;
   if( mods || op )alias = 0;

   // 
   //  check basic arguments
   //
   
   validate( name,   NODEOBJ, "decset" );
   validate( source, NODEOBJ, "decset" );
   if( desc )validate( desc, NODEOBJ, "decset" );
   
   //
   //  check the name against the optional reserved word list
   //

   if( is_reserved_word(name->str) )
      error_back("Cannot use reserved word '%s' as a name",name->str);

   //
   //  get the base set
   //
   
   sym = lookup(source->str);
   if( sym==0 )
      {
      error_front("Definition of set '%s' uses undefined set '%s'\n",name->str,source->str);
      exit(0);
      }
   srcset = symvalue(sym);

   //
   //  set up operation
   //

   switch( op )
      {
      case nul:
         newset = duplist( srcset );
         setalias(name->str,source->str);
         break;

      case add:
      case sub:
         newset = duplist( srcset );
         validate( mods, NODEOBJ, "decset" );
         break;

      case equ:
         newset = newsequence();
         validate( mods, NODEOBJ, "decset" );
         break;

      case sad:
      case ssu:
         newset = duplist( srcset );
         validate( mods, NODEOBJ, "decset" );
         altname = lookup(mods->str);
         if( altname==0 )
            {
            error_front("Definition of set '%s' uses undefined set '%s'\n",name->str,mods->str);
            exit(0);
            }
         altset = symvalue(altname);
         break;

      default:
         FAULT("Invalid operation in decset" );
      }
  
   // 
   //  make any modifications
   //
   
   decfmt = "Declaration of '%s': '%s' %s a member of '%s'";
   for( m=mods ; m ; m=m->r )
      switch( op )
         {
         case add: 
            if( ismember(m->str,newset)==0 )
               addlist( newset, m->str );
            else 
               error_front(decfmt,name->str,m->str,"is already",source->str);
            break;

         case sub:
            if( ismember(m->str,newset) )
               freeitem( newset, m->str );
            else
               error_front(decfmt,name->str,m->str,"is not",source->str);
            break;
         
         case equ:
            if( ismember(m->str,srcset) )
               addlist( newset, m->str );
            else
               error_front(decfmt,name->str,m->str,"is not",source->str);
            break;

         case sad:
            for( e=altset->first ; e ; e=e->next )
               if( ismember(e->str,newset)==0 )
                  addlist( newset, e->str );
               else
                  error_front(decfmt,name->str,e->str,"is already",source->str);
            break;

         case ssu:
            for( e=altset->first ; e ; e=e->next )
               if( ismember(e->str,newset) )
                  freeitem( newset, e->str );
               else
                  error_front(decfmt,name->str,e->str,"is not",source->str);
            break;

         default:
            FAULT("unexpected state in decset");
         }

   //
   //  is the resulting set nonempty?
   //

   if( newset->n == 0 )
      fatal_error("Source set '%s' is empty",name->str);

   //
   //  check whether first and last pick out the correct elements of time
   //

   if( isequal(name->str,"first") )
      if( ! isequal(mods->str,srcset->first->str) )
         error_front("Declaration of 'first' does not specify first element of 'time'\n");

   if( isequal(name->str,"last") )
      {
      for( e=srcset->first ; e->next ; e=e->next ); 
      if( ! isequal(mods->str,e->str) )
         error_front("Declaration of 'last' does not specify last element of 'time'\n");
      }
      
   //
   //  create the new symbol table entry
   //

   labelstr = desc ? desc->str : 0 ;

   symdeclare(set,name->str,newset,labelstr,0);

   //
   //  create the appropriate subset relationship
   //

   if( op == add )setsubset(source->str,name->str);
   if( op == sub )setsubset(name->str,source->str);
   if( op == equ )setsubset(name->str,source->str);
   if( op == sad )
      {
      setsubset(source->str,name->str);
      setsubset(mods->str,name->str);
      }
   if( op == ssu )setsubset(name->str,source->str);

   //
   //  free everything that's no longer needed
   //
   
   freelist(srcset);
   freelist(newset);
   freelist(altset);

   freenode( name   );
   freenode( source );
   freenode( mods   );
   freenode( desc   );
}


//----------------------------------------------------------------------
//  decunion()
//
//  Handle declarations of set unions
//----------------------------------------------------------------------

void decunion(Node *name, Node *srcs, Node *desc)
{
   List *newset,*srcset;
   Node *s;
   char *labelstr;
   Item *e;
   void *sym;

   newset = 0;
   srcset = 0;

   // 
   //  check basic arguments
   //
   
   validate( name, NODEOBJ, "decset" );
   validate( srcs, NODEOBJ, "decset" );
   if( desc )validate( desc, NODEOBJ, "decset" );
   
   //
   //  check the name against the optional reserved word list
   //

   if( is_reserved_word(name->str) )
      error_back("Cannot use reserved word '%s' as a name",name->str);

   //
   //  build the set
   //

   newset = newlist();

   for( s=srcs ; s ; s=s->r )
      {
      sym = lookup(s->str);
      if( sym==0 )
         {
         error_front("Definition of set '%s' uses undefined set '%s'\n",name->str,s->str);
         exit(0);
         }

      srcset = symvalue(sym);

      for( e=srcset->first ; e ; e=e->next )
         if( ismember(e->str,newset)==0 )
            addlist( newset, e->str );

      setsubset(s->str,name->str);

      freelist(srcset);
      }
      
   //
   //  create the new symbol table entry
   //

   labelstr = desc ? desc->str : 0 ;

   symdeclare(set,name->str,newset,labelstr,0);

   //
   //  free things no longer needed
   //

   freenode( name );
   freenode( srcs );
   freenode( desc );
}
