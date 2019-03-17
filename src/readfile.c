/*--------------------------------------------------------------------*
 *  readfile.c
 *  Mar 04 (PJW)
 *
 *  Read input files (potentially recursively) and call the parser.
 *--------------------------------------------------------------------*/

#include "error.h"
#include "lexical.h"
#include "output.h"
#include "str.h"
#include "sym.h"
#include "xmalloc.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define  myDEBUG 0

#define MAXLINE 10000
#define MAXSTMT 10000

static int get_stmt();
int yyparse();

//
//  Static buffer for grouping lines into statements
//

static char sbuf[MAXSTMT+1];

//
//  List of file names to catch circular references
//

List *file_list;

//
//  Structure for holding linked lines of source code
//

#define SRCOBJ 9193

typedef struct source_line 
   {
   int obj;
   char *file;
   int num;
   char *line;
   struct source_line *next;
   }
   SourceLine ;

SourceLine  head ;
SourceLine *tail ;
SourceLine *curr ;
SourceLine *prev ;

//
//  new_SourceLine
//
//     Constructor for SourceLine
//

static void new_SourceLine(char *file, int num, char *buf)
{
   SourceLine *new;

   new = (SourceLine *) xmalloc( sizeof(SourceLine) );
   new->obj = SRCOBJ;
   new->file = strdup(file);
   new->num  = num;
   new->line = strdup(buf);
   new->next = 0;

   if( tail->obj != SRCOBJ )
      FAULT("List tail is corrupt");

   tail->next = new;
   tail = new;
}


//
//  load_file
//
//     Read lines from a file, potentially recursively
//

static void load_file(char *filename)
{
   FILE *src;
   char *ibuf;
   int  linenum;
   char *c,*e;

   if( ismember(filename,file_list) )
      fatal_error("Circular #include reference to %s\n",filename);
   addlist(file_list,filename);

   src = fopen(filename,"r");
   if( src==0 )
      fatal_error("Could not open input file %s\n",filename);
   if( info )
      fprintf(info,"   Source file: %s\n", filename);

   ibuf = (char *) xmalloc( MAXLINE+1 );

   for( linenum=1 ; feof(src)==0 && fgets(ibuf,MAXLINE+1,src) ; linenum++ )
      {
      if( DBG )printf("read %s line %d\n",filename,linenum);
      
      c = strstr(ibuf,"//");
      if( c )*c = 0;
      if( strlen(ibuf) == MAXLINE )
         {
         printf( "File %s line %d exceeds max length of %d\n",filename,linenum,MAXLINE );
         exit(0);
         }
      
      if( strncasecmp(ibuf,"#include",8)==0 )
         {

         // 
         //  find the file name, ignoring leading and trailing spaces and 
         //  optionally allowing either single or double quotes.
         //

         for( c=ibuf ; *c && isspace(*c)==0 ; c++ );
         for(        ; *c && isspace(*c)    ; c++ );

         switch( *c ) {
            case '\'':
               c++;
               e = strchr(c,'\'');
               if( e==0 )fatal_error("Unclosed quote in line: %s",ibuf);
               *e = 0;
               break;

            case '"':
               c++;
               e = strchr(c,'"');
               if( e==0 )fatal_error("Unclosed quote in line: %s",ibuf);
               *e = 0;
               break;

            default:
               e = strpbrk(c," \t\n");
               if( e )*e = 0;
               break;
            }

         if( strlen(c)==0 )
            fatal_error("Missing file name in #include statment in %s\n",filename);

         //
         //  call ourself to process the included file
         //

         load_file(c);
         continue;
         }
      
      if( strncasecmp(ibuf,"#",1)==0 ) 
         fatal_error("Unexpected # at start of line: %s",ibuf);

      new_SourceLine(filename,linenum,ibuf);
      }

   fclose(src);
   xfree(ibuf);
}


