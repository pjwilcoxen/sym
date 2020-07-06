#ifndef SYMTABLE_H
#define SYMTABLE_H

#include "lists.h"

//  
//  Types of symbol table entry
//  

typedef enum 
   {
   und, set, par, var
   } 
   Symboltype;

//
//  Function prototypes
//

List* symattrib(void *);
List* symvalue(void *);
char* symall(void *);
char* symdef(void *);
char* symdescrip(void *);
char* symname(void *);
char* symuse(void *);
int   isattrib(void*,char*);
int   isident(void*);
int   islhs(void*);
int   isrhs(void*);
int   istype(void*,Symboltype);
int   isused(void*);
int   symsize(void*);
void  check_identifiers();
void  symdeclare(Symboltype, char*, List*, char*, List *);
void  validatetype(void*,Symboltype,char*);
void* firstsymbol(Symboltype);
void* lookup(char *);
void* nextsymbol(void*);

#endif /* SYMTABLE_H */
