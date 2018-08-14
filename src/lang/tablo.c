/*--------------------------------------------------------------------*
 *  tablo.c
 *  Dec 04 (PJW)
 *   
 *  Backend routine generating a tablo file. One attribute is
 *  allowed in variable and parameter declarations. If present,
 *  it is used as the name of an HAR header and should have the
 *  form: c### where c is one of the letters below and is used 
 *  to  infer the TABLO logical name:
 *
 *     B = base
 *     K = kalman
 *     M = make
 *     N = endog
 *     X = exog
 *
 *  $Id: tablo.c 57 2018-06-16 19:50:13Z wilcoxen $
 *--------------------------------------------------------------------*/

#include "../assoc.h"
#include "../error.h"
#include "../lang.h"
#include "../options.h"
#include "../output.h"
#include "../sets.h"
#include "../str.h"
#include "../sym.h"
#include "../symtable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define  myDEBUG 1

char *getsupset(char*);

// 
//  Flag for checking local Tablo objects for corruption
//

#define MYOBJ 3001


//
//  Internal variables
//

struct tabset {
   int obj;
   char *index;
   int  istime;
   };
typedef struct tabset Tabset;

Array *tabsets=0;

static int Tablo_eqn=0;
static int Tablo_var=0;
static int Tablo_par=0;

//----------------------------------------------------------------------//
//  tabloset
//
//  Add a set to the internal list.
//----------------------------------------------------------------------//
static void tabloset(char *name)
{
   static struct tabset *new;
   char abb[2];
   
   abb[0] = *name;
   abb[1] = '\0';
   
   new = (Tabset *) malloc( sizeof(Tabset) );
   new->obj    = MYOBJ;
   new->index  = strdup(abb);
   new->istime = 0;
   
   if( isequal(name,"time") || issubset(name,"time") )
      new->istime = 1;
   
   if( tabsets==0 )tabsets = newarray();
   addvalue(tabsets,name,new);
}


//----------------------------------------------------------------------//
//  tablovar
//
//  Generate a variable reference from a name and a list of sets.
//----------------------------------------------------------------------//
static char *tablovar(char *name, List *sets, int dt)
{
   Item *s;
   List *indexes;
   Tabset *cur;
   char *result;
   char tbuf[10];
   
   validate( tabsets, ARRAYOBJ, "tablovar" );
   if( name==0 )FAULT("null pointer passed to tablovar");

   if( sets==0 )return strdup(name);
   validate( sets, LISTOBJ, "tablovar" );

   if( sets->n == 0 )return strdup(name);

   indexes = newsequence();
   for( s=sets->first ; s ; s=s->next )
      {
      if( isimplicit(s->str) )
         {
         result = concat(3,"\"",s->str,"\"");
         addlist(indexes,result);
         free(result);
         continue;
         }
      cur = (Tabset *) getvalue(tabsets,s->str);
      if( cur->istime && dt )
         {
         sprintf(tbuf,"%s%+d",cur->index,dt);
         addlist(indexes,tbuf);
         }
      else
         addlist(indexes,cur->index);
      }
      
   result = concat(4,name,"(",slprint(indexes),")");
   freelist(indexes);
   return result;
}


//----------------------------------------------------------------------//
//  tabloqualifier
//
//  Generate a tablo qualifier from a list of sets.
//----------------------------------------------------------------------//
static char *tabloqualifier(List *sets)
{
   Item *s;
   Tabset *cur;
   char *result,*new;

   validate( sets, LISTOBJ, "tabloqualifier" );

   result = strdup("");
   for( s=sets->first ; s ; s=s->next )
      {
      if( isimplicit(s->str) )continue;
      cur = (Tabset *) getvalue(tabsets,s->str);
      new = concat(6,result,"(all,",cur->index,",",s->str,") ");
      free(result);
      result = new;
      }

   return result;
}


//----------------------------------------------------------------------//
//  tablo_error()
//
//  Print an error message and crash.  Use this routine instead of
//  fatal_error in order to indicate that the problem was with the
//  Tablo back end.
//----------------------------------------------------------------------//
void tablo_error(char *fmt, char *str)
{
   show_error("Fatal Error Writing Tablo File",fmt,str);
}


//----------------------------------------------------------------------//
//  Tablo_declare
//----------------------------------------------------------------------//
void Tablo_declare(void *sym)
{
   char *name;
   if( istype(sym,set) )
      {
      name = symname(sym);
      tabloset(name);
      free(name);
      }
   if( istype(sym,var) )Tablo_var++;
   if( istype(sym,par) )Tablo_par++;
}


