/*--------------------------------------------------------------------*
 * debug.c
 * Mar 04 (PJW)
 *--------------------------------------------------------------------*
.. ### debug
..
.. Write out in a format useful for checking equations.
..
.. + Equation descriptions will be written
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
#include "../wprint.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define  myDEBUG 1

static int DB_block =1;
static int DB_scalar=1;

FILE *fh_dec;
FILE *fh_use;


void DB_begin_file(char *basename)
{
   char *fname;

   if( do_scalars )
      {
      fname  = concat(2,basename,"_dec.csv");
      fh_dec = fopen(fname,"w");
      if( fh_dec == 0 )
         fatal_error("Could not create file: %s",fname);
      free( fname );
      
      fname  = concat(2,basename,"_use.csv");
      fh_use = fopen(fname,"w");
      if( fh_use == 0 )
         fatal_error("Could not create file: %s",fname);
      free( fname );
      }
}


void DB_declare(void *sym)
{
   char *name,*desc,*title,*supset;
   char *getsupset();
   List *valu,*attr;
   char buf[30];
   void *wp;
   Item *item;
   List *cur;

   validate( sym, SYMBOBJ, "DB_declare" );

   name = symname(sym);
   desc = symdescrip(sym);
   valu = symvalue(sym);
   attr = symattrib(sym);
   
   title = 0;
   if( istype(sym,set) )title = "Set";
   if( istype(sym,par) )title = "Parameter";
   if( istype(sym,var) )title = "Variable";
   
   if( title==0 )
         FAULT("Invalid symbol type in DB_declare");

   fprintf(code,"%s definition\n",title);
   fprintf(code,"   Name: %s\n",name);
      
   if( *desc )
      fprintf(code,"   Description: %s\n",desc);

   if( valu->n )
      {
      title = istype(sym,set) ? "Members" : "Sets" ;
      sprintf(buf,"   %s: ",title);
      wp = new_wprint(code,72);
      wprint_start(wp,buf);
      for( item=valu->first ; item ; item=item->next )
         {
         wprint_add(wp,item->str);
         if( item->next )wprint_add(wp,", ");
         wprint_break(wp);
         }
      wprint_end(wp);
      free_wprint(wp);
      }
      
   if( istype(sym,set) )
      {
      supset = getsupset(name);
      if( supset )fprintf(code,"   Subset of: %s\n",supset);
      }
   
   if( attr->n )
      fprintf(code,"   Attributes: %s\n",slprint(attr));

   fprintf(code,"\n");

   //  
   //  write out scalar declarations if needed
   //

   if( do_scalars ) 
      if( istype(sym,par) || istype(sym,var) )
         if( valu->n == 0 )
            fprintf(fh_dec,"%s,dec\n",name);
         else
            {
            cart_build(valu);
            while( cur=cart_next() )
               fprintf(fh_dec,"\"%s(%s)\",dec\n",name,slprint(cur));
            }

   free(name);
   free(desc);
   freelist(valu);
   freelist(attr);
}


void DB_begin_block(void *eq)
{
   List *esets,*eqnsets();
   int nblk,nstart,nscalar,nend;
   char *label;

   nblk    = DB_block++;
   nstart  = DB_scalar;
   nscalar = eqncount(eq);
   nend    = nstart+nscalar-1;
   
   DB_scalar = nend+1;

   esets = eqnsets(eq);
   label = eqnlabel(eq);

   if( label )
      fprintf(code,"// Equation block %d: %s\n",nblk,label);
   else
      fprintf(code,"// Equation block %d\n",nblk);

   if( esets->n )
      fprintf(code,"//    Defined over sets (%s)\n",slprint(esets) );

   if( nscalar )
      fprintf(code,"//    Scalar equations %d-%d (%d total)\n\n",nstart,nend,nscalar);
   else
      fprintf(code,"//    Contains undeclared symbols\n");
}


char *DB_show_symbol(char *str, List *sublist, Context context)
{
   char buf[1024],*ptr;
   int i;
   char *side[2];

   *buf = '\0';
   side[0] = "rhs";
   side[1] = "lhs";

   for( i=context.dt ; i < 0 ; i++ )strcat(buf,"lag(");
   for( i=context.dt ; i > 0 ; i-- )strcat(buf,"lead(");

   strcat(buf,str);
   if( sublist->n )
      {
      strcat(buf,"[");
      strcat(buf,slprint(sublist));
      strcat(buf,"]");
      }

   for( i=context.dt ; i < 0 ; i++ )strcat(buf,")");
   for( i=context.dt ; i > 0 ; i-- )strcat(buf,")");

   if( DBG )printf("db_show_symbol: %s\n",buf);

   ptr = strdup(buf);
   if( ptr == 0 )FAULT("Could not allocate memory in db_show_symbol");

   //
   //  write out scalar usage if needed
   //

   if( do_scalars )
      if( sublist->n == 0 )
         fprintf(fh_use,"%s,%s\n",str,side[context.lhs]);
      else
         fprintf(fh_use,"\"%s(%s)\",%s\n",str,slprint(sublist),side[context.lhs]);

   return ptr;
}

//
//  Connect up the public routines.
//

void Debug_setup(void)
{
   lang_begin_file ( DB_begin_file  );
   lang_declare    ( DB_declare     );
   lang_begin_block( DB_begin_block ); 
   lang_show_symbol( DB_show_symbol );

   set_eqn_scalar();
   set_sum_scalar();
}

char *Debug_version = "$Revision: 57 $";

