#include "lang.h"

#include "assoc.h"
#include "codegen.h"
#include "error.h"
#include "lists.h"
#include "str.h"
#include "sym.h"
#include <stdlib.h>

void  (*codegen_begin_file )();
void  (*codegen_end_file   )();
void  (*codegen_declare    )();
void  (*codegen_begin_block)();
void  (*codegen_begin_eqn  )();
void  (*codegen_end_eqn    )();
char* (*codegen_begin_func )();
char* (*codegen_end_func   )();
char* (*codegen_show_symbol)();

static Array *langinit=0;
static List  *known=0;
static List  *versions=0;

char *get_version();

void build_langinfo()
{
   known    = newlist();
   versions = newlist();
   langinit = newarray();
   setup_languages();
}


void addlang(char *lang, void (*fnc)(), char *ver )
{
   char *verstr;
   
   if( lang==0 || ver==0 )
      FAULT("Null pointer passed to addlang");

   if( ismember(lang,known) )
      FAULT("Multiple definitions for a language");

   verstr = concat(3,lang,": v",get_version(ver));
   
   addlist( known, lang );
   addlist( versions, verstr );
   addvalue( langinit, lang, fnc );

   free(verstr);
}


List *knownlangs()
{
   return known;
}


List *langvers()
{
   return versions;
}


void set_language(char *lang)
{
   void Default_setup(void);
   void (*initfunc)(void);
   
   if( lang==0 )
      FAULT("Null pointer passed to setlang");

   if( ismember(lang,known)==0 )
      FAULT("Unknown language");

   initfunc = getvalue(langinit,lang);

   Default_setup();
   initfunc();

   if( codegen_show_symbol==0 )
      FAULT("Selected language does not provide show_symbol");
}


void lang_begin_file(void (*fnc)())
{
   codegen_begin_file = fnc;
}

void lang_end_file(void (*fnc)())
{
   codegen_end_file = fnc;
}

void lang_declare(void (*fnc)())
{
   codegen_declare = fnc;
}

void lang_begin_block(void (*fnc)())
{
   codegen_begin_block = fnc;
}

void lang_begin_eqn(void (*fnc)())
{
   codegen_begin_eqn = fnc;
}

void lang_end_eqn(void (*fnc)())
{
   codegen_end_eqn = fnc;
}

void lang_begin_func(char* (*fnc)())
{
   codegen_begin_func = fnc;
}

void lang_end_func(char* (*fnc)())
{
   codegen_end_func = fnc;
}

void lang_show_symbol(char* (*fnc)())
{
   codegen_show_symbol = fnc;
}
