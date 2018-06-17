# Sym Input File Syntax

Input files consist of: **set**, **parameter** and **variable**
declarations, and **equations**. All statements must end with a
semicolon. Anything from a // to the end of the line is taken to
be a comment and is ignored. Items in square brackets are
optional and may be omitted. Attributes are target-language
dependent: there are no attributes applicable to all languages.

## Set Declarations:

Set elements do not need to be unique as long as sets using the
same element names cannot be mistaken for subsets of each other.
For example, elements a and b will distinct in set1(a,b,c) and
set2(a,b,d). However, using an additional set3(a,b) should be
avoided and will be flagged as an error in subsequent versions
of sym.

    SET name (elements)              ['description'] ;
    SET name = prevset               ['description'] ;
    SET name = prevset + (elements)  ['description'] ;
    SET name = prevset - (elements)  ['description'] ;

## Variable and Parameter Declarations:

    PARAMETER name [(sets)]   ['description'] [attributes] ;
    VARIABLE  name [(sets)]   ['description'] [attributes] ;

## Equations:

    [EQUATION name|/name/] [sets:] expr = expr [attributes];

## Expressions:

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

## Variable References:

    name
    name#list
    name(sets)
    name(sets)#list

## Functions:

    EXP(expr)
    LN(expr)
    LOG(expr)
    PROD(set,expr)
    SUM(set,expr)


