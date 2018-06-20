/*-------------------------------------------------------------------*
 *  error.c
 *  Dec 04 (PJW)
 *
 *  Handle errors
 *-------------------------------------------------------------------*/

#include "error.h"

#include "output.h"
#include "sym.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

static int e_front=0;
static int e_back=0;

//
//  null structure used by validate
//

struct nullobj
   {
   int obj;
   };


/*-------------------------------------------------------------------*
 *  error_more
 *-------------------------------------------------------------------*/
void error_more(char *format, ...)
{
   va_list arglist;
  
   printf( "      " );
   va_start( arglist, format );
   vprintf( format, arglist );
   va_end( arglist );
   printf( "\n" );
   
   fprintf( info, "      " );
   va_start( arglist, format );
   vfprintf( info, format, arglist );
   va_end( arglist );
   fprintf( info, "\n" );
}


/*-------------------------------------------------------------------*
 *  error_front
 *-------------------------------------------------------------------*/
void error_front(char *format, ...)
{
   char *who;
   va_list arglist;
  
   who = "Input File Error";
   
   printf( "\n%s:\n   ", who );
   va_start( arglist, format );
   vprintf( format, arglist );
   va_end( arglist );
   printf( "\n" );
   
   fprintf( info, "\n%s:\n   ", who );
   va_start( arglist, format );
   vfprintf( info, format, arglist );
   va_end( arglist );
   fprintf( info, "\n" );

   e_front++;
}


/*-------------------------------------------------------------------*
 *  error_back
 *-------------------------------------------------------------------*/
void error_back(char *format, ...)
{
   char *who;
   va_list arglist;

   who = "Target Language Error";
   
   printf( "\n%s:\n   ", who );
   va_start( arglist, format );
   vprintf( format, arglist );
   va_end( arglist );
   printf( "\n" );
   
   fprintf( info, "\n%s:\n   ", who );
   va_start( arglist, format );
   vfprintf( info, format, arglist );
   va_end( arglist );
   fprintf( info, "\n" );

   e_back++;
}


/*-------------------------------------------------------------------*
 *  error_count
 *-------------------------------------------------------------------*/
int error_count()
{
   return e_front+e_back;
}


/*-------------------------------------------------------------------*
 *  fatal_error
 *-------------------------------------------------------------------*/
void fatal_error(char *fmt, char *str)
{
   show_error("Fatal Error",fmt,str);
}


/*-------------------------------------------------------------------*
 *  show_error
 *-------------------------------------------------------------------*/
void show_error(char *who, char *fmt, char *str)
{
   printf("\n%s:\n   ",who);
   printf(fmt,str);
   printf("\n");
   exit(0);
}


/*-------------------------------------------------------------------*
 *  validate
 *  Mar 04 (PJW)
 *
 *  Check that a given object has the expected type.
 *-------------------------------------------------------------------*/
void validate( void *ob, int wantedtype, char *where )
{
   int havetype;
   char *msg;
   char *cob;

   if( ob == 0 )
      fatal_error("Validate found a null pointer in %s",where);

   if( wantedtype == 0 )
      {
      cob = (char *) ob;
      if( *cob == 0 )fatal_error( "Validate found a blank string in %s",where);
      return;
      }

   havetype = ((struct nullobj *)ob)->obj;

   if( havetype == wantedtype )
      return;

   switch( wantedtype )
      {
      case NODEOBJ: 
         msg = "Object in %s is not a node";
         break;
         
      case LISTOBJ:
         msg = "Object in %s is not a list";
         break;
         
      case ITEMOBJ:
         msg = "Object in %s is not a list item";
         break;
         
      case SYMBOBJ:
         msg = "Object in %s is not a symbol";
         break;
         
      default:
         msg = "Object in %s is not the correct type";
         break;
      }

   fatal_error(msg,where);
}

