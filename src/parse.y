%{
/*--------------------------------------------------------------------*
 *  PARSE.Y 4.00
 *  9 Apr 90 (PJW)
 *
 *  Parser specification for symbolic math program.  This file must
 *  be processed by yacc or bison.
 *--------------------------------------------------------------------*/

#include <stdio.h>
#include "str.h"
#include "sym.h"
#include "nodes.h"
#include "declare.h"
#include "eqns.h"

#define YYSTYPE Node*

int yylex();
void yyerror(char*);

%}

%token NAME NUM NEG LOG EXP SUM PROD STRING BADNUM
%token SET VAR PAR EQU
%token LEAD LAG NEXT
%token TIME FIRST LAST

%left    ','
%right   '='
%left    '+' '-'
%left    '*' '/'
%left    '^'
%left    NEG

%start file

%%

/*
.. # Sym Input File Syntax
..
.. Input files consist of: **set**, **parameter** and **variable** 
.. declarations, and **equations**. All statements must end with a 
.. semicolon. Anything from a // to the end of the line is taken to 
.. be a comment and is ignored. Items in square brackets are
.. optional and may be omitted. Attributes are target-language 
.. dependent: there are no attributes applicable to all languages.
..
*/

file     : stmt
         | file ';' stmt
         ;

stmt     : /* empty */
         | declare
         | eqn 
         ;
/*
.. ## Set Declarations:
..
.. Set elements do not need to be unique as long as sets using the
.. same element names cannot be mistaken for subsets of each other.
.. For example, elements a and b will distinct in set1(a,b,c) and 
.. set2(a,b,d). However, using an additional set3(a,b) should be
.. avoided and will be flagged as an error in subsequent versions
.. of sym.
..
..     SET name (elements)              ['description'] ;
..     SET name = prevset               ['description'] ;
..     SET name = prevset + (elements)  ['description'] ;
..     SET name = prevset - (elements)  ['description'] ;
..
.. ## Variable and Parameter Declarations:
.. 
..     PARAMETER name [(sets)]   ['description'] [attributes] ;
..     VARIABLE  name [(sets)]   ['description'] [attributes] ;
.. 
*/

declare  : decl  NAME                          { declare($1,$2, 0, 0, 0); }
         | decl  NAME              STRING      { declare($1,$2, 0,$3, 0); }
         | decl  NAME              attr        { declare($1,$2, 0, 0,$3); }
         | decl  NAME              STRING attr { declare($1,$2, 0,$3,$4); }
         | decl  NAME              list STRING { declare($1,$2, 0,$4,$3); }
         | decl  NAME '(' list ')'             { declare($1,$2,$4, 0, 0); }
         | decl  NAME '(' list ')' STRING      { declare($1,$2,$4,$6, 0); }
         | decl  NAME '(' list ')' attr        { declare($1,$2,$4, 0,$6); }
         | decl  NAME '(' list ')' STRING attr { declare($1,$2,$4,$6,$7); }
         | decl  NAME '(' list ')' list STRING { declare($1,$2,$4,$7,$6); }
         | decset
         ;

decl     : PAR
         | VAR
         ;


decset   : SET tiok '(' list ')'                      { declare($1,$2,$4, 0, 0) ; }
         | SET tiok '(' list ')' STRING               { declare($1,$2,$4,$6, 0) ; }
         | SET NAME '=' NAME                          { decset ($2,$4,nul, 0, 0); }
         | SET NAME '=' NAME STRING                   { decset ($2,$4,nul, 0,$5); }
         | SET NAME '=' tiok     '(' list ')'         { decset ($2,$4,equ,$6, 0); }
         | SET NAME '=' tiok     '(' list ')' STRING  { decset ($2,$4,equ,$6,$8); }
         | SET forl '=' TIME     '(' item ')'         { decset ($2,$4,equ,$6, 0); }
         | SET forl '=' TIME     '(' item ')' STRING  { decset ($2,$4,equ,$6,$8); }
         | SET NAME '=' NAME '+' '(' list ')'         { decset ($2,$4,add,$7, 0); }
         | SET NAME '=' NAME '+' '(' list ')' STRING  { decset ($2,$4,add,$7,$9); }
         | SET NAME '=' NAME '-' '(' list ')'         { decset ($2,$4,sub,$7, 0); }
         | SET NAME '=' NAME '-' '(' list ')' STRING  { decset ($2,$4,sub,$7,$9); }
         ;

tiok     : NAME
         | TIME
         ;

forl     : FIRST
         | LAST
         ;
         
