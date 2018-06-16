//  str.c
//  Dec 04 (PJW)
//
//  Convenience routines for dealing with strings in a consistent 
//  way.
//  
//  $Id: str.c 57 2018-06-16 19:50:13Z wilcoxen $

#include "str.h"

#include "error.h"
#include "sym.h"
#include "xmalloc.h"
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//
//  concat()
//  
//  Concatenates a list of strings and returns a new string obtained by 
//  malloc.  Derived from an example by Plauger and Brodie.
//

char *concat(int n, char *s, ...)
{
    char *new;
    va_list ap;
    int i,len;
    
    // measure the strings...
    
    va_start(ap, s);
    
    len = strlen(s);
    for( i=1 ; i<n ; i++ )
        len += strlen( va_arg(ap, char*) );
    
    va_end(ap);
    
    // get some space...
    
    new  = (char *) malloc( len+1 );
    
    // build the new string...

    va_start(ap, s);

    strcpy(new,s);
    for( i=1 ; i<n ; i++ )
        strcat(new, va_arg(ap, char*) );

    va_end(ap);

    return new;
}

//  
//  strlower()
//
//  Clone a string and convert it lower case.  
//

char *strlower( char *oldstr ) 
{
   char *newstr,*c;
   validate( oldstr, 0, "strlower" );
   newstr = xstrdup(oldstr);
   for( c=newstr ; *c ; c++ )*c = tolower(*c);
   return newstr;
}
