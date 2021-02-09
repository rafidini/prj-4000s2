MAJ        [A-Z]
MIN        [a-z]
DIGIT      [0-9]
ALPHA      {MAJ}|{MIN}
ALPHAUS    {ALPHA}|"_"
ALPHANUM   {ALPHAUS}|{DIGIT}
ident      {ALPHAUS}{ALPHANUM}*
motcle1    asm|auto|break|catch|case|char
motcle2    class|const|continue|default|delete|do
motcle3    double|else|enum|extern|float|for
motcle4    friend|goto|if|inline|int|long
motcle5    new|operator|overload|private|protected|public
motcle6    register|return|short|signed|sizeof|static
motcle7    struct|switch|this|template|typedef|union
motcle8    unsigned|virtual|void|volatile|while
motcle     {motcle1}|{motcle2}|{motcle3}|{motcle4}|{motcle5}|{motcle6}|{motcle7}|{motcle8}
begpp      "#"
begtag     "<"
endtag     ">"
ampers     "&"
begcom     "/*"
endcom     "*/"
begcoml    "//"
begstr     "\""
endstr     "\""
bckslh     "\\"
%X COMMENT
%X COMMENTLINE
%X CHAINE
%X PREPROC
%%
^{begpp}          {
                    BEGIN PREPROC;
                    printf("<font color=\"Blue\">#");
                  }
<PREPROC>{begtag} printf("&lt;");
<PREPROC>{endtag} printf("&gt;");
<PREPROC>{ampers} printf("&amp;");
<PREPROC>[\n]     {
                    printf("</font>\n");
                    BEGIN 0;
                  }
{begcoml}          {
                    BEGIN COMMENTLINE;
                    printf("<font color=\"Red\">//");
                  }
<COMMENTLINE>{begtag} printf("&lt;");
<COMMENTLINE>{endtag} printf("&gt;");
<COMMENTLINE>{ampers} printf("&amp;");
<COMMENTLINE>[\n]     {
                    printf("</font>\n");
                    BEGIN 0;
                  }
{begcom}	  {
                    BEGIN COMMENT;
                    printf("<font color=\"Red\">/*");
                  }
<COMMENT>{begtag} printf("&lt;");
<COMMENT>{endtag} printf("&gt;");
<COMMENT>{ampers} printf("&amp;");
<COMMENT>{endcom} {
                    printf("*/</font>");
                    BEGIN 0;
                  }
{begstr}	  {
                    BEGIN CHAINE;
                    printf("<font color=\"Green\">\"");
                  }
<CHAINE>{begtag}  printf("&lt;");
<CHAINE>{endtag}  printf("&gt;");
<CHAINE>{ampers}  printf("&amp;");
<CHAINE>{bckslh}{endstr}  printf("%s", yytext);
<CHAINE>{endstr}  { 
                    printf("\"</font>");
                    BEGIN 0;
                  }
<<EOF>>           return;
{begtag}          printf("&lt;");
{endtag}          printf("&gt;");
{ampers}          printf("&amp;");
{motcle}          printf("<b>%s</b>", yytext);
{ident}           printf("%s", yytext);
%%		     

main( argc, argv )
  int argc;
  char **argv;
{
  ++argv, --argc;  /* skip over program name */
  if ( argc > 0 )
          yyin = fopen( argv[0], "r" );
  else
          yyin = stdin;
  printf("<html><body><pre>");
  yylex();
  printf("</pre></body></html>");  
}


