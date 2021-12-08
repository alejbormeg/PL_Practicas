%option yylineno
%option noyywrap

char* msgError[256];

%%

"{"                         { return INIBLOQUE; }
"}"                         { return FINBLOQUE; }
"["                         { return CORIZQ; }
"]"                         { return CORDER; }
"("                         { return PARIZQ; }
")"                         { return PARDER; }
";"                         { return PYC; }
","                         { return COMA; }
"="                         { return ASIGN; }
"@"                         { return ARROBA; }
"input"                     { return INPUT; }
"output"                    { return OUTPUT; }
"return"                    { return RETURN; }
"var"                       { return VAR; }
"main"                      { return MAIN; }
"repeat"                    { return REPEAT; }
"until"                     { return UNTIL; }
"int"                       { yylval.atributo = 0; yylval.dtipo = entero; return TIPO; }
"float"                     { yylval.atributo = 1; yylval.dtipo = real; return TIPO; }
"char"                      { yylval.atributo = 2; yylval.dtipo = caracter; return TIPO; }
"bool"                      { yylval.atributo = 3; yylval.dtipo = booleano; return TIPO; }
"list_of"" "+"int"          { yylval.atributo = 4; yylval.dtipo = listaEntero; return TIPO; }
"list_of"" "+"float"        { yylval.atributo = 5; yylval.dtipo = listaReal; return TIPO; }
"list_of"" "+"char"         { yylval.atributo = 6; yylval.dtipo = listaCaracter; return TIPO; }
"list_of"" "+"bool"         { yylval.atributo = 7; yylval.dtipo = listaBooleano; return TIPO; }
"if"                        { return IF; }
"while"                     { return WHILE; }
"else"                      { return ELSE; }
"||"                        { return ORLOG; }
"&&"                        { return ANDLOG; }
"=="                        { yylval.atributo = 0; return EQN; }
"!="                        { yylval.atributo = 1; return EQN;}
"<"                         { yylval.atributo = 0; return REL; }
">"                         { yylval.atributo = 1; return REL; }
"<="                        { yylval.atributo = 2; return REL; }
">="                        { yylval.atributo = 3; return REL; }
"+"                         { yylval.atributo = 0; return MASMENOS; }
"-"                         { yylval.atributo = 1; return MASMENOS; }
"*"                         { yylval.atributo = 0; return MULDIV;}
"/"                         { yylval.atributo = 1; return MULDIV; }
"!"                         { return EXCL; }
"**"                        { return PORPOR; }
"--"                        { yylval.atributo = 0; return BORRLIST; }
"%"                         { yylval.atributo = 1; return BORRLIST; }
"?"                         { yylval.atributo = 0; return INTHASH; }
"#"                         { yylval.atributo = 1; return INTHASH; }
"++"                        { return MASMAS; }
"$"                         { return DOLLAR; }
"<<"                        { yylval.atributo = 0; return OPERLISTA; }
">>"                        { yylval.atributo = 1; return OPERLISTA; }
\"[^\"]*\"                  { return CADENA; }
[0-9]+                      { yylval.atributo = 0; yylval.dtipo = entero; return CONST; }
[0-9]*\.[0-9]*              { yylval.atributo = 1; yylval.dtipo = real; return CONST; }
\'[^\']\'                   { yylval.atributo = 2; yylval.dtipo = caracter; return CONST; }
"true"|"false"              { yylval.atributo = 3; yylval.dtipo = booleano; return CONST; }
[a-z|A-Z][a-z|A-Z|0-9|_]*   { yylval.lexema = strdup(yytext); return ID; }
[ \t\n]                     { ; }
<*>.|\n                     {sprintf(msgError, "ERROR LÉXICO: Lexema %s no reconocible.\n", yytext); yyerror(msgError);}

%%
