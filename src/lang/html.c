/*--------------------------------------------------------------------*
 * html.c
 * Aug 05 (PJW)
 *--------------------------------------------------------------------*
.. ### html
..
.. Write out the model in HTML for use as documentation.
..
.. + Equation descriptions will be written
 *--------------------------------------------------------------------*/

#include "../assoc.h"
#include "../eqns.h"
#include "../error.h"
#include "../lang.h"
#include "../options.h"
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

#define MYOBJ 3001 

static int HTML_block =1;
static int HTML_scalar=1;

//
//  Internal variables
//

struct htmlset {
   int obj;
   char *index;
   int  istime;
   };
typedef struct htmlset Htmlset;

Array *htmlsets=0;

static char *HTML_css = "\
table.dec {}\n\
td { padding-left: 1em; padding-right: 1em; }\n\
th { text-align: left; padding-left: 1em; padding-right: 1em; }\n\
div.heading { margin-top: 2em; font-weight: bold; font-size: 120%; }\n\
div.dblock { margin-top: 1em; margin-left: 2em; }\n\
div.eblock { margin-top: 1em; }\n\
div.eqn { margin-top: 1em; margin-left: 2em;}\n";


//----------------------------------------------------------------------//
//  html_slprint
//
//  Generate a comma-separated string from a list.  Almost identical 
//  to slprint but with a space after each comma.  One difference is 
//  that this returns an HTML nbsp if the list is empty.
//----------------------------------------------------------------------//
char *html_slprint(list)
List *list;
{
   char *obuf;
   Item *cur;
   int n,size;
  
   if( list->obj != LISTOBJ )
      fatal_error("%s","argument to html_slprint is not a list");

   if( list->n == 0 )
      return xstrdup("&nbsp;");

   size = 0;

   for( n=0, cur=list->first ; cur ; cur=cur->next )
      {
      if( size )size += 2;
      size += strlen(cur->str);
      }

   obuf = (char *) xmalloc( size+1 );

   n=0;
   for( n=0, cur=list->first ; cur ; cur=cur->next )
      if( 0 == n++ )
         strcpy(obuf,cur->str);
      else
         {
         strcat(obuf,", ");
         strcat(obuf,cur->str);
         }

   return obuf;
}


//----------------------------------------------------------------------//
//  htmlset
//
//  Add a set to the internal list.
//----------------------------------------------------------------------//
static void htmlset(char *name)
{
   static struct htmlset *new;
   char abb[2];
   
   abb[0] = *name;
   abb[1] = '\0';
   
   new = (Htmlset *) malloc( sizeof(Htmlset) );
   new->obj    = MYOBJ;
   new->index  = strdup(abb);
   new->istime = 0;
   
   if( intertemporal )
      if( isequal(name,"time") || issubset(name,"time") )
         new->istime = 1;
   
   if( htmlsets==0 )htmlsets = newarray();
   addvalue(htmlsets,name,new);
}


//----------------------------------------------------------------------//
//  htmlvar
//
//  Generate a variable reference from a name and a list of sets.
//----------------------------------------------------------------------//
static char *htmlvar(char *name, List *sets, int dt)
{
   Item *s;
   List *indexes;
   Htmlset *cur;
   char *result;
   char tbuf[10];
   
   validate( htmlsets, ARRAYOBJ, "htmlvar" );
   if( name==0 )FAULT("null pointer passed to htmlvar");

   if( sets==0 )return strdup(name);
   validate( sets, LISTOBJ, "htmlvar" );

   if( sets->n == 0 )return concat(3,"<i>",name,"</i>");

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
      cur = (Htmlset *) getvalue(htmlsets,s->str);
      if( cur->istime && dt )
         {
         sprintf(tbuf,"%s%+d",cur->index,dt);
         addlist(indexes,tbuf);
         }
      else
         addlist(indexes,cur->index);
      }
      
   result = concat(5,"<i>",name,"</i>(<i>",slprint(indexes),"</i>)");
   freelist(indexes);
   return result;
}


//----------------------------------------------------------------------//
//  htmlqualifier
//
//  Generate an html qualifier from a list of sets.
//----------------------------------------------------------------------//
static char *htmlqualifier(List *sets)
{
   Item *s;
   Htmlset *cur;
   char *result,*new;
   char *sep;

   validate( sets, LISTOBJ, "htmlqualifier" );

   sep = "";
   result = strdup("");
   for( s=sets->first ; s ; s=s->next )
      {
      if( isimplicit(s->str) )continue;
      cur = (Htmlset *) getvalue(htmlsets,s->str);
      new = concat(7,result,sep,"<i>",cur->index,"</i> in <b>",s->str,"</b>");
      free(result);
      result = new;
      sep = ", ";
      }

   return result;
}


