/*--------------------------------------------------------------------*
 *  default.c
 *  Dec 04 (PJW)
 *
 *  Default_setup() will be called automatically just before the
 *  actual language is initialized.  The actual language only 
 *  needs to override methods that need to be changed from their
 *  default behavior.
 *--------------------------------------------------------------------*/

#include "lang.h"
#include "output.h"
#include "str.h"
#include "sym.h"
#include <stdio.h>
#include <string.h>

void Default_begin_file(char *basename)
{
}

void Default_end_file()
{
}

void Default_declare(void *sym)
{
}

void Default_begin_block(void *eq)
{
}

void Default_begin_eqn(void *eq)
{
}

void Default_end_eqn(void *eq)
{
   fprintf(code," ;\n\n");
}

char *Default_begin_func(char *func, char *arg)
{
   if( arg )return concat(4,func,"(",arg,",");
   return concat(2,func,"("); 
}

char *Default_end_func()
{
   return strdup(")");
}

//
//  Connect up the public routines.
//

void Default_setup(void)
{
   lang_begin_file ( Default_begin_file  ); 
   lang_end_file   ( Default_end_file    );
   lang_declare    ( Default_declare     );
   lang_begin_block( Default_begin_block ); 
   lang_begin_eqn  ( Default_begin_eqn   ); 
   lang_end_eqn    ( Default_end_eqn     );
   lang_begin_func ( Default_begin_func  );
   lang_end_func   ( Default_end_func    );
   lang_show_symbol( 0                   );
}