/*
.. ## Equations:
..
..     [EQUATION name|/name/] [sets:] expr = expr [attributes];
..
*/

eqn      :        expr '=' expr               { neweqn( 0,$1,$3, 0, 0); }
         |        expr '=' expr          attr { neweqn( 0,$1,$3,$4, 0); }
         |        qual expr '=' expr          { neweqn($1,$2,$4, 0, 0); }
         |        qual expr '=' expr attr     { neweqn($1,$2,$4,$5, 0); }
         | eqname expr '=' expr               { neweqn( 0,$2,$4, 0,$1); }
         | eqname expr '=' expr          attr { neweqn( 0,$2,$4,$5,$1); }
         | eqname qual expr '=' expr          { neweqn($2,$3,$5, 0,$1); }
         | eqname qual expr '=' expr attr     { neweqn($2,$3,$5,$6,$1); }
         ;

eqname   : '/' NAME '/'          { $$ = $2; }
         | EQU NAME              { $$ = $2; }
         ;

qual     : list ':'              { $$ = $1; }
         ;

attr     : list                  { $$ = $1; }
         | '{' list '}'          { $$ = $2; }
         | '[' list ']'          { $$ = $2; }
         ;

list     : item              { $$ = $1; }
         | item ',' list     { $$ = $1; $1->r=$3; }  
         ;

item     : NAME
         | NUM
         | FIRST
         | LAST
         | TIME
         ;

/*
.. ## Expressions:
..
..     expr + expr
..     expr - expr
..     expr * expr
..     expr / expr
..     expr ^ expr
..     - expr
..     ( expr )
..     function
..     lead(variable)
..     lag(variable)
..     variable
..     number
..
*/

expr     : expr '+' expr      { $$ = newnode(add, "+"  ,$1,$3); }
         | expr '-' expr      { $$ = newnode(sub, "-"  ,$1,$3); }
         | expr '*' expr      { $$ = newnode(mul, "*"  ,$1,$3); }
         | expr '/' expr      { $$ = newnode(dvd, "/"  ,$1,$3); }
         | expr '^' expr      { $$ = newnode(pow, "^"  ,$1,$3); }
         | '-' expr %prec NEG { $$ = newnode(neg, "-"  , 0,$2); }
         | '(' expr ')'       { $$ = $2; }
         | func               { $$ = $1; }
         | setfunc            { $$ = $1; }
         | lag                { $$ = $1; }
         | lead               { $$ = $1; }
         | var                { $$ = $1; }
         | NUM                { $$ = $1; }
         ;

/*
.. ## Variable References:
..
..     name
..     name#list
..     name(sets)
..     name(sets)#list
..
*/

var      : NAME                       { $$ = $1; }
         | NAME '(' list ')'          { $$ = $1; $1->r=newnode(lst,"lst",0,$3); }
         | NAME '#' NAME              { $$ = newnode(dom,"#",$1,$3); }
         | NAME '(' list ')' '#' NAME { $$ = newnode(dom,"#",$1,$6); $1->r=newnode(lst,"lst",0,$3); } 
         ;

/*
.. ## Functions:
..
..     EXP(expr)
..     LN(expr)      
..     LOG(expr)
..     PROD(set,expr)
..     SUM(set,expr)
..
*/

func     : LOG  '(' expr ')'     { $$ = newnode(log,"log", 0,$3); }
         | EXP  '(' expr ')'     { $$ = newnode(exp,"exp", 0,$3); }
         ;
       
lag      : LAG  '(' var  ')'     { $$ = newnode(lag,"lag", 0,$3); }
         | LAG  '(' lag  ')'     { $$ = newnode(lag,"lag", 0,$3); }
         ;

lead     : LEAD  '(' var  ')'    { $$ = newnode(led,"lead",0,$3); }
         | LEAD  '(' lead ')'    { $$ = newnode(led,"lead",0,$3); }
         | NEXT  '(' var  ')'    { $$ = newnode(led,"lead",0,$3); }
         | NEXT  '(' lead ')'    { $$ = newnode(led,"lead",0,$3); }
         ;

setfunc  : SUM   '(' NAME ',' expr ')'  { $$ = newnode(sum,"sum"  ,$3,$5); }
         | PROD  '(' NAME ',' expr ')'  { $$ = newnode(prd,"prod" ,$3,$5); }
         ;

%%


// error handling is done in readfile rather than here

void yyerror(char *str)
{
}

// recover the last token for error messages

char *getlasttoken()
{
   if( yylval==0 )return 0;
   return snprint(yylval);
}

/*  Include lexical analyzer so yylex and yyparse can share tokens  */

#include "lexical.c"