//----------------------------------------------------------------------//
//  Tablo_writedecs
//
//  Write out the declaration part of the file.
//----------------------------------------------------------------------//

static void Tablo_writedecs()
{
   Element *cur;
   Tabset *cts;
   char buf[10],*name,*qual,*ref,*iqual,*stmt;
   int  n;
   List *val;
   List *sofar;
   int  hdr=0;
   List *atts;
   char *filename;

   //
   //  make unique set index variables
   //

   if( tabsets )
      {
      sofar = newlist();
      for( cur=tabsets->first ; cur ; cur=cur->next )
         {
         n=1;
         cts = (Tabset *) cur->value;
         while( lookup(cts->index) || ismember(cts->index,sofar) )
            {
            sprintf(buf,"%c%d",*(cts->index),n++);
            free(cts->index);
            cts->index = strdup(buf);
            }
         addlist( sofar, cts->index );
         }
      sofar = freelist(sofar);
      }
      
   //
   //  write out sets
   //
   
   for( cur=firstsymbol(set), n=0 ; cur ; cur=nextsymbol(cur), n++ )
      {
      name = symname(cur);
      val  = symvalue(cur);
      cts = (Tabset *) getvalue(tabsets,name);
      
      iqual = cts->istime ? "(intertemporal) " : "" ;
      stmt  = concat(6,"set ",iqual,name," (",slprint(val),") ;"); 
      wrap_write(stmt,1,1);
      free(stmt);

      freelist(val);
      free(name);
      }

   if( n )fprintf(code,"\n");
   
   // 
   //  write out subset statements
   //
   
   for( cur=firstsymbol(set), n=0 ; cur ; cur=nextsymbol(cur), n++ )
      {
      char *supset;
      name   = symname(cur);
      supset = getsupset(name);
      if( supset )
         {
         fprintf(code,"subset %s is subset of %s ;\n",name,supset);
         free(supset);
         }
      free(name);
      }

   if( n )fprintf(code,"\n");
   
   //
   //  write coefficient declarations
   //
   
   for( cur=firstsymbol(par), n=0 ; cur ; cur=nextsymbol(cur), n++ )
      {
      name = symname(cur);
      val  = symvalue(cur);
      qual = tabloqualifier(val);
      ref  = tablovar(name,val,0);
      
      stmt = concat(4,"coefficient ",qual,ref," ;");
      wrap_write(stmt,1,0);
      free(stmt);
      
      freelist(val);
      free(name);
      free(qual);
      free(ref);
      }

   if( n )fprintf(code,"\n");
   
   //
   //  write out some placeholder read statements
   //

   if( n )fprintf(code,"file param ;\n\n");

   for( cur=firstsymbol(par), n=0 ; cur ; cur=nextsymbol(cur), n++ )
      {
      name = symname(cur);
      val  = symvalue(cur);
      qual = tabloqualifier(val);
      ref  = tablovar(name,val,0);

      atts = symattrib(cur);
      if( atts->n == 1 )
         strncpy(buf,atts->first->str,10);
      else
         sprintf(buf,"H%3.3d",hdr++);

      stmt = concat(7,"read ",qual,"\n   ",ref," from file param header \"",buf,"\" ;");
      wrap_write(stmt,1,0);
      free(stmt);
      
      free(name);
      freelist(val);
      free(qual);
      free(ref);
      }

   if( n )fprintf(code,"\n");

   //
   //  write out variable statements
   // 
    
   for( cur=firstsymbol(var) ; cur ; cur=nextsymbol(cur) )
      {
      name = symname(cur);
      val  = symvalue(cur);
      qual = tabloqualifier(val);
      ref  = tablovar(name,val,0);
      
      stmt = concat(4,"variable ",qual,ref," ;");
      wrap_write(stmt,1,0);
      free(stmt);
      
      free(name);
      freelist(val);
      free(qual);
      free(ref);
      }

   //
   //  write out some placeholder read statements
   //

   fprintf(code,"\nfile base ;\n");
   fprintf(code,"file exogen ;\n");
   fprintf(code,"file make ;\n");
   fprintf(code,"file kalman ;\n\n");

   for( cur=firstsymbol(var) ; cur ; cur=nextsymbol(cur) )
      {
      name = symname(cur);
      val  = symvalue(cur);
      qual = tabloqualifier(val);
      ref  = tablovar(name,val,0);
      
      filename = "base";
      atts = symattrib(cur);
      if( atts->n == 1 ) {
         strncpy(buf,atts->first->str,10);
         switch( *buf ) {
            case 'B': filename = "base";   break;
            case 'K': filename = "kalman"; break;
            case 'M': filename = "make";   break;
            case 'N': filename = "endog";  break;
            case 'X': filename = "exog";   break;
            default:  filename = "base";   break;
         }
      }
      else
         sprintf(buf,"H%3.3d",hdr++);

      stmt = concat(9,"read ",qual,"\n   ",ref," from file ",filename," header \"",buf,"\" ;");
      wrap_write(stmt,1,0);
      free(stmt);
      
      free(name);
      freelist(val);
      free(qual);
      free(ref);
      }

}


