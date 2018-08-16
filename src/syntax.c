#include <stdio.h>
void syntax()
{
   char from_parse_y[]={"\
# Sym Input File Syntax\n\
\n\
Input files consist of: **set**, **parameter** and **variable**\n\
declarations, and **equations**. All statements must end with a\n\
semicolon. Anything from a // to the end of the line is taken to\n\
be a comment and is ignored. Items in square brackets are\n\
optional and may be omitted. Attributes are target-language\n\
dependent: there are no attributes applicable to all languages.\n\
\n\
## Set Declarations:\n\
\n\
Set elements do not need to be unique as long as sets using the\n\
same element names cannot be mistaken for subsets of each other.\n\
For example, elements a and b will distinct in set1(a,b,c) and\n\
set2(a,b,d). However, using an additional set3(a,b) should be\n\
avoided and will be flagged as an error in subsequent versions\n\
of sym.\n\
\n\
    SET name (elements)              ['description'] ;\n\
    SET name = prevset               ['description'] ;\n\
    SET name = prevset(elements)     ['description'] ;\n\
    SET name = prevset + (elements)  ['description'] ;\n\
    SET name = prevset - (elements)  ['description'] ;\n\
\n\
## Variable and Parameter Declarations:\n\
\n\
    PARAMETER name [(sets)]   ['description'] [attributes] ;\n\
    VARIABLE  name [(sets)]   ['description'] [attributes] ;\n\
\n\
## Equations:\n\
\n\
    [EQUATION name|/name/] [sets:] expr = expr ['description'] [attributes];\n\
\n\
## Expressions:\n\
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
## Variable References:\n\
\n\
    name\n\
    name#list\n\
    name(sets)\n\
    name(sets)#list\n\
\n\
## Functions:\n\
\n\
    EXP(expr)\n\
    LN(expr)\n\
    LOG(expr)\n\
    PROD(set,expr)\n\
    SUM(set,expr)\n\
\n\
\n"};
printf(from_parse_y);
}
