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
"@"                         { yylval.lexema = strdup(yytext); return ARROBA; }
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
"||"                        { yylval.lexema = strdup(yytext); return ORLOG; }
"&&"                        { yylval.lexema = strdup(yytext); return ANDLOG; }
"=="                        { yylval.lexema = strdup(yytext); yylval.atributo = 0; return EQN; }
"!="                        { yylval.lexema = strdup(yytext); yylval.atributo = 1; return EQN;}
"<"                         { yylval.lexema = strdup(yytext); yylval.atributo = 0; return REL; }
">"                         { yylval.lexema = strdup(yytext); yylval.atributo = 1; return REL; }
"<="                        { yylval.lexema = strdup(yytext); yylval.atributo = 2; return REL; }
">="                        { yylval.lexema = strdup(yytext); yylval.atributo = 3; return REL; }
"+"                         { yylval.lexema = strdup(yytext); yylval.atributo = 0; return MASMENOS; }
"-"                         { yylval.lexema = strdup(yytext); yylval.atributo = 1; return MASMENOS; }
"*"                         { yylval.lexema = strdup(yytext); yylval.atributo = 0; return MULDIV;}
"/"                         { yylval.lexema = strdup(yytext); yylval.atributo = 1; return MULDIV; }
"!"                         { yylval.lexema = strdup(yytext); return EXCL; }
"**"                        { yylval.lexema = strdup(yytext); return PORPOR; }
"--"                        { yylval.lexema = strdup(yytext); yylval.atributo = 0; return BORRLIST; }
"%"                         { yylval.lexema = strdup(yytext); yylval.atributo = 1; return BORRLIST; }
"?"                         { yylval.lexema = strdup(yytext); yylval.atributo = 0; return INTHASH; }
"#"                         { yylval.lexema = strdup(yytext); yylval.atributo = 1; return INTHASH; }
"++"                        { yylval.lexema = strdup(yytext); return MASMAS; }
"$"                         { yylval.lexema = strdup(yytext); return DOLLAR; }
"<<"                        { yylval.lexema = strdup(yytext); yylval.atributo = 0; return OPERLISTA; }
">>"                        { yylval.lexema = strdup(yytext); yylval.atributo = 1; return OPERLISTA; }
\"[^\"]*\"                  { yylval.lexema = strdup(yytext); yylval.dtipo = cadena; return CADENA; }
[0-9]+                      { yylval.lexema = strdup(yytext); yylval.atributo = 0; yylval.dtipo = entero; return CONST; }
[0-9]*\.[0-9]*              { yylval.lexema = strdup(yytext); yylval.atributo = 1; yylval.dtipo = real; return CONST; }
\'[^\']\'                   { yylval.lexema = strdup(yytext); yylval.atributo = 2; yylval.dtipo = caracter; return CONST; }
"true"|"false"              { yylval.lexema = strdup(yytext); yylval.atributo = 3; yylval.dtipo = booleano; return CONST; }
[a-z|A-Z][a-z|A-Z|0-9|_]*   { yylval.lexema = strdup(yytext); return ID; }
[ \t\n]                     { ; }
<*>.|\n                     {sprintf(msgError, "ERROR LÉXICO: Lexema %s no reconocible.\n", yytext); yyerror(msgError);}

%%
