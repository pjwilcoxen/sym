/*--------------------------------------------------------------------*
 *  SYMBOL.C 4.00
 *  9 Apr 90 (PJW)
 *
 *  Driver routine for symbolic math program.  Processes the command
 *  line and then calls yyparse() to do the actual work.
 *
 *  $Id: sym.c 65 2018-06-17 02:56:59Z wilcoxen $
 *--------------------------------------------------------------------*/

#include "sym.h"

#include "build.h"
#include "eqns.h"
#include "error.h"
#include "lang.h"
#include "lists.h"
#include "nodes.h"
#include "output.h"
#include "readfile.h"
#include "sets.h"
#include "str.h"
#include "symtable.h"
#include "version.h"
#include "xmalloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define  myDEBUG 0

FILE *code=0;
FILE *info=0;

int debug=0;
int debugforce=0;
int intertemporal=0;
int only_first=0;
int only_last=0;
int mergeonly=0;

char *usage   = "sym [options] <language> <symfile> <codefile>";
char *options = "-version -d -dd -doc -first -last -syntax -merge_only";

char *doc1 = "\
Translates models from algebraic form into one of several programming\n\
languages.";

char *doc2 = "\
## Details:\n\
\n\
### Argument language\n\
Indicates the target output language and must be one of the alternatives\n\
listed under the Languages heading. Required unless the -merge_only option\n\
is used.\n\
\n\
### Argument symfile\n\
Indicates the input model to be translated. Required.\n\
\n\
### Argument codefile\n\
The name that should be used for the resulting target-language file. Some\n\
target languages will involve multiple files that will be based on this\n\
name. Required.\n\
\n\
### Option -d\n\
Turn on the most commonly used debugging messages.\n\
\n\
### Option -dd\n\
Turn on all debugging messages.\n\
\n\
### Option -doc\n\
Print this message.\n\
\n\
### Option -first\n\
Build a single-year model using only the first year.\n\
\n\
### Option -last\n\
Build a single-year model using only the last year.\n\
\n\
### Option -merge_only\n\
Combine all included modules and return the resulting file\n\
without generating any target-language code.\n\
\n\
### Option -syntax\n\
Print a short summary of the input syntax.\n\
\n\
### Option -version\n\
Print detailed information about the versions of the main\n\
program and the individual language support modules.\n\
\n\
### Language -debug\n\
Produce a text file for debugging.\n\
\n\
### Language -html\n\
Produce an HTML file that can be used for documentation.\n\
\n\
### Language -tablo\n\
GEMPACK's TABLO language.";

void build_langinfo(void);
List *knownlangs(void);
List *langvers(void);
void syntax();
void parse_command(int,char*[]);
int isoption(char*,int);
static char *builtby();

