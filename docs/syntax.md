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
    SET name = prevset(elements)     ['description'] ;
    SET name = prevset + (elements)  ['description'] ;
    SET name = prevset - (elements)  ['description'] ;
    SET name = prevset + otherset    ['description'] ;
    SET name = prevset - otherset    ['description'] ;
    SET name = UNION(setlist)        ['description'] ;

## Variable and Parameter Declarations:

    PARAMETER name [(sets)]   ['description'] [attributes] ;
    VARIABLE  name [(sets)]   ['description'] [attributes] ;

## Equations:

    [EQUATION name|/name/] [sets:] expr = expr ['description'] [attributes];
    ['description'] [EQUATION name|/name/] [sets:] expr = expr [attributes];

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

## Notes on Output Targets:

The sections below provide additional information about the features
or requirements of specific output targets.

### debug

Write out in a format useful for checking equations.

+ Equation descriptions will be written

### html

Write out the model in HTML for use as documentation.

+ Equation descriptions will be written

### msgproc

Support the Ox implementation of the MSG/G-Cubed solution algorithm.

### oxgs

Support a simple Ox implementation of the Gauss-Seidel algorithm.

### oxgst

Extended version of the OxGS target that removes a restriction
in OxGS limiting it to single periods.

### oxnewton

Support an Ox implementation of Newton's Method for simple
models.

### tablo

Support GEMPACK's TABLO language.

+ All variables are implicitly subscripted by time. However,
  parameters are not.

+ The left side of an equation may be an expression, not just a
  variable name.

+ At most one attribute is allowed in variable and parameter
  declarations. If present, it is used as the name of an HAR header
  and should have the form: c###.

+ Parameters are read from TABLO logical name 'param'. Variables
  are read from one of the following files depending on the value
  of the first letter of the header: B = 'base', K = 'kalman',
  M = 'make', N = 'endog', and X = 'exog'. If the first letter does
  not match one of the above 'base' will be used as the file name.

### troll

Support TROLL.