//----------------------------------------------------------------------//
//  Begin processing the file
//----------------------------------------------------------------------//

void HTML_begin_file(char *basename)
{
   fprintf(code,"<html>\n<head>\n");
   fprintf(code,"<title>%s</title>\n",basename);
   fprintf(code,"<style type=\"text/css\">\n%s</style>\n",HTML_css);
   fprintf(code,"</head>\n<body>\n");
   fprintf(code,"<h1>%s</h1>\n",basename);
}


//----------------------------------------------------------------------//
//  End processing the file
//----------------------------------------------------------------------//

void HTML_end_file()
{
   fprintf(code,"</div>\n</body>\n</html>\n");
}


//----------------------------------------------------------------------//
//  HTML_declare
//----------------------------------------------------------------------//

void HTML_declare(void *sym)
{
   char *name;
   if( istype(sym,set) )
      {
      name = symname(sym);
      htmlset(name);
      free(name);
      }
}


//----------------------------------------------------------------------//
//  HTML_writedecs
//
//  Write out the declaration part of the file.
//----------------------------------------------------------------------//

static void HTML_writedecs()
{
   Element *cur;
   Htmlset *cts;
   char buf[10],*name,*vstr,*desc,*what;
   int  n;
   List *val;
   List *sofar;
   char *nbsp="&nbsp;";

   //
   //  make unique set index variables
   //

   if( htmlsets )
      {
      sofar = newlist();
      for( cur=htmlsets->first ; cur ; cur=cur->next )
         {
         n=1;
         cts = (Htmlset *) cur->value;
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
   
   what = "Sets";
   fprintf(code,"<div class=\"heading\">%s:</div>\n",what);

   for( cur=firstsymbol(set), n=0 ; cur ; cur=nextsymbol(cur), n++ )
      {
      name = symname(cur);
      desc = symdescrip(cur);
      val  = symvalue(cur);
      
      if( n%30 == 0 ) 
         {
         if( n>0 )
            {
            fprintf(code,"</table>\n</div>\n");
            fprintf(code,"<div class=\"heading\">%s, continued:</div>\n",what);
            }
         fprintf(code,"<div class=\"dblock\">\n");
         fprintf(code,"<table class=\"dec\" border=1 cellspacing=0>\n");
         fprintf(code,"<tr><th>Name<th>Elements<th>Description</tr>\n");
         }

      if( strlen(desc)==0 )desc = nbsp;
      vstr = html_slprint(val) ;
      fprintf(code,"<tr><td><b>%s</b><td>%s<td>%s</tr>\n",name,vstr,desc);

      xfree(vstr);
      freelist(val);
      free(name);
      }

   fprintf(code,"</table>\n</div>\n");
   
   //
   //  write out variable statements
   // 
    
   what = "Variables";
   fprintf(code,"<div class=\"heading\">%s:</div>\n",what);

   for( cur=firstsymbol(var), n=0 ; cur ; cur=nextsymbol(cur), n++ )
      {
      name = symname(cur);
      desc = symdescrip(cur);
      val  = symvalue(cur);
      
      if( n%30 == 0 ) 
         {
         if( n>0 )
            {
            fprintf(code,"</table>\n</div>\n");
            fprintf(code,"<div class=\"heading\">%s, continued:</div>\n",what);
            }
         fprintf(code,"<div class=\"dblock\">\n");
         fprintf(code,"<table class=\"dec\" border=1 cellspacing=0>\n");
         fprintf(code,"<tr><th>Name<th>Domain<th>Description</tr>\n");
         }

      if( strlen(desc)==0 )desc = nbsp;
      vstr = html_slprint(val) ;
      fprintf(code,"<tr><td>%s<td><b>%s</b><td>%s</tr>\n",name,vstr,desc);
      
      xfree(vstr);
      free(name);
      freelist(val);
      }

   fprintf(code,"</table>\n</div>\n");
   
   //
   //  write coefficient declarations
   //
   
   what = "Parameters";
   fprintf(code,"<div class=\"heading\">%s:</div>\n",what);

   for( cur=firstsymbol(par), n=0 ; cur ; cur=nextsymbol(cur), n++ )
      {
      name = symname(cur);
      desc = symdescrip(cur);
      val  = symvalue(cur);
      
      if( n%30 == 0 ) 
         {
         if( n>0 )
            {
            fprintf(code,"</table>\n</div>\n");
            fprintf(code,"<div class=\"heading\">%s, continued:</div>\n",what);
            }
         fprintf(code,"<div class=\"dblock\">\n");
         fprintf(code,"<table class=\"dec\" border=1 cellspacing=0>\n");
         fprintf(code,"<tr><th>Name<th>Domain<th>Description</tr>\n");
         }

      if( strlen(desc)==0 )desc = nbsp;
      vstr = html_slprint(val) ;
      fprintf(code,"<tr><td>%s<td><b>%s</b><td>%s</tr>\n",name,vstr,desc);
      
      xfree(vstr);
      freelist(val);
      free(name);
      }

   fprintf(code,"</table>\n</div>\n");

   fprintf(code,"<div class=\"heading\">Equations:</div>\n");
   fprintf(code,"<div class=\"dblock\">\n");
}


//----------------------------------------------------------------------//
//  HTML_begin_block
//----------------------------------------------------------------------//

void HTML_begin_block(void *eq)
{
   char *qual;
   List *eqnsets();
   int nblk,nstart,nscalar,nend;
   char *label;

   if( HTML_block == 1 )
      HTML_writedecs();

   nblk    = HTML_block++;
   nstart  = HTML_scalar;
   nscalar = eqncount(eq);
   nend    = nstart+nscalar-1;
   
   HTML_scalar = nend+1;

   label = eqnlabel(eq);
   if( label )
      fprintf(code,"<div class=\"eblock\">\nEquation %d: %s<br>\n",nblk,label);
   else
      fprintf(code,"<div class=\"eblock\">\nEquation %d<br>\n",nblk);

   switch( nscalar )
      {
      case 0:
         fprintf(code,"Contains undeclared symbols<br>\n");
         break;

      case 1:
         break;

      default:
         qual = htmlqualifier(eqnsets(eq));
         fprintf(code,"For %s (%d total):<br>\n",qual,nscalar);
         break;
      }

   fprintf(code,"<div class=\"eqn\">");
}


//----------------------------------------------------------------------//
//  HTML_end_eqn
//----------------------------------------------------------------------//

void HTML_end_eqn(void *eq)
{
   fprintf(code,"</div>\n</div>\n");
}


//----------------------------------------------------------------------//
//  HTML_begin_func
//----------------------------------------------------------------------//

char *HTML_begin_func(char *func, char *arg)
{
   Htmlset *cur;
   
   if( isequal(func,"sum") || isequal(func,"prod") )
      {
      cur = (Htmlset *) getvalue(htmlsets,arg);
      if( cur==0 )FAULT("null pointer in HTML_begin_func");
      return concat(6,func,"(<i>",cur->index,"</i> in <b>",arg,"</b>, ");
      }
      
   if( arg )
      FAULT("unexpected function call in HTML_begin_func");

   if( isequal(func,"log") )
      return strdup("loge(");
      
   return concat(2,func,"("); 
}

//----------------------------------------------------------------------//
//  HTML_show_symbol
//----------------------------------------------------------------------//

char *HTML_show_symbol(char *str, List *setlist, Context context)
{
   char *curstr,*tmpstr;
   int  delta;
   
   curstr = htmlvar(str,setlist,context.dt);
   delta  = context.dt ;

   while( delta != 0 )
      {
      if( delta < 0 ){
         tmpstr = concat(3,"lag(" ,curstr,")");
         delta++;
      } else {
         tmpstr = concat(3,"lead(",curstr,")");
         delta--;
      }
      free(curstr);
      curstr = tmpstr;
      }
      
   return curstr;
}

//----------------------------------------------------------------------//
//  Connect up the public routines.
//----------------------------------------------------------------------//

void Html_setup(void)
{
   lang_begin_file ( HTML_begin_file  ); 
   lang_end_file   ( HTML_end_file    );
   lang_declare    ( HTML_declare     );
   lang_begin_block( HTML_begin_block ); 
   lang_end_eqn    ( HTML_end_eqn     ); 
   lang_begin_func ( HTML_begin_func  );
   lang_show_symbol( HTML_show_symbol );

   set_eqn_vector();
   set_sum_vector();
}

char *Html_version = "$Revision: 57 $";

