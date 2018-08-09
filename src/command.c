/*--------------------------------------------------------------------*
 * COMMAND.C 2.10
 * 15 Sep 89 (PJW)
 * Parse a command line, portably!
 *
 * $Id: command.c 49 2018-06-16 17:15:39Z wilcoxen $
 *--------------------------------------------------------------------*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLEN   256
#define MAXATOMS 200

/*--------------------------------------------------------------------*
 *  Operating system dependencies.  If SLASH_OK is defined then
 *  VMS/DOS option notation may be used.  In particular, qualifiers
 *  may begin with / and may touch.
 *--------------------------------------------------------------------*/

#ifndef DOS
#define SLASH_OK
#define index(a,b) strchr(a,b)
#else
#define SLASH_OK
extern char *index();
#endif

static char *atoms[MAXATOMS];
static char *opt_list[MAXATOMS];
static char *val_list[MAXATOMS];
static char *arg_list[MAXATOMS];
static char cmd_name[MAXLEN+1];
static int n_opt,n_arg;

static int atomize(argc,argv)
int argc;
char *argv[];
{

#ifdef SLASH_OK

   int i,n;
   char *s,*o,*c;

   n = 0;

   if( argc > MAXATOMS ) 
      {
      fprintf(stderr,"Too may items on the command line: ");
      fprintf(stderr,"found %d but limit is %d\n", argc, MAXATOMS );
      exit(0);
      }

/* Turn slashes into dashes, allowing slash qualifiers to touch  */

/* Aug 06: only do this is the first character is a slash to avoid */
/*         clobbering filenames that begin with subdirectories     */

/* May 07: for arguments that don't begin with a slash, don't break */
/*         on whitespace -- may be embedded spaces in paths         */

   for( i=1 ; i<argc ; i++ )
      {
      s = argv[i];
      if( *s != '/' )
         {
         atoms[n++] = s;
         continue;
         }

      while( s )
         {
         *s = '-';
         o = strdup(s);
         c = index(o,'/');
         if( c )*c = '\0';
         atoms[n++] = o;
         s = index(s,'/');
         }
      }

   return( n );

#else

   int i,n;
   n = 0;
   for( i=1 ; i<argc ; i++ )
      atoms[n++] = argv[i];
   return( n );

#endif

}


void parse_command(argc,argv)
int argc;
char *argv[];
{
   int i,natom;
   char temp[MAXLEN+1],*equal;
   cmd_name[MAXLEN] = temp[MAXLEN] = '\0';

   strncpy(cmd_name,argv[0],MAXLEN);

   natom = atomize(argc,argv);
   for( i=0 ; i<natom  ; i++ )
      if( *atoms[i] != '-' )
         arg_list[n_arg++] = atoms[i];
      else
         {
         opt_list[n_opt] = atoms[i]+1;
         equal = (char *) index( atoms[i], '=' );
         if( equal == NULL )
            val_list[n_opt] = NULL;
         else
            {
            *equal = '\0';
            val_list[n_opt] = equal+1;
            }
         n_opt++;
         }
}

char *command()
{   return( cmd_name );   }

char *option(n)
int n;
{   return( n<n_opt ? opt_list[n] : NULL );   }

char *argument(n)
int n;
{   return( n<n_arg ? arg_list[n] : NULL );   }

char *argprompt(n,str)
int n;
char *str;
{
   static char buf[80],*c;
   if( n<n_arg )return( arg_list[n] );
   printf("%s: ",str);
   fgets(buf,80,stdin);
   c = strpbrk(buf,"\n\r");
   if( c )*c=0;
   c = (char *) strdup( buf );
   return( c );
}

char *opvalue(n)
int n;
{   return( n<n_opt ? val_list[n] : NULL );   }

int isoption(str,min)
char *str;
int min;
{
   int i,j;
   for( i=0 ; i < n_opt ; i++ )
      {
      for( j=0 ; opt_list[i][j] != 0 && str[j] != 0 ; j++ )
         if( tolower(opt_list[i][j]) != tolower(str[j]) ) break;
      if( (opt_list[i][j]==0 || str[j]==0) && j >= min )return(i+1);
      }
   return(0);
}

int ishelp()
{
   if( isoption("help",1) )return(1);
   if( isoption("?"   ,1) )return(1);
   return(0);
}


void check_help(usage)
char *usage;
{
   if( ishelp() )
      {
      fprintf(stderr,"Usage: %s\n",usage);
      exit(0);
      }
}


void check_version(vstring)
char *vstring;
{
   char *v,*get_version();

   if( isoption("version",1) )
      {
      v = get_version(vstring);
      fprintf(stderr,"Version %s\n",v);
      exit(0);
      }
}


void check_docver(name,vers,use,doc) 
char *name,*vers,*use,*doc;
{
   if( isoption("version",1) ) {
      printf("Version %s\n",vers);
      exit(0);
   }

   if( isoption("doc",1) ) {
      printf("\n   %s\n",name);
      printf("%s",doc);
      printf("      Build version %s\n",vers);
      exit(0);
   }

   if( isoption("help",1) || isoption("?",1) ) {
      printf("%s",use);
      exit(0);
   }
}


char *get_version(vstring)
char *vstring;
{
   /* understands the following:           */
   /*    "$Id: command.c 49 2018-06-16 17:15:39Z wilcoxen $"                      */
   /*    "$Revision: 49 $"                */
   /*    "$Date: 2018-06-16 13:15:39 -0400 (Sat, 16 Jun 2018) $"                    */
   /*    "$Revision: 49 $ $Date: 2018-06-16 13:15:39 -0400 (Sat, 16 Jun 2018) $"   */
   /*    "string"                          */

   static char outbuf[80];
   char *ws=" \n\t";
   static char tmpbuf[80];
   char *word,*junk,*file,*vers,*date;

   if( vstring==NULL || *vstring=='\0' )return( "" );

   strcpy(tmpbuf,vstring);
   word = strtok(tmpbuf,ws);
   if( word==NULL )return( "" );

   if( strcmp(word,"$Id:")==0 )
      {
      if( (file=strtok(NULL,ws)) == NULL )return("");
      if( (vers=strtok(NULL,ws)) == NULL )return("");
      if( (date=strtok(NULL,ws)) == NULL )return(vers);
      sprintf(outbuf,"%s (%s)",vers,date);
      return( outbuf );
      }

   if( strcmp(word,"$Revision:")==0 )
      {
      if( (vers=strtok(NULL,ws)) == NULL )return("");
      if( (junk=strtok(NULL,ws)) == NULL )return(vers);
      if( (word=strtok(NULL,ws)) == NULL )return(vers);
      if(    strcmp(word,"$Date:")!=0    )return(vers);
      if( (date=strtok(NULL,ws)) == NULL )return(vers);
      sprintf(outbuf,"%s (%s)",vers,date);
      return( outbuf );
      }

   if( strcmp(word,"$Date:")==0 )
      {
      if( (date=strtok(NULL,ws)) == NULL )return("");
      return(date);
      }

   return( vstring );
}


void parse_std_command(argc,argv,usage,vstring)
int argc;
char **argv,*usage,*vstring;
{
   char *v,*get_version();

   parse_command(argc,argv);

   v = get_version(vstring);

   if( ishelp() )
      {
      fprintf(stderr,"Usage: %s\n",usage);
      fprintf(stderr,"Version: %s\n",v);
      exit(0);
      }

   if( isoption("version",4) )
      {
      v = get_version(vstring);
      fprintf(stderr,"Version: %s\n",v);
      exit(0);
      }
}


