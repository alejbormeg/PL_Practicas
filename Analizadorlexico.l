%option yylineno
%{
	#include "tabla.h"
%}

%%

"{"                                                                                          { return INIBLOQUE; }
"}"                                                                                          { return FINBLOQUE; }
"["                                                                                          { return CORIZQ; }
"]"                                                                                          { return CORDER; }
"("                                                                                          { return PARIZQ; }
")"                                                                                          { return PARDER; }
";"                                                                                          { return PYC; }
","                                                                                          { return COMA; }
"="                                                                                          { return ASIGN; }
"@"                                                                                          { return AT; }
"$"                                                                                          { return DOLLAR; }
"++"                                                                                         { return MASMAS; }
"cin"                                                                                        { return CIN; }
"cout"                                                                                       { return COUT; }
"return"                                                                                     { return RETURN; }
"local"                                                                                      { return LOCAL; }
"main"                                                                                       { return MAIN; }
"do"                                                                                         { return DO; }
"until"                                                                                      { return UNTIL; }
"int"|"float"|"char"|"bool"|"list_of int"|"list_of float"|"list_of char"|"list_of bool"      { return TIPO; }
"if"                                                                                         { return IF; }
"while"                                                                                      { return WHILE; }
"else"                                                                                       { return ELSE; }
"+"|"-"                                                                                      { return BINYUN; }
"<<"|">>"                                                                                    { return SHIFT; }
"*"|"/"|"%"|"**"|"=="|"!="|"&&"|"||"|"<"|">"|"<="|">="|"&"|"|"|"^"|"--"                      { return OPBIN; }
"!"|"#"|"?"|"+"|"-"                                                                          { return OPUNARIO; }
\"[^\"]*\"                                                                                   { return CADENA; }
([0-9]+)|([0-9]*\.[0-9]*)|"true"|"false"|\'[^\']\'                                           { return CONST; }
[a-z|A-Z][a-z|A-Z|0-9|_]*                                                                    { return ID; }
[ \t\n]                                                                                      { ; }
<*>.|\n                                                                                      { printf("Error en la línea %d. Lexema %s no reconocible.\n", yylineno, yytext); }

%%
main (){
	int val;
	val = yylex();

	while (val != 0){
		printf("%d\n", val);
		val = yylex();
	}

	exit(0);
}
