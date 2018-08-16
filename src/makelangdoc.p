$tar = "langdoc.c";
@inp = glob("lang/*.c");

unlink $tar if -e $tar;

open( OUT, "> $tar" ) || die( "Could not open a new $tar\n" );

print OUT "#include <stdio.h>\n";
print OUT "void langdoc()\n";
print OUT "{\n";
print OUT "   char from_lang[]={\"\\\n";

foreach $src ( @inp ) {
   if( $src ne 'langdoc.c' && $src ne 'setup.c' ) {
      $hit = 0;
      open( INP, "< $src" ) || die( "Could not open parser file $src\n" );
      while( <INP> )
         {
         next unless /^\.\./ ;
         s/^\.\.//;
         s/^ //;
         s/\s*$//;
         print OUT "$_\\n\\\n"; 
         $hit += 1;
         }
      if( $hit > 0 ) {
         print OUT "\\n\\\n";
      }
   }
}

print OUT "\\n\"};\n";
print OUT "printf(from_lang);\n";
print OUT "}\n";

close OUT;