/*--------------------------------------------------------------------*
 *  READ_FILES
 *
 *  Read facts from the concatenated files loaded by load_file
 *--------------------------------------------------------------------*/
static void read_files()
{
   char buf[MAXSTMT+1];
   int  ngood=0,fatal=0,netopen;
   char *tokn,*getlasttoken();
   char *lbuf,*getlexbuf();
   char *c;

   sbuf[0] = 0;

   curr = head.next ;

   while( get_stmt(buf,MAXSTMT) )
      {
      ngood++;
      load_lex_buf(buf);
      if( DBG )
         {
         printf( "parsing statement %d (%s,%d):\n",ngood,prev->file,prev->num );
         printf( "%s\n\n",buf );
         }
      if( yyparse() )
         {
         tokn = getlasttoken();
         lbuf = getlexbuf();

         printf("\nA statement in file %s at line %d is invalid:\n\n",prev->file,prev->num);

         if( lbuf )printf("%s\n",lbuf);
         if( tokn )
            printf("\nThe error occurs near:\n   %s\n",tokn);

         // check for unbalanced parentheses in the whole statement

         netopen = 0;
         for( c=buf ; *c ; c++ )
            {
            netopen += (*c == '(');
            netopen -= (*c == ')');
            }
         if( netopen > 0 )
            printf("\nUnbalanced parentheses: %d open without close.\n",netopen);
         if( netopen < 0 )
            printf("\nUnbalanced parentheses: %d close without open.\n",-netopen);

         fatal++;
         }
      }

   if( fatal )exit(0);
}


/*--------------------------------------------------------------------*
 *  GET_STMT
 *
 *  Get a statement using buffering
 *--------------------------------------------------------------------*/
static int get_stmt(buf,n)
char *buf;
int n;
{
   char ibuf[1024];
   char *semi,*c;
   int  len;
   
   len  = strlen( sbuf ); 
   semi = strchr( sbuf, ';' ) ;
   while( semi == NULL )
      {
      if( curr==0 && len == 0 )
         {
         *buf = 0;
         return 0;
         }
      if( curr==0 )
         {
         for( c=sbuf ; isspace(*c) ; c++ );
         strcpy(buf,c);
         sbuf[0] = 0;
         if( strlen(buf) == 0 )return 0;
         strcat(buf," ;");
         return 1;
         }
      *ibuf = '\0';
      strcpy(ibuf,curr->line);
      if( DBG )printf("read %s line %d\n",curr->file,curr->num);
      prev = curr;
      curr = curr->next;

      c = strstr(ibuf,"//");
      if( c )*c = 0;
      if( len+strlen(ibuf) > MAXSTMT )
         {
         printf( "Statement too long\n" );
         exit(0);
         }
      strcat( sbuf, ibuf );
      len  = strlen( sbuf );
      semi = strchr( sbuf,';') ;
      }

   *semi++ = 0;
   for( c=sbuf ; isspace(*c) ; c++ );
   strncpy( buf, c, n-2 );
   buf[n-2]=0;
   strcat(buf,";");
   for( c=sbuf ; *semi ; c++, semi++ )
      *c = *semi;
   *c = 0;
   return 1;
}


/*--------------------------------------------------------------------*
 *  READ_SOURCE
 *
 *  Public entry point.  Reads facts from the main source file and 
 *  any included files, then parses the result.  Calls load_file 
 *  recursively.
 *--------------------------------------------------------------------*/
void read_source(char *sourcefile)
{
   head.obj  = SRCOBJ;
   head.next = 0;
   tail = &head;

   file_list = newlist();

   load_file(sourcefile);

   curr = head.next;
   if( curr==0 )
      fatal_error( "No input statements found in %s\n", sourcefile );
   if( curr->obj != SRCOBJ )
      FAULT("Statement list is corrupt");

   if( mergeonly ) {
      for( ; curr ; curr=curr->next )fprintf(code,"%s",curr->line);
      return;
      }

   fprintf(info,"\n");

   read_files();
}

