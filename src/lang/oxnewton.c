/*--------------------------------------------------------------------*
 *  oxnewton.c
 *  Dec 04 (PJW)
 *--------------------------------------------------------------------*
.. ### oxnewton
..
.. Support an Ox implementation of Newton's Method for simple 
.. models.
 *
 * + Variable attributes used: end = endogenous, exo = exogenous,
 *   guess = endogenous and part of the X vector in F(X)=0, cos = 
 *   costate variable but treated as a synonym for guess, sta = 
 *   state variable but treated as a synonym for guess.
 *
 * + Equation attributes used: miss = include in the F vector in 
 *   F(X)=0
 *  
 * + Leads and lags are not allowed.
 *
 * + The left hand side of each equation must be a variable name.
 *--------------------------------------------------------------------*/

#include "../cart.h"
#include "../eqns.h"
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
//  Flag for checking local OxNewton objects for corruption
//

#define MYOBJ 3001


//
//  Internal variables
//

static int OxNewton_block =1;
static int OxNewton_scalar=0;

static int OxNewton_guess=0;  // guess vector
static int OxNewton_endog=0;

static int OxNewton_miss=0;   // miss vector
static int OxNewton_norm=0;


//
//  Define the variable object and initialize the linked list
//

struct variable
   {
   int obj;                // fidelity check
   char *str;              // variable's name
   int isendo;             // is this endogenous?
   int isguess;            // is this one of the guessed variables?
   int off;                // offset from start of vector
   List *sets;             // sets
   struct variable *next;  // next variable
   };

typedef struct variable Variable;
static Variable *v_head=0;

static int offset=0;
static FILE *incfile;
static FILE *initfile;


//----------------------------------------------------------------------//
//  oxn_error()
//
//  Print an error message and crash.  Use this routine instead of
//  fatal_error in order to indicate that the problem was with the
//  OxNewton back end.
//----------------------------------------------------------------------//

void oxn_error(char *fmt, char *str)
{
   show_error("Fatal Error Writing OxNewton File",fmt,str);
}


//----------------------------------------------------------------------//
//  oxn_mkname
//
//  Combine a variable name and list of subscripts into a form
//  suitable for output.  Allocate and return a new string containing
//  the results.  Called by oxn_getname and oxn_declcode.
//----------------------------------------------------------------------//

char *oxn_mkname(char *base, List *subs)
{
   char buf[1024],*result;
   Item *s;
  
   validate( subs, LISTOBJ, "oxn_mkname" );
   if( base == 0 )FAULT( "Null pointer passed to oxn_mkname" );

   strcpy(buf,base);
   for( s=subs->first ; s ; s=s->next )
      {
      strcat(buf,"_");
      strcat(buf,s->str);
      }

   result = strlower(buf);
   return( result );
}


//----------------------------------------------------------------------//
//  oxn_declcode
//
//  Called to generate code for declarations.
//----------------------------------------------------------------------//
void oxn_declcode(Variable *v)      
{
   List *cur,*numsubs,*setlist;
   char *msg;
   char *fullname;

   msg = (v->isendo) ? "" : "  // exogenous or parameter";

   setlist = symvalue(lookup(v->str));
   cart_build(setlist);
   freelist(setlist);

   while( cur=cart_next() )
      {
      fullname = oxn_mkname(v->str,cur);
      fprintf(incfile,"   decl %s;%s\n",fullname,msg);
      fprintf(initfile,"   %s = getval(\"%s(%s)\") ; \n",fullname,v->str,slprint(cur));

      // if this is a guess variable, add code to pull it out of the
      // guess vector

      if( v->isguess )
         {
         numsubs = sub_offset( v->str, cur, v->off );
         fprintf(code,"   %s = guess[%s] ;\n",fullname,(numsubs->first)->str);
         freelist(numsubs);
         }

      free(fullname);
      }
}


//----------------------------------------------------------------------//
//  OxNewton_declare
//  
//  Add a new variable or parameter to the internal list.  Reserves 
//  space in the relevant OxNewton vectors.
//----------------------------------------------------------------------//

