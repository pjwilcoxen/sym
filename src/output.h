/*--------------------------------------------------------------------*
 *  output.h
 *
 *  $Id: output.h 49 2018-06-16 17:15:39Z wilcoxen $
 *--------------------------------------------------------------------*/

#ifndef OUTPUT_H
#define OUTPUT_H

#include "lists.h"
#include <stdio.h>

//
//  File pointers always opened by the main program
//  

extern FILE *code;  // program code
extern FILE *info;  // information

//
//  Context in which a symbol appears
//

typedef struct
   {
   int lhs;
   int dt;
   char *tsub;
   }
   Context;

// 
//  generate subscripts for variables
//

List* sub_offset(char*,List*,int);
List* sub_tuple(char*,List*);

void wrap_write(char*,int,int);
void write_file(char*);

#endif /* OUTPUT_H */
