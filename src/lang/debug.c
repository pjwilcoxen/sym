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
#include "../dict.h"
#include "../eqns.h"
#include "../error.h"
#include "../lang.h"
#include "../options.h"
#include "../output.h"
#include "../str.h"
#include "../sym.h"
#include "../symtable.h"
#include "../xmalloc.h"
#include "../wprint.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define  myDEBUG 1

//
//  Object ID for scalars
//

#define SCALOBJ 3030

//
//  Internal variables
//

static int  DB_block =1;
static int  DB_scalar=1;

static void *scalar_dict=0;

//
//  Scalar information file handle
//

FILE *fh_sca;

//
//  Variable types for handling information about scalars
//

typedef struct scalar_info {
   int obj;
   char *name;
   int dec;
   int lhs;
   int rhs;
   } Scalar ;

typedef enum {
   sca_dec, sca_lhs, sca_rhs
   } ScalarRef ;

//
//  Private methods
//

//----------------------------------------------------------------------//
//  saw_scalar()
//  
//  Count the appearance of a scalar in different contexts.
//----------------------------------------------------------------------//

static void saw_scalar(char *name, void *subs, ScalarRef where) 
{
   Scalar *cur;
   char *fullname;

   validate( name, 0, "saw_scalar" );
   
   if( scalar_dict == 0 ) 
      scalar_dict = newdict(60000);
   
   if( subs )
      fullname = concat(4,name,"(",slprint(subs),")");
   else
      fullname = name;

   cur = (Scalar *) getdict(scalar_dict,fullname);
   if( cur == 0 )
      {
      cur = (Scalar *) xmalloc( sizeof(Scalar) );
      cur->obj = SCALOBJ;
      cur->name = strdup(fullname);
      cur->dec  = 0;
      cur->lhs  = 0;
      cur->rhs  = 0;
      putdict(scalar_dict,fullname,cur);
      }
   
   validate( cur, SCALOBJ, "saw_scalar" );

   switch( where )
      {
      case sca_dec: cur->dec++; break;
      case sca_lhs: cur->lhs++; break;
      case sca_rhs: cur->rhs++; break;
      default:
         FAULT("unexpected op in saw_scalar");
      }

   if( subs )
      free( fullname );
}

//
//  Public methods
//

//----------------------------------------------------------------------//
//  DB_begin_file()
//
//  Begin processing the file
//----------------------------------------------------------------------//

void DB_begin_file(char *basename)
{
   char *fname;

   if( do_scalars )
      {
      fname  = concat(2,basename,"_scalars.csv");
      fh_sca = fopen(fname,"w");
      if( fh_sca == 0 )
         fatal_error("Could not create file: %s",fname);
      free( fname );
      }
}


//----------------------------------------------------------------------//
//  DB_end_file()
//
//  End processing the file. Do nothing unless do_scalars was set
//  and some scalars were seen. Then write the scalars file.
//----------------------------------------------------------------------//

void DB_end_file()
{
   List *keys;
   Item *cur;
   Scalar *entry;
   int unused = 0;

   if( do_scalars == 0 ) 
      return;
   
   if( scalar_dict == 0 ) 
      {
      fclose( fh_sca );
      return;
      }

   printf("writing information about scalars...");
   fflush(stdout);

   keys = getkeys(scalar_dict);

   fprintf(fh_sca,"name,dec,lhs,rhs\n");

   for( cur=keys->first ; cur ; cur=cur->next )
      {
      entry = getdict(scalar_dict,cur->str);
      if( entry==0 )
         FAULT("failed to find valid scalar key");
      if( (entry->lhs + entry->rhs) == 0 )
         unused++;
      fprintf(fh_sca,"\"%s\",%d,%d,%d\n",cur->str,entry->dec,entry->lhs,entry->rhs);
      }

   printf("done\n");
   fflush(stdout);

   fprintf(info,"\nScalar information:\n\n");
   fprintf(info,"   Total found: %d\n",keys->n);
   fprintf(info,"   Number unused: %d\n",unused);

   if( unused )
      printf("warning: %d scalars are unused\n",unused);

   fclose( fh_sca );
}


//----------------------------------------------------------------------//
//  DB_declare()
//
//  Declare a new variable or parameter.
//----------------------------------------------------------------------//

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
            saw_scalar(name,0,sca_dec);
         else
            {
            cart_build(valu);
            while( cur=cart_next() )
               saw_scalar(name,cur,sca_dec);
            }

   free(name);
   free(desc);
   freelist(valu);
   freelist(attr);
}


//----------------------------------------------------------------------//
//  DB_begin_block()
//
//  Begin an equation block
//----------------------------------------------------------------------//

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


//----------------------------------------------------------------------//
//  DB_show_symbol()
//
//  Show a parameter or variable.
//----------------------------------------------------------------------//

char *DB_show_symbol(char *str, List *sublist, Context context)
{
   char buf[1024],*ptr;
   int i;
   char *side[2];
   ScalarRef sop;

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

   sop = context.lhs ? sca_lhs : sca_rhs ;

   if( do_scalars )
      if( sublist->n == 0 ) 
         saw_scalar(str,0,sop);
      else
         saw_scalar(str,sublist,sop);

   return ptr;
}

//----------------------------------------------------------------------//
//  Debug_setup()
//
//  Connect up the public routines.
//----------------------------------------------------------------------//

void Debug_setup(void)
{
   lang_begin_file ( DB_begin_file  );
   lang_end_file   ( DB_end_file    );
   lang_declare    ( DB_declare     );
   lang_begin_block( DB_begin_block ); 
   lang_show_symbol( DB_show_symbol );

   set_eqn_scalar();
   set_sum_scalar();
}

char *Debug_version = "$Revision: 57 $";

