
Summary of Sym Input File Syntax

   Input files consist of: (1) set declarations, (2) parameter
   and variable declarations, and (3) equations.  All statements
   must end with a semicolon.  Anything from a // to the end of
   the line is taken to be a comment and is ignored.

Set Declarations:

   SET name (elements)              ['description'] ;
   SET name = prevset               ['description'] ;
   SET name = prevset + (elements)  ['description'] ;
   SET name = prevset - (elements)  ['description'] ;

Variable and Parameter Declarations:

   PARAMETER name [(sets)]   ['description'] [attributes] ;
   VARIABLE  name [(sets)]   ['description'] [attributes] ;

Equations:

   [EQUATION name|/name/] [sets:] expr = expr [attributes];

Expressions:

   expr + expr
   expr - expr
   expr * expr
   expr / expr
   expr ^ expr
   - expr
   ( expr )
   function
   lead(variable)
   lag(variable)
   variable
   number

Variable References:

   name
   name#list
   name(sets)
   name(sets)#list

Functions:

   LOG(expr)  Note: synonym for ln()
   LN (expr)
   EXP(expr)

   SUM (set,expr)
   PROD(set,expr)


