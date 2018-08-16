/*--------------------------------------------------------------------*
 *  troll.c
 *  Dec 04 (PJW)
 *--------------------------------------------------------------------*
.. ### troll
..
.. Support TROLL.
 *
 * + Variable attributes used: exo = exogenous.
 *--------------------------------------------------------------------*/

#include "../cart.h"
#include "../error.h"
#include "../lang.h"
#include "../options.h"
#include "../output.h"
#include "../str.h"
#include "../sym.h"
#include "../symtable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define  myDEBUG 1


// 
//  Flag for checking local Troll objects for corruption
//

#define MYOBJ 3001


//
//  Internal variables
//

static int Troll_eqn=0;
static int Troll_end=0;
static int Troll_exo=0;
static int Troll_par=0;


//----------------------------------------------------------------------//
//  troll_error()
//
//  Print an error message and crash.  Use this routine instead of
//  fatal_error in order to indicate that the problem was with the
//  Troll back end.
//----------------------------------------------------------------------//

void troll_error(char *fmt, char *str)
{
   show_error("Fatal Error Writing Troll File",fmt,str);
}


//----------------------------------------------------------------------//
//  Troll_declare
//  
//  Add a new variable or parameter to the internal list.  Reserves 
//  space in the relevant Troll vectors.
//----------------------------------------------------------------------//

void Troll_declare(void *sym)
{
}


//----------------------------------------------------------------------//
//  Troll_namegen
//
//  Build a scalar name for a variable by appending its subscripts,
//  if any.
//----------------------------------------------------------------------//

char *Troll_namegen(char *base, List *subs)
{
   Item *s;
   char *done,*more;
   done = strdup(base);
   for( s=subs->first ; s ; s=s->next )
      {
      more = concat(3,done,"_",s->str);
      free(done);
      done = more;
      }
   return done;
}

//----------------------------------------------------------------------//
//  Troll_namelist
//
//  Generate blocks of name for the declaration part of the file.  
//  Automatically generates all elements of arrays.
//----------------------------------------------------------------------//

int Troll_namelist(void *sym)
{
   char *obuf,*nbuf;
   char *base,*full;
   List *sets,*subs;
   int count;

   base = symname(sym);
   sets = symvalue(sym);
   count = 0;

   if( sets->n == 0 )
      {
      fprintf(code,"    %s\n",base);
      freelist( sets );
      free( base );
      return 1;
      }

   obuf = strdup("");
   
   cart_build(sets);
   while( subs=cart_next() )
      {
      full = Troll_namegen(base,subs);
      if( strlen(obuf)==0 )
         {
         obuf = full;
         continue;
         }
      if( strlen(obuf)+strlen(full) < 79 )
         {
         nbuf = concat(3,obuf," ",full);
         free(obuf);
         free(full);
         obuf = nbuf;
         }
      else
         {
         fprintf(code,"    %s\n",obuf);
         free(obuf);
         obuf = full;
         }
      count++;
      }

   if( strlen(obuf) )
      fprintf(code,"    %s\n",obuf);
   
   free(obuf);
   freelist(sets);
   free(base);
   return count;
}


//----------------------------------------------------------------------//
//  Troll_ADDSYM
//
//  Write out the introductory part of the file.
//----------------------------------------------------------------------//

static void Troll_ADDSYM()
{
   void *cur;
   
   fprintf(code,"ADDSYM\n");
   fprintf(code,"  ENDOGENOUS\n");
   
   for( cur=firstsymbol(var) ; cur ; cur=nextsymbol(cur) )
      if( isattrib(cur,"exo")==0 )
         Troll_end += Troll_namelist(cur);

   fprintf(code,"  ,\n");
   fprintf(code,"  EXOGENOUS\n");

   for( cur=firstsymbol(var) ; cur ; cur=nextsymbol(cur) )
      if( isattrib(cur,"exo") )
         Troll_exo += Troll_namelist(cur);

   fprintf(code,"  ,\n");
   fprintf(code,"  PARAMETER\n");

   for( cur=firstsymbol(par) ; cur ; cur=nextsymbol(cur) )
      Troll_par += Troll_namelist(cur);

   fprintf(code,"  ,\n");
   fprintf(code,"  FUNCTION\n");
   fprintf(code,"    LOG\n");
   fprintf(code,"    EXP\n");
   fprintf(code,"  ;\n\n");
   fprintf(code,"ADDEQ BOTTOM\n");
}


//----------------------------------------------------------------------//
//
//  End processing the file
//
//----------------------------------------------------------------------//

void Troll_end_file()
{
   fprintf(code,"    ;\n");
   
   fprintf(info,"\n");
   fprintf(info,"Equation Count: %d\n",Troll_eqn);
   fprintf(info,"Endogenous Variables: %d\n",Troll_end);
   fprintf(info,"Exogenous Variables: %d\n",Troll_exo);
   fprintf(info,"Parameters: %d\n",Troll_par);
}

//----------------------------------------------------------------------//
//
//  Begin a scalar equation
//
//----------------------------------------------------------------------//

void Troll_begin_eqn(void *eq)
{
   
   //
   //  if this is the first equation, write out the ADDSYM statement
   //  otherwise, end the previous equation with a comma.

   if( Troll_eqn==0 )
      Troll_ADDSYM();
   else
      fprintf(code,"   ,\n");
      
   //
   //  now start work on the equation
   //

   Troll_eqn++;
   
   //
   //  good to go...
   //
      
   fprintf(code,"\nEQN%d:\n   ",Troll_eqn);
}

//----------------------------------------------------------------------//
//
//  End an equation
//
//----------------------------------------------------------------------//

void Troll_end_eqn(void *eq)
{
   fprintf(code,"\n");
}

//----------------------------------------------------------------------//
//
//  Show a symbol
//  
//----------------------------------------------------------------------//

char *Troll_show_symbol(char *str, List *sublist, Context context)
{
   char *full,*aug,lag[10];

   full = Troll_namegen(str,sublist);

   if( context.dt != 0 )
      {
      sprintf(lag,"(%d)",context.dt);
      aug = concat(2,full,lag);
      free(full);
      return aug;
      }

   return full;
}


//----------------------------------------------------------------------//
//
//  Connect up the public routines.
//  
//----------------------------------------------------------------------//

void Troll_setup(void)
{
   lang_end_file   ( Troll_end_file    );
   lang_declare    ( Troll_declare     ); 
   lang_begin_eqn  ( Troll_begin_eqn   ); 
   lang_end_eqn    ( Troll_end_eqn     ); 
   lang_show_symbol( Troll_show_symbol );
   
   set_eqn_scalar();
   set_sum_scalar();
}

char *Troll_version = "$Revision: 57 $";

