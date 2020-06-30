#include <stdio.h>
void langdoc()
{
   char from_lang[]={"\
### debug\n\
\n\
Write out in a format useful for checking equations.\n\
\n\
+ Equation descriptions will be written\n\
\n\
### html\n\
\n\
Write out the model in HTML for use as documentation.\n\
\n\
+ Equation descriptions will be written\n\
\n\
### msgproc\n\
\n\
Support the Ox implementation of the MSG/G-Cubed solution algorithm.\n\
\n\
### oxgs\n\
\n\
Support a simple Ox implementation of the Gauss-Seidel algorithm.\n\
\n\
### oxgst\n\
\n\
Extended version of the OxGS target that removes a restriction\n\
in OxGS limiting it to single periods.\n\
\n\
### oxnewton\n\
\n\
Support an Ox implementation of Newton's Method for simple\n\
models.\n\
\n\
### tablo\n\
\n\
Support GEMPACK's TABLO language.\n\
\n\
+ All variables are implicitly subscripted by time. However,\n\
  parameters are not.\n\
\n\
+ The left side of an equation may be an expression, not just a\n\
  variable name.\n\
\n\
+ At most one attribute is allowed in variable and parameter\n\
  declarations. If present, it is used as the name of an HAR header\n\
  and should have the form: c###.\n\
\n\
+ Parameters are read from TABLO logical name 'param'. Variables\n\
  are read from one of the following files depending on the value\n\
  of the first letter of the header: B = 'base', K = 'kalman',\n\
  M = 'make', N = 'endog', and X = 'exog'. If the first letter does\n\
  not match one of the above 'base' will be used as the file name.\n\
\n\
### troll\n\
\n\
Support TROLL.\n\
\n\
\n"};
printf(from_lang);
}