void main(argc,argv)
int argc;
char *argv[];
{
   void listsymbols();
   char *argument();
   char *get_version();
   char *sourcefile,*codefile,*listfile;
   char *basename,*ext;
   char *lang;
   char *rev;
   char *h1,*h2;
   List *known;
   Item *thislang;
   int  do_doc;
   int  do_usage;

   lang = "debug";

   rev = strdup(get_version(rcs));

   parse_command(argc,argv);

   build_langinfo();
   known = knownlangs();
   validate( known, LISTOBJ, "sym main program" );

   if( isoption("version",1) )
      {
      printf( "symbol %s\n",verstr);
      printf( "--- compiler: %s\n",builtby());
      printf( "--- base code: v%s\n", rev );
      known = langvers();
      for( thislang=known->first ; thislang ; thislang=thislang->next )
         printf("--- module %s\n",thislang->str);
      exit(0);
      }

   if( isoption("syntax"  ,1) )
      {
      syntax();
      exit(0);
      }

   do_doc   = isoption("doc",2);
   do_usage = isoption("?",1) || isoption("help",1);
   if( argument(1) == NULL && do_doc == 0 )do_usage = 1;
   
   if( do_doc || do_usage )
      {
      h1 = do_doc ? "# " : "" ;
      h2 = do_doc ? "## " : "" ;
      if( do_doc ) {
         printf( "%sSym Usage\n\n",h1 );
         printf( "%s\n", doc1 );
      }
      printf( "\n%sUsage:\n    %s\n\n", h2, usage );
      printf( "%sOptions:\n    %s\n\n", h2, options );
      printf( "%sLanguages:\n    ", h2);
      for( thislang=known->first ; thislang ; thislang=thislang->next )
         printf("-%s ",thislang->str);
      printf( "\n\n");
      if( do_doc ) {
         printf( "%s\n\n", doc2 );
         printf( "Sym %s\n",verstr );
      }
      if( do_usage )
         printf( "See also:\n    sym -doc and sym -syntax\n" );
      exit(0);
      }

   if( isoption("dd"        ,2) )debugforce = 1;
   if( isoption("d"         ,1) && !(isoption("debug",2) || isoption("dd",2)) )debug = 1;
   if( isoption("first"     ,1) )only_first = 1;
   if( isoption("last"      ,1) )only_last = 1;
   if( isoption("merge_only",1) )mergeonly =1;

   if( only_first && only_last )
      {
      printf( "Cannot use -first and -last simultaneously\n\n" );
      exit(0);
      }

   //
   //  set the language and initialize the backend language module 
   //

   if( mergeonly==0 ) {
      lang = 0;
      for( thislang=known->first ; thislang ; thislang=thislang->next )
         if( isoption(thislang->str,strlen(thislang->str)) )lang=thislang->str;

      if( lang==0 )
         fatal_error("%s","No target language specified\n");
      
      set_language(lang);
   }

   //
   //  assemble file names
   //  

   sourcefile = argument(0);
   codefile   = argument(1);

   basename = strdup(codefile);
   if( ext=strrchr(basename,'.') )*ext = '\0';
   listfile = concat(2,basename,".lis");

   //
   //  open the output files; input file will be opened by read_file
   //
   
   code = fopen(codefile,"w");
   if( code==0 )
      fatal_error("Could not open output code file %s\n",codefile);

   if( mergeonly==0 ) {
      info = fopen(listfile,"w");
      if( info==0 )
         fatal_error("Could not open output list file %s\n",listfile);
      fprintf(info,"Run Specifications:\n");
      fprintf(info,"   Sym %s\n", verstr);
      fprintf(info,"   Target language: %s\n", lang);
   }

   //
   //  parse the file(s)
   //

   if( DBG )xcheck("init");

   read_source(sourcefile);
   if( DBG )xcheck("after read");
 
   //
   //  exit if merge_only is on
   //

   if( mergeonly ) {
      printf("Merged code written to %s\n",codefile);
      exit(0);
   }

   //
   //  there must be at least one equation
   //

   if( num_eqns() == 0 )
      fatal_error("No equations found in input file %s\n",sourcefile);

   //
   //  see if this is an intertemporal model by building the 
   //  identifier context information for each equation
   //

   build_context();

   //
   //  figure out how sets are related
   //
   
   build_set_relationships();
   if( DBG )xcheck("after build sets");
   
   //
   //  check to make sure that no variables or parameters are
   //  defined over invalid domains.
   //
   
   check_identifiers();
   
   //
   //  write out information about symbols to the listing file
   //
   
   listsymbols();
   listelements();
   if( DBG )xcheck("after listing");

   //
   //  prepare the equations
   //
  
   check_equations();
   if( DBG )xcheck("after check_equations");

   //
   //  write the code file
   //
   
   if( error_count()==0 || DBG )
      write_file(basename);
   else
      fatal_error("%s","No code file created");

   if( DBG )xcheck("after write_file");
   
   //
   //  all done
   //

   if( DBG )
      {
      long mem;
      xcheck("at end");
      mem = xmark();
      printf("Net memory allocations: %ld\n",mem);
      }

   exit(0);
}

// 
//  builtby()
//
//  Pull out preprocessor directives so we can report how 
//  we were built. Convenient for supporting multiple
//  compilers.
//

static char *builtby() 
{
   static char buf[256];
   int maj,min;

   strcpy(buf,"undetermined");

#if defined(__CYGWIN__) && defined(__GNUC__)
   sprintf(buf,"cygwin gcc %d.%d",__GNUC__,__GNUC_MINOR__);
#endif

#if defined(__linux__) && defined(__GNUC__)
   sprintf(buf,"linux gcc %d.%d",__GNUC__,__GNUC_MINOR__);
#endif

#if defined(__MINGW32__)
   sprintf(buf,"mingw32 gcc 32-bit");
#endif
#if defined(__MINGW64__)
   sprintf(buf,"mingw32 gcc 64-bit");
#endif

#ifdef __WATCOMC__
   maj = __WATCOMC__/100;
   min = __WATCOMC__-100*maj;
   sprintf(buf,"watcom c %d.%d",maj,min);
#endif 

   return(buf);
}
