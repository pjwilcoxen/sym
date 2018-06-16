/*--------------------------------------------------------------------*
 *  OxGST.c
 *  Jun 08 (PJW)
 *   
 *  Backend routine generating an Ox procedure for use with 
 *  the Gauss-Seidel algorithm.  This version removes the
 *  restriction in OxGS that limits it to single periods.
 *  
 *  Variable Attributes Used:
 *  
 *     end   = endogenous
 *     exo   = exogenous
 *     cos   = treated as 'guess'
 *     sta   = treated as 'guess' (for use with ss programs)
 *     
 *  Equation Attributes Used:
 *
 *     none
 *     
 *  Variable Context Restrictions Imposed:
 *
 *     The LHS must be an lvalue.  All variables will be treated
 *     as subscripted by time even if they aren't explicitly
 *     declared that way.
 *
 *--------------------------------------------------------------------*/

#include "../error.h"
#include "../eqns.h"
#include "../options.h"
#include "../cart.h"
#include "../lang.h"
#include "../output.h"
#include "../sets.h"
#include "../str.h"
#include "../sym.h"
#include "../symtable.h"
#include "../xmalloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define  myDEBUG 1

// 
//  Flags for checking local OxGST objects for corruption
//

#define MYOBJ 3111
#define MYEND 3112


//
//  Internal variables
//

static int OxGST_block   = 1;  // count of block equations
static int OxGST_scalar  = 0;  // count of scalar equations
static int OxGST_endog   = 0;  // count of endogenous vars

static int OxGST_next_is_lhs;  // next symbol is the LHS of an equation

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
   int tsub;               // index of the time subscript in the set list
   };

typedef struct variable Variable;
static Variable *v_head=0;

//
//  Define the endogenous object.  There will be one of these for 
//  each element of each variable, and they are primarily used in 
//  building the guess and actual vectors.
//

struct endo 
   {
   int obj;                // fidelity check
   char *str;              // variable's name
   char *mat;              // name in matrix notation
   char *basename;         // similar to mat but without the t subscript
   int t_index;            // 0-based time index
   int off;                // location in the guess/actual list
   int lhs;                // times seen on the lhs
   struct endo *next;      // next endo
   };

typedef struct endo Endogenous;
static Endogenous *e_head=0;

static int offset=0;
static FILE *incfile;
static FILE *initfile;
static FILE *csvfile;
static Endogenous *lastlhs=0;

static List *timeset=0;
static char **timesubs;
static int timecount=0;

//----------------------------------------------------------------------//
//  OxGST_error()
//
//  Print an error message and crash.  Use this routine instead of
//  fatal_error in order to indicate that the problem was with the
//  OxGST back end.
//----------------------------------------------------------------------//

static void OxGST_error(char *fmt, char *str)
{
   show_error("Fatal Error Writing OxGST File",fmt,str);
}


//----------------------------------------------------------------------//
//  OxGST_mkname
//
//  Combine a variable name and list of subscripts into a form
//  suitable for output.  Allocate and return a new string containing
//  the results.  Called by OxGST_getname and OxGST_declcode.
//----------------------------------------------------------------------//