void OxNewton_declare(void *sym)
{
   char     *name;
   Variable *newvar;
   Variable *cur,*nxt;
   List     *attlist;
   
   int check,count;
   int isendo,isguess;
   
   validate( sym, SYMBOBJ, "OxNewton_declare" );

   if( istype(sym,set) )return;

   if( ! isident(sym) )
      FAULT("Symbol is not an identifier in OxNewton_declare");

   name    = symname(sym);
   attlist = symattrib(sym);

   //
   //  is this an endogenous or guess variable?
   //
   
   isendo  = 0;
   isguess = 0;
   if( istype(sym,var) )
      {
      if( !ismember("exo"  ,attlist) )isendo  = 1;
      if(  ismember("guess",attlist) )isguess = 1;
      if(  ismember("cos"  ,attlist) )isguess = 1;
      if(  ismember("sta"  ,attlist) )isguess = 1;
      }

   freelist(attlist);

   //
   //  got an endogenous variable; find out more about it and then
   //  add it to the list so that we can map it to the guess vector
   //

   count = symsize(sym);

   if( count<1 )FAULT("Symbol has no element count in OxNewton_declare");

   //
   //  save information about it
   //
   
   newvar = (Variable *) malloc( sizeof(Variable) );
   newvar->obj    = MYOBJ;
   newvar->str    = name;
   newvar->isendo = isendo;
   newvar->isguess= isguess;
   newvar->off    = 0;
   newvar->sets   = symvalue(sym);
   newvar->next   = 0;

   // 
   //  if it's a guess, make a note of its offset and then update 
   //  the offset to reserve space for it
   //

   if( isguess )
      {
      newvar->off = offset;
      offset += count;
      OxNewton_guess += count;
      }

   if( isendo && !isguess )
      OxNewton_endog += count;

   //
   //  entry is now complete
   //

   if( DBG && isguess )
      {
      printf("OxNewton_declare: %s has %d elements beginning at ",name,count);
      printf("guess[%d]\n",newvar->off);
      }

   //
   //  insert it into the list in alphabetical order
   //

   //  case 1: list is empty
   
   if( v_head == 0 ) 
      {
      v_head = newvar;
      oxn_declcode(newvar);
      return;
      }

   cur = v_head;
   check = strcasecmp(name,cur->str);

   if( check == 0 )
      FAULT("Multiple definitions of variable in OxNewton_declare");

   //  case 2: insert before first existing entry

   if( check < 0 ) 
      {
      newvar->next = v_head;
      v_head = newvar;
      oxn_declcode(newvar);
      return;
      }

   //  case 3: anywhere else except the end

   for( nxt=cur->next ; nxt ; nxt=nxt->next )
      {
      check = strcasecmp(name,nxt->str);
      if( check == 0 )
         FAULT("Multiple definitions of variable in OxNewton_declare");
      if( check  < 0 )
         {
         newvar->next = nxt;
         cur->next = newvar;
         oxn_declcode(newvar);
         return;
         }
      cur=nxt; 
      }

   //  case 4: append after last element

   cur->next = newvar;
   oxn_declcode(newvar);
}


//----------------------------------------------------------------------//
//  oxn_getname()
//
//  Figure out the vector and element number for this variable given
//  the context in which it appears.
//----------------------------------------------------------------------//

char *oxn_getname(char *str, List *sublist, Context context)
{
   int check;
   char buf[1024],*ptr;
   Variable *var;

   if( v_head == 0 )
      FAULT("Variable list is blank in oxn_getname");

   for( var = v_head ; var ; var=var->next )
      {
      validate( var, MYOBJ, "oxn_getname" );
      check = strcasecmp(str,var->str);
      if( check == 0 )break;
      if( check <  0 )
         FAULT("Name not in variable list in oxn_getname");
      }

   if( var==0 )
      FAULT("Name not in variable list in oxn_getname");

   //
   //  check that lead and lag structure is OK for oxnewton
   //
   
   if( context.dt < 0 )
      oxn_error("%s","lag() cannot be used with oxnewton");

   if( context.dt >  0 )
      oxn_error("%s","lead() cannot be used with oxnewton");

   // 
   //  check that the LHS of lvalue equations is not exogenous
   //

   if( context.lhs && is_eqn_lvalue() && var->isendo==0 )
      oxn_error("LHS of non-miss equation is exogenous: %s",var->str );

   //
   //  context is OK; now convert the subscripts.
   //

   strcpy(buf,str);
   if( DBG )printf("oxn non-endog identifier sublist for %s: %s\n",buf,slprint(sublist));
   if( DBG )printf("   length of sublist: %d\n",sublist->n);

   ptr = oxn_mkname( str, sublist );
   if( ptr == 0 )FAULT("Could not allocate memory in oxn_getname");

   return ptr;
}

   
//----------------------------------------------------------------------//
//  OxNewton_begin_file()
//
//  Begin processing the file
//----------------------------------------------------------------------//

