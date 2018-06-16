#include <stdio.h>
void syntax()
{
   char from_parse_y[]={"\n\
Summary of Sym Input File Syntax\n\
\n\
   Input files consist of: (1) set declarations, (2) parameter\n\
   and variable declarations, and (3) equations.  All statements\n\
   must end with a semicolon.  Anything from a // to the end of\n\
   the line is taken to be a comment and is ignored.\n\
\n\
Set Declarations:\n\
\n\
   SET name (elements)              ['description'] ;\n\
   SET name = prevset               ['description'] ;\n\
   SET name = prevset + (elements)  ['description'] ;\n\
   SET name = prevset - (elements)  ['description'] ;\n\
\n\
Variable and Parameter Declarations:\n\
\n\
   PARAMETER name [(sets)]   ['description'] [attributes] ;\n\
   VARIABLE  name [(sets)]   ['description'] [attributes] ;\n\
\n\
Equations:\n\
\n\
   [EQUATION name|/name/] [sets:] expr = expr [attributes];\n\
\n\
Expressions:\n\
\n\
   expr + expr\n\
   expr - expr\n\
   expr * expr\n\
   expr / expr\n\
   expr ^ expr\n\
   - expr\n\
   ( expr )\n\
   function\n\
   lead(variable)\n\
   lag(variable)\n\
   variable\n\
   number\n\
\n\
Variable References:\n\
\n\
   name\n\
   name#list\n\
   name(sets)\n\
   name(sets)#list\n\
\n\
Functions:\n\
\n\
   LOG(expr)  Note: synonym for ln()\n\
   LN (expr)\n\
   EXP(expr)\n\
\n\
   SUM (set,expr)\n\
   PROD(set,expr)\n\
\n\
\n"};
printf(from_parse_y);
}