static char *OxGST_mkname(char *base, List *subs)
{
   char buf[1024],*result;
   Item *s;
  
   validate( subs, LISTOBJ, "OxGST_mkname" );
   if( base == 0 )FAULT( "Null pointer passed to OxGST_mkname" );

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
//  OxGST_modtime()
//
//  Revise the time subscript of a variable for leads or lags.
//  Returns a new list of subscipts that should be freed immediately
//  after use.
//----------------------------------------------------------------------//

static List *OxGST_modtime(char *str, List *sublist, Context context)
{
   int t;
   Item *ele;
   List *newsubs;
   int found;

   //
   //  build a handy list of time subscripts the first time we arrive
   //  in this routine
   //

   if( timeset == 0 )
      {
      timeset   = setelements("time");
      timecount = timeset->n;
      timesubs  = (char **) xmalloc( timecount*sizeof(char *) );
      for( t=0, ele=timeset->first ; t < timecount ; ele=ele->next )
         timesubs[t++] = ele->str;
      }

   //
   //  now find the time subscript and move backward or forward 
   //  from it through the set
   //

   t = setindex("time",context.tsub) + context.dt ;

   if( t < 0 || t >= timecount )
      OxGST_error("Internal error: invalid lead or lag for %s", str); 

   //
   //  ok, build a new list that looks like the old one but with 
   //  the time subscript replaced
   //

   found = 0;
   newsubs = duplist(sublist);
   for( ele=newsubs->first ; ele ; ele=ele->next )
      if( isequal(ele->str,context.tsub) )
         {
         free( ele->str );
         ele->str = strdup( timesubs[t] );
         found = 1;
         break;
         }

   if( found == 0 )
      OxGST_error( "Internal error: failed to update lead or lag subscript on %s", str );

   return newsubs;
}


//----------------------------------------------------------------------//
//  OxGST_getname()
//
//  Figure out the vector and element number for this variable given
//  the context in which it appears.
//----------------------------------------------------------------------//

static char *OxGST_getname(char *str, List *sublist, Context context)
{
   int check;
   char *ptr;
   Variable *var;
   List *mod_sublist;

   if( v_head == 0 )
      FAULT("Variable list is blank in OxGST_getname");

   for( var = v_head ; var ; var=var->next )
      {
      validate( var, MYOBJ, "OxGST_getname" );
      check = strcasecmp(str,var->str);
      if( check == 0 )break;
      if( check <  0 )
         FAULT("Name not in variable list in OxGST_getname");
      }

   if( var==0 )
      FAULT("Name not in variable list in OxGST_getname");

   // 
   //  check that the LHS of lvalue equations is not exogenous
   //

   if( context.lhs && is_eqn_lvalue() && var->isendo==0 )
      OxGST_error("LHS of non-miss equation is exogenous: %s",var->str );

   //
   //  building the name is easy if there are no leads or lags ...
   //
   
   if( context.dt == 0 )
      {
      ptr = OxGST_mkname( str, sublist );
      if( ptr == 0 )FAULT("Could not allocate memory in OxGST_getname");
      return ptr;
      }

   //
   //  a bit more complicated with a lead or lag ..
   //

   if( context.tsub == 0 )
      OxGST_error("Internal error: time subscript not identified for %s", str); 
      
   if( DBG )printf( "lead or lag with sublist = %s\n", slprint(sublist) );
   if( DBG )printf( "time subscript is %s and dt is %d\n",context.tsub,context.dt );

   mod_sublist = OxGST_modtime(str,sublist,context);
   ptr = OxGST_mkname( str, mod_sublist );
   if( ptr == 0 )FAULT("Could not allocate memory in OxGST_getname");

   freelist( mod_sublist );

   return ptr;
}


//----------------------------------------------------------------------//
//  OxGST_csvname
//
//  Build a name suitable for use in input and output files handled
//  by simple-csv.ox.  Returns a new string.
//----------------------------------------------------------------------//
static char *OxGST_csvname( Variable *var, List *cursubs )
{
   char *csvname,*catname,*join;
   int i;
   Item *ele;

   validate( var,     MYOBJ,   "OxGST_csvname" );
   validate( cursubs, LISTOBJ, "OxGST_csvname" );
   
   csvname = strdup(var->str);
   if( cursubs->n == 0 )return csvname;
   if( cursubs->n == 1 && var->tsub == 0 )return csvname;

   join = "(";
   for( i=0, ele=cursubs->first ; ele ; i++, ele=ele->next )
      if( i != var->tsub )
         {
         catname = concat(3,csvname,join,ele->str);
         free(csvname);
         csvname = catname;
         join = ",";
         }

   catname = concat(2,csvname,")");
   free(csvname);
   csvname = catname;

   return csvname;
   }

   
//----------------------------------------------------------------------//
//  OxGST_time
//
//  Find the time subscript in a list and then translate it into
//  an index in the "time" set.
//----------------------------------------------------------------------//
static int OxGST_time( int tsub, List *cursubs )
{
   int i;
   Item *ele;

   validate( cursubs, LISTOBJ, "OxGST_csvname" );
   
   if( cursubs->n == 0 )
      return 0;

   for( i=0, ele=cursubs->first ; ele ; i++, ele=ele->next )
      if( i == tsub )
         return setindex("time",ele->str);

   FAULT("Internal error in OxGST_time");

   return -1;
   }

   
//----------------------------------------------------------------------//
//  OxGST_declcode
//
//  Called to generate code for declarations.
//----------------------------------------------------------------------//
static void OxGST_declcode(Variable *v)      
{
   List *cur,*setlist;
   char *msg;
   char *fullname,*algname,*lastname;
   void *sym;
   int  i,tsub,t_index;

   sym = lookup(v->str);
   
   msg = "  // exogenous";
   if( istype(sym,par) )msg = "  // parameter";
   if( v->isendo       )msg = "";

   setlist = symvalue(sym);

   //
   //  see if this variable has a time subscript
   //

   tsub = v->tsub;

   //
   //  build the cartesian product of the set indices
   //

   cart_build(setlist);
   freelist(setlist);

   //
   //  go through the whole list of subscripts
   //

   lastname = strdup("");
   while( cur=cart_next() )
      {

      //
      //  create the OxGST variable name
      //

      fullname = OxGST_mkname(v->str,cur);
      fprintf(incfile, "decl %s;%s\n",fullname,msg);

      //
      //  
      //  that can be used with simple-csv.ox
      //

      algname = OxGST_csvname(v,cur);
      t_index = OxGST_time(v->tsub,cur);

      //
      //  add the scalar name to the init file
      //

      fprintf(initfile,"   %s = getval(\"%s\",%d) ; \n",fullname,algname,t_index+1);
      
      // 
      //  detect whether we've gone to a new time series variable by 
      //  checking the time index.  since the time subscript is required
      //  to be last in the list, it will vary the fastest.
      //

      if( !isequal(algname,lastname) )
         {
         fprintf(csvfile ,"\"%s\"",algname);
         for( i=0 ; i < setsize("time") ; i++ )
            fprintf(csvfile,",1");
         fprintf(csvfile,"\n");
         free(lastname);
         lastname = strdup(algname);
         }

      // 
      //  go on to the next element
      //

      free(fullname);
      free(algname);
      }
}


//----------------------------------------------------------------------//
//  OxGST_guess_actuals
//
//  Called to generate code for loading variables from the guess 
//  or for storing them in the actuals vector.
//----------------------------------------------------------------------//
static void OxGST_guess_actuals(int do_actuals)
{
   Endogenous *end;

   if( do_actuals == 0 )
      {
      fprintf(initfile,"\n");
      fprintf(initfile,"   decl guess;\n");
      fprintf(initfile,"   guess=zeros(%d,1);\n",OxGST_endog);
      fprintf(initfile,"   vname=new array[%d];\n",OxGST_endog);
      fprintf(initfile,"   vtime=new array[%d];\n",OxGST_endog);
      fprintf(initfile,"\n");
      }
      
   for( end=e_head ; end ; end=end->next )
      {
      validate( end, MYEND, "OxGST_guess_actuals" );
      if( do_actuals )
         {
         if( end->lhs != 1 )
            OxGST_error("Variable does not appear on the LHS of any equation: %s",end->str);
         }
      else
         {
         fprintf(initfile,"   guess[%d] =  %s ;\n",end->off,end->str);
         fprintf(initfile,"   vname[%d] = \"%s\" ;\n",end->off,end->basename);
         fprintf(initfile,"   vtime[%d] =  %d ;\n",end->off,end->t_index+1);
         fprintf(code,"   %s = guess[%d] ;\n",end->str,end->off);
         }
      }
      
   fprintf(code,"\n");
   fprintf(code,"   if(verbose>1)println(\" \");\n\n");
}


//----------------------------------------------------------------------//
//  OxGST_declare
//  
//  Add a new variable or parameter to the internal list.  Reserves 
//  space in the relevant OxGST vectors.
//----------------------------------------------------------------------//

void OxGST_declare(void *sym)
{
   char     *name;
   Variable *newvar;
   Variable *cur,*nxt;
   List     *attlist,*cart;
   Item     *ele;

   int check,count;
   int isendo;
   
   validate( sym, SYMBOBJ, "OxGST_declare" );

   if( istype(sym,set) )return;

   if( ! isident(sym) )
      FAULT("Symbol is not an identifier in OxGST_declare");

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

   newvar->tsub   = 0;
   for( ele=newvar->sets->first ; ele ; ele=ele->next )
      if( isequal(ele->str,"time") || issubset(ele->str,"time") || isaliasof(ele->str,"time") )
         break;
      else
         newvar->tsub++;

   if( newvar->tsub > newvar->sets->n )
      OxGST_error( "Variable has no time subscript: %s", name );

   if( newvar->tsub < (newvar->sets->n - 1) )
      OxGST_error( "Time subscript must be last: %s", name );

   // 
   //  if it's an endogenous variable, add all of its elements to the 
   //  endogenous list and set the LHS count for each one to 0.
   //

   if( isendo )
      {
      Endogenous *newendo;
      Endogenous *lastendo;

      count = symsize(sym);
      if( count<1 )FAULT("Variable has no element count in OxGST_declare");
      OxGST_endog += count;

      cart_build( newvar->sets );

      while( cart=cart_next() )
         {
         newendo  = (Endogenous *) malloc( sizeof(Endogenous) );
         newendo->obj  = MYEND;
         newendo->str  = OxGST_mkname(name,cart);
         newendo->off  = offset++;
         newendo->lhs  = 0;
         if( cart->n == 0 )
            newendo->mat  = strdup(name);
         else
            newendo->mat  = concat(4,name,"(",slprint(cart),")");
         newendo->next = 0;
      
         newendo->basename = OxGST_csvname(newvar,cart);
         newendo->t_index  = OxGST_time(newvar->tsub,cart);

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
      printf("OxGST_declare: %s has %d elements\n",name,count);

   //
   //  insert it into the list in alphabetical order
   //

   //  case 1: list is empty
   
   if( v_head == 0 ) 
      {
      v_head = newvar;
      OxGST_declcode(newvar);
      return;
      }

   cur = v_head;
   check = strcasecmp(name,cur->str);

   if( check == 0 )
      FAULT("Multiple definitions of variable in OxGST_declare");

   //  case 2: insert before first existing entry

   if( check < 0 ) 
      {
      newvar->next = v_head;
      v_head = newvar;
      OxGST_declcode(newvar);
      return;
      }

   //  case 3: anywhere else except the end

   for( nxt=cur->next ; nxt ; nxt=nxt->next )
      {
      check = strcasecmp(name,nxt->str);
      if( check == 0 )
         FAULT("Multiple definitions of variable in OxGST_declare");
      if( check  < 0 )
         {
         newvar->next = nxt;
         cur->next = newvar;
         OxGST_declcode(newvar);
         return;
         }
      cur=nxt; 
      }

   //  case 4: append after last element

   cur->next = newvar;
   OxGST_declcode(newvar);
}


//----------------------------------------------------------------------//
//  OxGST_begin_file()
//
//  Begin processing the file
//----------------------------------------------------------------------//

void OxGST_begin_file(char *basename)
{
   char *fname_decl,*fname_init,*fname_csv;
   char *fname_csvin,*fname_csvout;
   
   fname_decl = concat(2,basename,"_decl.h");
   incfile = fopen(fname_decl,"w");
   if( incfile==0 )
      OxGST_error("Could not create include file: %s",fname_decl);
   
   fname_init = concat(2,basename,"_init.h");
   initfile = fopen(fname_init,"w");
   if( initfile==0 )
      OxGST_error("Could not create include file: %s",fname_init);
   
   fname_csv = concat(2,basename,"_tmp.csv");
   csvfile = fopen(fname_csv,"w");
   if( csvfile==0 )
      OxGST_error("Could not create template csv file: %s",fname_csv);

   fname_csvin  = concat(2,basename,"_in.csv");
   fname_csvout = concat(2,basename,"_out.csv");
   
   fprintf(code,"#include <oxstd.h>\n");
   fprintf(code,"\n");
   fprintf(code,"//\n");
   fprintf(code,"// mode controls updating:\n");
   fprintf(code,"//   0 = evaluate all equations before updating\n");
   fprintf(code,"//   1 = update after each equation\n");
   fprintf(code,"//   2 = print LHS and RHS for each equation, then quit\n");
   fprintf(code,"//\n");
   fprintf(code,"\n");
   fprintf(code,"decl mode=1;\n");
   fprintf(code,"\n");
   fprintf(code,"//\n");
   fprintf(code,"// verbose controls output:\n");
   fprintf(code,"//   0 = print only the final guess vector\n");
   fprintf(code,"//   1 = print each guess vector\n");
   fprintf(code,"//   2 = same as 1 but also show intermediate calcs\n");
   fprintf(code,"//\n");
   fprintf(code,"\n");
   fprintf(code,"decl verbose=0;\n");
   fprintf(code,"\n");
   fprintf(code,"//\n");
   fprintf(code,"// gauss-seidel control parameters:\n");
   fprintf(code,"//   gs_weight  = weight of guess in revision [0,1]\n");
   fprintf(code,"//   gs_tol     = convergence tolerance\n");
   fprintf(code,"//   gs_maxit   = maximum iterations\n");
   fprintf(code,"//   gs_logfile = CSV log file for results\n");
   fprintf(code,"//\n");
   fprintf(code,"\n");
   fprintf(code,"decl gs_weight = 0.5;\n");
   fprintf(code,"decl gs_tol    = 1e-7;\n");
   fprintf(code,"decl gs_maxit  = 400;\n");
   fprintf(code,"decl gs_logfile = \"%s.csv\";\n",basename);
   fprintf(code,"\n");
   fprintf(code,"//\n");
   fprintf(code,"// csv data file to use for input and output data:\n");
   fprintf(code,"//\n");
   fprintf(code,"\n");
   fprintf(code,"decl csv_input_data = \"%s\"; \n",fname_csvin);
   fprintf(code,"decl csv_output_data = \"%s\"; \n",fname_csvout);
   fprintf(code,"\n");
   fprintf(code,"//\n");
   fprintf(code,"//  function prototypes\n");
   fprintf(code,"//\n");
   fprintf(code,"\n");
   fprintf(code,"init();\n");
   fprintf(code,"readcsv(filename);\n");
   fprintf(code,"getval(basename,timesub);\n");
   fprintf(code,"setval(basename,timesub,value);\n");
   fprintf(code,"actual(guess);\n");
   fprintf(code,"output(values);\n");
   fprintf(code,"writecsv(filename);\n");
   fprintf(code,"\n");
   fprintf(code,"//\n");
   fprintf(code,"//  assemble all the pieces via includes \n");
   fprintf(code,"//\n");
   fprintf(code,"\n");
   fprintf(code,"#include \"gauss-seidel-gst.ox\"\n");
   fprintf(code,"#include \"simple-csv.ox\"\n",basename);
   fprintf(code,"#include \"%s\"\n",fname_decl);
   fprintf(code,"#include \"%s\"\n",fname_init);
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

   fprintf(initfile,"decl vname;\n");
   fprintf(initfile,"decl vtime;\n");
   fprintf(initfile,"\n");
   fprintf(initfile,"init()\n");
   fprintf(initfile,"{\n");
   fprintf(initfile,"   readcsv(csv_input_data);\n");
   fprintf(initfile,"\n");

   free(fname_decl);
   free(fname_init);
}


//----------------------------------------------------------------------//
//
//  End processing the file
//
//----------------------------------------------------------------------//

void OxGST_end_file()
{
   //
   //  build actuals vector
   //
   
   OxGST_guess_actuals(1);
   
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
   fprintf(initfile,"   for( i=0 ; i<%d ; i++ )\n",OxGST_endog);
   fprintf(initfile,"      setval(vname[i],vtime[i],values[i][]);\n");
   fprintf(initfile,"   writecsv(csv_output_data);\n");
   fprintf(initfile,"}\n");   
   
   fprintf(info,"\n");
   fprintf(info,"Total Equation Count: %d\n",OxGST_scalar);
   fprintf(info,"Total Endogenous Variables: %d\n",OxGST_endog);

   fclose(incfile);
   fclose(initfile);
}

//----------------------------------------------------------------------//
//
//  Begin an equation
//
//----------------------------------------------------------------------//

void OxGST_begin_block(void *eq)
{
   List *esets,*eqnsets();
   int nblk,nstart,nscalar,nend;

   if( DBG )printf("   backend begin_block\n");

   nblk    = OxGST_block++;
   nstart  = OxGST_scalar;
   nscalar = eqncount(eq);
   nend    = nstart+nscalar-1;
   
   OxGST_scalar = nend+1;

   esets = eqnsets(eq);
   
   if( ! islvalue(eq) )
      OxGST_error("%s","LHS of equation is not a variable");
      
   //
   //  if this is the first block, finish initialization of files
   //  before writing the equation
   //

   if( nblk == 1 )OxGST_guess_actuals(0);

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

   OxGST_next_is_lhs = 1;

}

//----------------------------------------------------------------------//
//
//  Begin a scalar equation
//
//----------------------------------------------------------------------//

void OxGST_begin_eqn(void *eq)
{
   OxGST_next_is_lhs = 1;
}


//----------------------------------------------------------------------//
//
//  End a scalar equation
//
//----------------------------------------------------------------------//

void OxGST_end_eqn(void *eq)
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

char *OxGST_show_symbol(char *str, List *sublist, Context context)
{
   char *ptr;
   int efound;
   
   ptr = OxGST_getname(str,sublist,context) ;
   if( DBG )printf("OxGST_show_symbol: %s\n",ptr);

   if( OxGST_next_is_lhs )
      {
      Endogenous *e;
      efound = 0;
      
      for( e=e_head ; e ; e=e->next )
         if( strcasecmp(ptr,e->str)==0)
            {
            efound = 1;
            e->lhs++;
            if( e->lhs > 1 )
               OxGST_error("Variable appears on LHS of multiple equations: %s",ptr);
            break;
            }
      if( efound==0 )
         OxGST_error("Variable on LHS is not endogenous: %s",ptr);

      OxGST_next_is_lhs = 0;

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

void OxGST_setup(void)
{
   lang_begin_file ( OxGST_begin_file  ); 
   lang_end_file   ( OxGST_end_file    );
   lang_declare    ( OxGST_declare     ); 
   lang_begin_block( OxGST_begin_block ); 
   lang_begin_eqn  ( OxGST_begin_eqn   ); 
   lang_end_eqn    ( OxGST_end_eqn     ); 
   lang_show_symbol( OxGST_show_symbol );

   set_eqn_scalar();
   set_eqn_lvalue();
   set_sum_scalar();
   set_explicit_time();
}

char *OxGST_version = "$Revision: 57 $";