void OxNewton_begin_file(char *basename)
{
   char *fname_decl,*fname_init;
   
   fname_decl = concat(2,basename,"_decl.h");
   incfile = fopen(fname_decl,"w");
   if( incfile==0 )
      oxn_error("Could not create include file: %s",fname_decl);
   
   fname_init = concat(2,basename,"_init.h");
   initfile = fopen(fname_init,"w");
   if( initfile==0 )
      oxn_error("Could not create include file: %s",fname_init);
   
   fprintf(code,"#include <oxstd.h>\n");
   fprintf(code,"#include \"%s\"\n",fname_decl);
   fprintf(code,"#include \"%s\"\n",fname_init);
   fprintf(code,"#include \"%s_func.h\"\n",basename);
   fprintf(code,"\n");
   fprintf(code,"miss(guess)\n");
   fprintf(code,"{\n   decl missdist;\n\n");

   fprintf(initfile,"getval(str);\n");
   fprintf(initfile,"\n");
   fprintf(initfile,"init()\n");
   fprintf(initfile,"{\n");

   free(fname_decl);
   free(fname_init);
}


//----------------------------------------------------------------------//
//
//  End processing the file
//
//----------------------------------------------------------------------//

void OxNewton_end_file()
{
   fprintf(code,"   return missdist;\n}\n");
   
   fprintf(initfile,"}\n");

   fprintf(info,"\n");
   fprintf(info,"Total Equation Count: %d\n",OxNewton_miss+OxNewton_norm);
   fprintf(info,"Total Endogenous Variables: %d\n",OxNewton_guess+OxNewton_endog);
   fprintf(info,"\n");
   fprintf(info,"Number of Miss Equations: %d\n",OxNewton_miss);
   fprintf(info,"Number of Guess Variables: %d\n",OxNewton_guess);

   fclose(incfile);
   fclose(initfile);
}

//----------------------------------------------------------------------//
//
//  Begin an equation
//
//----------------------------------------------------------------------//

void OxNewton_begin_block(void *eq)
{
   List *esets,*eqnsets();
   int nblk,nstart,nscalar,nend;

   if( DBG )printf("   backend begin_block\n");

   nblk    = OxNewton_block++;
   nstart  = OxNewton_scalar;
   nscalar = eqncount(eq);
   nend    = nstart+nscalar-1;
   
   OxNewton_scalar = nend+1;

   esets = eqnsets(eq);
   
   //
   //  Equations with the 'miss' attribute are converted to 
   //  normalized form on the RHS and the LHS is replaced with 
   //  a miss distance variable.
   //

   if( iseqnattr(eq,"miss") )
      set_eqn_normalized();
   else
      {
      set_eqn_lvalue();
      if( ! islvalue(eq) )
         oxn_error("%s","LHS of non-miss equation is not a variable");
      }
      
   //
   //  good to go...
   //
      
   fprintf(code,"// Equation block %d\n",nblk);
  
   if( esets->n )
      fprintf(code,"//    Defined over sets (%s)\n",slprint(esets) );

   if( nscalar )
      fprintf(code,"//    Scalar equations %d-%d (%d total)\n\n",nstart,nend,nscalar);
   else
      fprintf(code,"//    Contains undeclared symbols\n");
}

//----------------------------------------------------------------------//
//
//  Begin a scalar equation
//
//----------------------------------------------------------------------//

void OxNewton_begin_eqn(void *eq)
{
   if( is_eqn_normalized(eq) )
      {
      fprintf(code,"   missdist[%d] = ",OxNewton_miss++);
      return;
      }
   OxNewton_norm++;
}

//----------------------------------------------------------------------//
//
//  Show a symbol
//  
//----------------------------------------------------------------------//

char *OxNewton_show_symbol(char *str, List *sublist, Context context)
{
   char *ptr;

   ptr = oxn_getname(str,sublist,context) ;

   if( DBG )printf("OxNewton_show_symbol: %s\n",ptr);
   
   return ptr;
}


//----------------------------------------------------------------------//
//
//  Connect up the public routines.
//  
//----------------------------------------------------------------------//

void Oxnewton_setup(void)
{
   lang_begin_file ( OxNewton_begin_file  ); 
   lang_end_file   ( OxNewton_end_file    );
   lang_declare    ( OxNewton_declare     ); 
   lang_begin_block( OxNewton_begin_block ); 
   lang_begin_eqn  ( OxNewton_begin_eqn   ); 
   lang_show_symbol( OxNewton_show_symbol );

   set_eqn_scalar();
   set_sum_scalar();
   set_eqn_normalized();
}

char *Oxnewton_version = "$Revision: 57 $";

