%option yylineno
%option noyywrap

char* msgError[256];

%%
[ \n\t] {}
"{"                                     { return INICIOBLOQUE; }
"}"                                     { return FINBLOQUE; }
"["                                     { return CORCHETEIZQ; }
"]"                                     { return CORCHETEDER; }
"("                                     { return PARIZQ; }
")"                                     { return PARDER; }
";"                                     { return PYC; }
","                                     { return COMA; }
"="                                     { return ASIGN; }
"@"                                     { return ARROBA; }
"input "                                { return INPUT; }
"output"                                { return OUTPUT; }
"return"                                { return RETURN; }
"var"                                   { return VAR; }
"main"                                  { return MAIN; }
"repeat"                                { return REPEAT; }
"until"                                 { return UNTIL; }


"int"                                   {yylval.atributo = 0; yylval.dtipo = entero; return PRIMITIVO;}
"float"                                 {yylval.atributo = 1; yylval.dtipo = real; return PRIMITIVO; }
"char"                                  {yylval.atributo = 2; yylval.dtipo = caracter; return PRIMITIVO; }
"bool"                                  {yylval.atributo = 3; yylval.dtipo = booleano; return PRIMITIVO; }


"list"" "+"of"" "+"int"          { yylval.atributo = 0; yylval.dtipo = listaEntero; return ESTRUCTURA; } // WARNING hemos separado tipo de estructura en todos los list of
"list"" "+"of"" "+"float"        { yylval.atributo = 1; yylval.dtipo = listaReal; return ESTRUCTURA; }
"list"" "+"of"" "+"char"         { yylval.atributo = 2; yylval.dtipo = listaCaracter; return ESTRUCTURA; }
"list"" "+"of"" "+"bool"         { yylval.atributo = 3; yylval.dtipo = listaBooleano; return ESTRUCTURA; }


"if"                                    { return IF; }
"while"                                 { return WHILE; }
"else"                                  { return ELSE; }
"and"                                   {return ANDLOG;}
"or"                                    {return ORLOG;}
"xor"                                   {return XOR;}

"<<"					 {yylval.atributo = 0;return OPERLISTA;}
">>"                                    {yylval.atributo = 1; return OPERLISTA;}


"*"                                     {yylval.atributo = 0; return MULDIV;}
"/"                                     {yylval.atributo = 1; return MULDIV;}
"%"                                {return PORPOT;} // Hemso quitado esto     |"^" 

"=="                                    {yylval.atributo = 0; return EQN; }
"!="                                    {yylval.atributo = 1; return EQN; }


"**"                                    {return PORPOR;}


"<"                                     { yylval.atributo = 0; return REL; }
">"                                     { yylval.atributo = 1; return REL; }
"<="                                    { yylval.atributo = 2; return REL; }
">="                                    { yylval.atributo = 3; return REL; }


"--"                                    {return MENOSMENOS; } // TODO Cuidado con este a ver que pasa

"++"                                    {return OPERMASMAS; }

"+"                                     { yylval.atributo = 0; return MASMENOS; }
"-"                                     { yylval.atributo = 1; return MASMENOS; }

"//"                                    {return BAR; } // TODO Adaptarlo al # de ellos, supuestamente esto no debería de hacer na
"?"                                     {return INT; }

"not"                                   {return NOT; }

"$"                                     {return DOLLAR; }

\"[^\"]*\"                              {return CADENA; }


[0-9]+                                  { yylval.atributo = 0; yylval.dtipo = entero; return CONSTANTE; }
[0-9]*\.[0-9]*                          { yylval.atributo = 1; yylval.dtipo = real; return CONSTANTE; }

[a-z|A-Z][a-z|A-Z|0-9|_]*               {yylval.lexema = strdup(yytext); return ID;}

<*>.|\n                            {sprintf(msgError, "ERROR LÉXICO: Lexema %s no reconocible.\n", yytext); yyerror(msgError);}
%%



