/*--------------------------------------------------------------------*
 * oxgs.c
 * Mar 06 (PJW)
 *--------------------------------------------------------------------*
.. ### oxgs
..
.. Support a simple Ox implementation of the Gauss-Seidel algorithm.
 *  
 * + Variable attributes used: end = endogenous, exo = exogenous,
 *   cos = costate variable treated as guess, sta = state variable 
 *   treated as guess for steady states.
 * 
 * + Leads and lags are not allowed. 
 *
 * + The left hand side of the equation must be an variable, not
 *   an expression.
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
//  Flags for checking local OxGS objects for corruption
//

#define MYOBJ 3001
#define MYEND 3002


//
//  Internal variables
//

static int OxGS_block   = 1;  // count of block equations
static int OxGS_scalar  = 0;  // count of scalar equations
static int OxGS_endog   = 0;  // count of endogenous vars

static int OxGS_next_is_lhs;  // next symbol is the LHS of an equation

//
//  Define the variable object and initialize the linked list
//

struct variable
   {
   int obj;                // fidelity check
   char *str;              // variable's name
   int isendo;             // is this endogenous?
   List *sets;             // sets
   struct variable *next;  // next variable
   int lhs;                // line number of lhs or 0
   int rhs;                // 1 if some rhs, 0 otherwise
   };

typedef struct variable Variable;
static Variable *v_head=0;

struct endo 
   {
   int obj;                // fidelity check
   char *str;              // variable's name
   char *mat;              // name in matrix notation
   int off;                // location in the guess/actual list
   int lhs;                // times seen on the lhs
   struct endo *next;      // next endo
   };

typedef struct endo Endogenous;
static Endogenous *e_head=0;

static int offset=0;
static FILE *incfile;
static FILE *initfile;
static Endogenous *lastlhs=0;

//----------------------------------------------------------------------//
//  oxgs_error()
//
//  Print an error message and crash.  Use this routine instead of
//  fatal_error in order to indicate that the problem was with the
//  OxGS back end.
//----------------------------------------------------------------------//

static void oxgs_error(char *fmt, char *str)
{
   show_error("Fatal Error Writing OxGS File",fmt,str);
}


//----------------------------------------------------------------------//
//  oxgs_mkname
//
//  Combine a variable name and list of subscripts into a form
//  suitable for output.  Allocate and return a new string containing
//  the results.  Called by oxgs_getname and oxgs_declcode.
//----------------------------------------------------------------------//

static char *oxgs_mkname(char *base, List *subs)
{
   char buf[1024],*result;
   Item *s;
  
   validate( subs, LISTOBJ, "oxgs_mkname" );
   if( base == 0 )FAULT( "Null pointer passed to oxgs_mkname" );

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
//  oxgs_getname()
//
//  Figure out the vector and element number for this variable given
//  the context in which it appears.
//----------------------------------------------------------------------//

static char *oxgs_getname(char *str, List *sublist, Context context)
{
   int check;
   char buf[1024],*ptr;
   Variable *var;

   if( v_head == 0 )
      FAULT("Variable list is blank in oxgs_getname");

   for( var = v_head ; var ; var=var->next )
      {
      validate( var, MYOBJ, "oxgs_getname" );
      check = strcasecmp(str,var->str);
      if( check == 0 )break;
      if( check <  0 )
         FAULT("Name not in variable list in oxgs_getname");
      }

   if( var==0 )
      FAULT("Name not in variable list in oxgs_getname");

   //
   //  check that lead and lag structure is OK for oxgs
   //
   
   if( context.dt < 0 )
      oxgs_error("%s","lag() cannot be used with oxgs");

   if( context.dt >  0 )
      oxgs_error("%s","lead() cannot be used with oxgs");

   // 
   //  check that the LHS of lvalue equations is not exogenous
   //

   if( context.lhs && is_eqn_lvalue() && var->isendo==0 )
      oxgs_error("LHS of non-miss equation is exogenous: %s",var->str );

   //
   //  context is OK; now convert the subscripts.
   //

   strcpy(buf,str);
   if( DBG )printf("oxn non-endog identifier sublist for %s: %s\n",buf,slprint(sublist));
   if( DBG )printf("   length of sublist: %d\n",sublist->n);

   ptr = oxgs_mkname( str, sublist );
   if( ptr == 0 )FAULT("Could not allocate memory in oxgs_getname");

   return ptr;
}

   
//----------------------------------------------------------------------//
//  oxgs_declcode
//
//  Called to generate code for declarations.
//----------------------------------------------------------------------//
static void oxgs_declcode(Variable *v)      
{
   List *cur,*setlist;
   char *msg;
   char *fullname;
   void *sym;
   
   sym = lookup(v->str);
   
   msg = "  // exogenous";
   if( istype(sym,par) )msg = "  // parameter";
   if( v->isendo       )msg = "";

   setlist = symvalue(sym);
   cart_build(setlist);
   freelist(setlist);

   while( cur=cart_next() )
      {
      fullname = oxgs_mkname(v->str,cur);
      fprintf(incfile, "decl %s;%s\n",fullname,msg);
      if( cur->n == 0 )
         fprintf(initfile,"   %s = getval(\"%s\") ; \n",fullname,v->str);
      else
         fprintf(initfile,"   %s = getval(\"%s(%s)\") ; \n",fullname,v->str,slprint(cur));
      free(fullname);
      }
}


//----------------------------------------------------------------------//
//  oxgs_guess_actuals
//
//  Called to generate code for loading variables from the guess 
//  or for storing them in the actuals vector.
//----------------------------------------------------------------------//
static void oxgs_guess_actuals(int do_actuals)
{
   Endogenous *end;

   if( do_actuals == 0 )
      {
      fprintf(initfile,"\n");
      fprintf(initfile,"   decl guess;\n");
      fprintf(initfile,"   guess=zeros(%d,1);\n",OxGS_endog);
      fprintf(initfile,"   vname=new array[%d];\n",OxGS_endog);
      fprintf(initfile,"\n");
      }
      
   for( end=e_head ; end ; end=end->next )
      {
      validate( end, MYEND, "oxgs_guess_actuals" );
      if( do_actuals )
         {
         if( end->lhs != 1 )
            oxgs_error("Variable does not appear on the LHS of any equation: %s",end->str);
         }
      else
         {
         fprintf(initfile,"   vname[%d] = \"%s\" ;\n",end->off,end->mat);
         fprintf(initfile,"   guess[%d] =  %s   ;\n",end->off,end->str);
         fprintf(code,"   %s = guess[%d] ;\n",end->str,end->off);
         }
      }
      
   fprintf(code,"\n");
   fprintf(code,"   if(verbose>1)println(\" \");\n\n");
}


//----------------------------------------------------------------------//
//  OxGS_declare
//  
//  Add a new variable or parameter to the internal list.  Reserves 
//  space in the relevant OxGS vectors.
//----------------------------------------------------------------------//

void OxGS_declare(void *sym)
{
   char     *name;
   Variable *newvar;
   Variable *cur,*nxt;
   List     *attlist,*cart;

   int check,count;
   int isendo;
   
   validate( sym, SYMBOBJ, "OxGS_declare" );

   if( istype(sym,set) )return;

   if( ! isident(sym) )
      FAULT("Symbol is not an identifier in OxGS_declare");

   name = symname(sym);

   //
   //  is this an endogenous variable?
   //
   
   isendo = 0;
   if( istype(sym,var) )
      {
      isendo = 1;
      attlist = symattrib(sym);
      if( ismember("exo",attlist) )isendo = 0 ;
      freelist(attlist);
      }

   //
   //  save information about it
   //
   
   newvar = (Variable *) malloc( sizeof(Variable) );
   newvar->obj    = MYOBJ;
   newvar->str    = name;
   newvar->isendo = isendo;
   newvar->sets   = symvalue(sym);
   newvar->next   = 0;

   // 
   //  if it's an endogenous variable, add all of its elements to the 
   //  endogenous list and set the LHS count for each one to 0.
   //

   if( isendo )
      {
      Endogenous *newendo;
      Endogenous *lastendo;

      count = symsize(sym);
      if( count<1 )FAULT("Variable has no element count in OxGS_declare");
      OxGS_endog += count;

      cart_build( newvar->sets );

      while( cart=cart_next() )
         {
         newendo  = (Endogenous *) malloc( sizeof(Endogenous) );
         newendo->obj  = MYEND;
         newendo->str  = oxgs_mkname(name,cart);
         newendo->off  = offset++;
         newendo->lhs  = 0;
         if( cart->n == 0 )
            newendo->mat  = strdup(name);
         else
            newendo->mat  = concat(4,name,"(",slprint(cart),")");
         newendo->next = 0;

         if( e_head == 0 )
            e_head = newendo;
         else
            {
            for( lastendo=e_head ; lastendo->next ; lastendo=lastendo->next );
            lastendo->next = newendo;
            }
            
         }
      }

   //
   //  entry is now complete
   //

   if( DBG && isendo )
      printf("OxGS_declare: %s has %d elements\n",name,count);

   //
   //  insert it into the list in alphabetical order
   //

   //  case 1: list is empty
   
   if( v_head == 0 ) 
      {
      v_head = newvar;
      oxgs_declcode(newvar);
      return;
      }

   cur = v_head;
   check = strcasecmp(name,cur->str);

   if( check == 0 )
      FAULT("Multiple definitions of variable in OxGS_declare");

   //  case 2: insert before first existing entry

   if( check < 0 ) 
      {
      newvar->next = v_head;
      v_head = newvar;
      oxgs_declcode(newvar);
      return;
      }

   //  case 3: anywhere else except the end

   for( nxt=cur->next ; nxt ; nxt=nxt->next )
      {
      check = strcasecmp(name,nxt->str);
      if( check == 0 )
         FAULT("Multiple definitions of variable in OxGS_declare");
      if( check  < 0 )
         {
         newvar->next = nxt;
         cur->next = newvar;
         oxgs_declcode(newvar);
         return;
         }
      cur=nxt; 
      }

   //  case 4: append after last element

   cur->next = newvar;
   oxgs_declcode(newvar);
}


//----------------------------------------------------------------------//
//  OxGS_begin_file()
//
//  Begin processing the file
//----------------------------------------------------------------------//

void OxGS_begin_file(char *basename)
{
   char *fname_decl,*fname_init;
   
   fname_decl = concat(2,basename,"_decl.h");
   incfile = fopen(fname_decl,"w");
   if( incfile==0 )
      oxgs_error("Could not create include file: %s",fname_decl);
   
   fname_init = concat(2,basename,"_init.h");
   initfile = fopen(fname_init,"w");
   if( initfile==0 )
      oxgs_error("Could not create include file: %s",fname_init);
   
   fprintf(code,"#include <oxstd.h>\n");
   fprintf(code,"\n");
   fprintf(code,"// mode controls updating:\n");
   fprintf(code,"//   0 = evaluate all equations before updating\n");
   fprintf(code,"//   1 = update after each equation\n");
   fprintf(code,"//   2 = print LHS and RHS for each equation, then quit\n");
   fprintf(code,"//\n");
   fprintf(code,"decl mode=1;\n");
   fprintf(code,"//\n");
   fprintf(code,"// verbose controls output:\n");
   fprintf(code,"//   0 = print only the final guess vector\n");
   fprintf(code,"//   1 = print each guess vector\n");
   fprintf(code,"//   2 = same as 1 but also show intermediate calcs\n");
   fprintf(code,"//\n");
   fprintf(code,"decl verbose=0;\n");
   fprintf(code,"//\n");
   fprintf(code,"// gauss-seidel control parameters:\n");
   fprintf(code,"//   gs_weight  = weight of guess in revision [0,1]\n");
   fprintf(code,"//   gs_tol     = convergence tolerance\n");
   fprintf(code,"//   gs_maxit   = maximum iterations\n");
   fprintf(code,"//   gs_logfile = CSV log file for results\n");
   fprintf(code,"//\n");
   fprintf(code,"decl gs_weight = 0.5;\n");
   fprintf(code,"decl gs_tol    = 1e-7;\n");
   fprintf(code,"decl gs_maxit  = 400;\n");
   fprintf(code,"decl gs_logfile = \"%s.csv\";\n",basename);
   fprintf(code,"\n");
   fprintf(code,"#include \"%s\"\n",fname_decl);
   fprintf(code,"#include \"%s\"\n",fname_init);
   fprintf(code,"#include \"%s_func.h\"\n",basename);
   fprintf(code,"#include \"gauss-seidel.ox\"\n");
   fprintf(code,"\n");
   fprintf(code,"actual(guess)\n");
   fprintf(code,"{\n");
   fprintf(code,"   decl actuals = zeros(guess);\n");
   fprintf(code,"   decl lhs,rhs;\n\n");
   fprintf(code,"   if( mode == 2 )\n");
   fprintf(code,"      {\n");
   fprintf(code,"      verbose = 0;\n");
   fprintf(code,"      println(\"variable,lhs,rhs\");\n");
   fprintf(code,"      }\n");

   fprintf(initfile,"getval(str);\n");
   fprintf(initfile,"setval(str,val);\n");
   fprintf(initfile,"\n");
   fprintf(initfile,"decl vname;\n");
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

void OxGS_end_file()
{
   //
   //  build actuals vector
   //
   
   oxgs_guess_actuals(1);
   
   fprintf(code,"   if( mode == 2 )exit(0);\n\n");
   fprintf(code,"   return actuals;\n}\n");
   
   // 
   //  finish the other files
   //

   fprintf(initfile,"\n   return guess;\n");
   fprintf(initfile,"}\n");
   fprintf(initfile,"\n");
   
   fprintf(initfile,"output(values)\n");
   fprintf(initfile,"{\n");
   fprintf(initfile,"   decl i;\n");
   fprintf(initfile,"   for( i=0 ; i<%d ; i++ )\n",OxGS_endog);
   fprintf(initfile,"      setval(vname[i],values[i][]);\n");
   fprintf(initfile,"}\n");   
   
   fprintf(info,"\n");
   fprintf(info,"Total Equation Count: %d\n",OxGS_scalar);
   fprintf(info,"Total Endogenous Variables: %d\n",OxGS_endog);

   fclose(incfile);
   fclose(initfile);
}

//----------------------------------------------------------------------//
//
//  Begin an equation
//
//----------------------------------------------------------------------//

void OxGS_begin_block(void *eq)
{
   List *esets,*eqnsets();
   int nblk,nstart,nscalar,nend;

   if( DBG )printf("   backend begin_block\n");

   nblk    = OxGS_block++;
   nstart  = OxGS_scalar;
   nscalar = eqncount(eq);
   nend    = nstart+nscalar-1;
   
   OxGS_scalar = nend+1;

   esets = eqnsets(eq);
   
   if( ! islvalue(eq) )
      oxgs_error("%s","LHS of equation is not a variable");
      
   //
   //  if this is the first block, finish initialization of files
   //  before writing the equation
   //

   if( nblk == 1 )oxgs_guess_actuals(0);

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

   OxGS_next_is_lhs = 1;

}

//----------------------------------------------------------------------//
//
//  Begin a scalar equation
//
//----------------------------------------------------------------------//

void OxGS_begin_eqn(void *eq)
{
   OxGS_next_is_lhs = 1;
}


//----------------------------------------------------------------------//
//
//  End a scalar equation
//
//----------------------------------------------------------------------//

void OxGS_end_eqn(void *eq)
{
   fprintf(code," ;\n");
   fprintf(code,"actuals[%d] = rhs ;\n",lastlhs->off);
   fprintf(code,"if( mode == 1 )%s = rhs;\n",lastlhs->str);
   fprintf(code,"if( mode == 2 )println(\"%s, \",lhs,\", \",rhs);\n",lastlhs->str);
   fprintf(code,"if( verbose>1 )println(\"%s = \",lhs,\" -> \",rhs);\n\n",lastlhs->str);
}


//----------------------------------------------------------------------//
//
//  Show a symbol
//  
//----------------------------------------------------------------------//

char *OxGS_show_symbol(char *str, List *sublist, Context context)
{
   char *ptr;
   int efound;
   
   ptr = oxgs_getname(str,sublist,context) ;
   if( DBG )printf("OxGS_show_symbol: %s\n",ptr);

   if( OxGS_next_is_lhs )
      {
      Endogenous *e;
      efound = 0;
      
      for( e=e_head ; e ; e=e->next )
         if( strcasecmp(ptr,e->str)==0)
            {
            efound = 1;
            e->lhs++;
            if( e->lhs > 1 )
               oxgs_error("Variable appears on LHS of multiple equations: %s",ptr);
            break;
            }
      if( efound==0 )
         oxgs_error("Variable on LHS is not endogenous: %s",ptr);

      OxGS_next_is_lhs = 0;

      lastlhs = e;

      fprintf(code,"lhs = %s ;\n",ptr);
      return "rhs";
      }
     
   return ptr;
}


//----------------------------------------------------------------------//
//
//  Connect up the public routines.
//  
//----------------------------------------------------------------------//

void OxGS_setup(void)
{
   lang_begin_file ( OxGS_begin_file  ); 
   lang_end_file   ( OxGS_end_file    );
   lang_declare    ( OxGS_declare     ); 
   lang_begin_block( OxGS_begin_block ); 
   lang_begin_eqn  ( OxGS_begin_eqn   ); 
   lang_end_eqn    ( OxGS_end_eqn     ); 
   lang_show_symbol( OxGS_show_symbol );

   set_eqn_scalar();
   set_eqn_lvalue();
   set_sum_scalar();
}

char *OxGS_version = "$Revision: 57 $";
