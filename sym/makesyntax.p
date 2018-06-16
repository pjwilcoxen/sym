$src = "parse.y";
$tar = "syntax.c";

unlink $tar if -e $tar;

open( INP, "< $src" ) || die( "Could not open parser file $src\n" );
open( OUT, "> $tar" ) || die( "Could not open a new $tar\n" );

print OUT "#include <stdio.h>\n";
print OUT "void syntax()\n";
print OUT "{\n";
print OUT "   char from_parse_y[]={\"\\n\\\n";

while( <INP> ){ last if /^%%/; }
while( <INP> )
   {
   last if /^%%/;
   next unless /^\.\./ ;
   s/^\.\.//;
   s/^ //;
   s/\s*$//;
   print OUT "$_\\n\\\n"; }

print OUT "\\n\"};\n";
print OUT "printf(from_parse_y);\n";
print OUT "}\n";

close OUT;