//----------------------------------------------------------------------//
//  Tablo_begin_file()
//
//  Begin processing the file
//----------------------------------------------------------------------//

void Tablo_begin_file(char *basename)
{
   fprintf(code,"equation    (default=levels)    ;\n");
   fprintf(code,"variable    (default=levels)    ;\n");
   fprintf(code,"coefficient (default=parameter) ;\n");
   fprintf(code,"\n");
}


//----------------------------------------------------------------------//
//
//  End processing the file
//
//----------------------------------------------------------------------//

void Tablo_end_file()
{
   int ucount;
   void *cur;

   ucount = 0;
   for( cur=firstsymbol(var) ; cur ; cur=nextsymbol(cur) ) 
      if( !isused(cur) )
         ucount++;

   fprintf(info,"\n");
   fprintf(info,"Equations: %d\n",Tablo_eqn);
   fprintf(info,"Variables, Used: %d\n",Tablo_var-ucount);
   fprintf(info,"Variables, Unused: %d\n",ucount);
   fprintf(info,"Parameters: %d\n",Tablo_par);
}

//----------------------------------------------------------------------//
//
//  Begin an equation
//
//----------------------------------------------------------------------//

void Tablo_begin_block(void *eq)
{
   char *qual;
   List *eqnsets();
   char *name,*eqname();
   
   if( Tablo_eqn == 0 )
      Tablo_writedecs();

   Tablo_eqn++;

   qual = tabloqualifier(eqnsets(eq));
   name = eqname(eq);
   if( name )
      fprintf(code,"\nequation %s %s\n   ",name,qual);
   else
      fprintf(code,"\nequation EQN%d %s\n   ",Tablo_eqn,qual);
   free(qual);
}


//----------------------------------------------------------------------//
//
//  End an equation
//
//----------------------------------------------------------------------//

void Tablo_end_eqn(void *eq)
{

   fprintf(code," ;\n");

}

//----------------------------------------------------------------------//
//
//  Begin a function
//  
//----------------------------------------------------------------------//

char *Tablo_begin_func(char *func, char *arg)
{
   Tabset *cur;
   
   if( isequal(func,"sum") || isequal(func,"prod") )
      {
      cur = (Tabset *) getvalue(tabsets,arg);
      if( cur==0 )FAULT("null pointer in Tablo_begin_func");
      return concat(6,func,"(",cur->index,",",arg,",");
      }
      
   if( arg )
      FAULT("unexpected function call in Tablo_begin_func");

   if( isequal(func,"log") )
      return strdup("loge(");
      
   return concat(2,func,"("); 
}

//----------------------------------------------------------------------//
//
//  Show a symbol
//  
//----------------------------------------------------------------------//

char *Tablo_show_symbol(char *str, List *setlist, Context context)
{
   return tablovar(str,setlist,context.dt);
}


//----------------------------------------------------------------------//
//
//  Connect up public routines
//  
//----------------------------------------------------------------------//

void Tablo_setup(void)
{
   lang_begin_file ( Tablo_begin_file  ); 
   lang_end_file   ( Tablo_end_file    );
   lang_declare    ( Tablo_declare     ); 
   lang_begin_block( Tablo_begin_block ); 
   lang_end_eqn    ( Tablo_end_eqn     ); 
   lang_begin_func ( Tablo_begin_func  );
   lang_show_symbol( Tablo_show_symbol );

   set_eqn_vector();
   set_sum_vector();
   
   set_line_length(75);
   set_alpha_elements();
   set_explicit_time();
   set_reserved_word("to");
   set_reserved_word("from");
}

char *Tablo_version = "$Revision: 57 $";

