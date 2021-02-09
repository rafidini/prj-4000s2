MAJ        [A-Z]
MIN        [a-z]
DIGIT      [0-9]
ALPHA      {MAJ}|{MIN}
ALPHAUS    {ALPHA}|"_"
ALPHANUM   {ALPHAUS}|{DIGIT}
BLANC      " "
WS         {BLANC}+
IDENT      {ALPHAUS}{ALPHANUM}*
IDENT2     {IDENT}{WS}{IDENT}
IDENT3     {IDENT}{WS}{IDENT}{WS}{IDENT}
I8         "signed"{WS}"char"
U8         "unsigned"{WS}"char"|"ubyte"
I16        "short"|"short"{WS}"int"|"signed"{WS}"short"|"signed"{WS}"short"{WS}"int"|"short"{WS}"signed"{WS}"int"
U16        "unsigned"{WS}"short"|"unsigned"{WS}"short"{WS}"int"|"short"{WS}"unsigned"{WS}"int"
I32        "boolean"|"int"|"long"|"long"{WS}"int"|"signed"{WS}"long"|"signed"{WS}"long"{WS}"int"|"long"{WS}"signed"{WS}"int"|"signed"{WS}"int"
U32        "unsigned"{WS}"long"|"unsigned"{WS}"long"{WS}"int"|"long"{WS}"unsigned"{WS}"int"|"unsigned"{WS}"int"|"ulong"
%%
{U8}    printf("u_int8_t");
{I8}    printf("int8_t");
{U16}   printf("u_int16_t");
{I16}   printf("int16_t");
{U32}   printf("u_int32_t");
{I32}   printf("int32_t");

"extern"{WS}{U8}    printf("extern u_int8_t");
"extern"{WS}{I8}    printf("extern int8_t");
"extern"{WS}{U16}   printf("extern u_int16_t");
"extern"{WS}{I16}   printf("extern int16_t");
"extern"{WS}{U32}   printf("extern u_int32_t");
"extern"{WS}{I32}   printf("extern int32_t");

"extern"{WS}"u_int8_t"		   printf("%s", yytext);
"extern"{WS}"int8_t"		   printf("%s", yytext);
"extern"{WS}"u_int16_t"		   printf("%s", yytext);
"extern"{WS}"int16_t"		   printf("%s", yytext);
"extern"{WS}"u_int32_t"		   printf("%s", yytext);
"extern"{WS}"int32_t"		   printf("%s", yytext);

"static"{WS}{U8}    printf("static u_int8_t");
"static"{WS}{I8}    printf("static int8_t");
"static"{WS}{U16}   printf("static u_int16_t");
"static"{WS}{I16}   printf("static int16_t");
"static"{WS}{U32}   printf("static u_int32_t");
"static"{WS}{I32}   printf("static int32_t");

"static"{WS}"u_int8_t"		   
"static"{WS}"int8_t"		   
"static"{WS}"u_int16_t"		   
"static"{WS}"int16_t"		   
"static"{WS}"u_int32_t"		   
"static"{WS}"int32_t"		   

"register"{WS}{U8}    printf("register u_int8_t");
"register"{WS}{I8}    printf("register int8_t");
"register"{WS}{U16}   printf("register u_int16_t");
"register"{WS}{I16}   printf("register int16_t");
"register"{WS}{U32}   printf("register u_int32_t");
"register"{WS}{I32}   printf("register int32_t");

"register"{WS}"u_int8_t"	       printf("%s", yytext);
"register"{WS}"int8_t"		       printf("%s", yytext);
"register"{WS}"u_int16_t"	       printf("%s", yytext);
"register"{WS}"int16_t"		       printf("%s", yytext);
"register"{WS}"u_int32_t"	       printf("%s", yytext);
"register"{WS}"int32_t"		       printf("%s", yytext);

{IDENT}               printf("%s", yytext);

%%		     
