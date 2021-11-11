%option yylineno
%{
	#include "tabla.h"
%}
int acarrero = 1;
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
"@"                                       { return ARROBA; }
"input "                                   { return INPUT; }
"output"                                  { return OUTPUT; }
"return"                                  { return RETURN; }
"var"                                     { return VAR; }
"main"                                    { return MAIN; }
"repeat"                                  { return REPEAT; }
"until"                                   { return UNTIL; }
"int"|"float"|"char"|"bool"               { return PRIMITIVO; }
"list of"                                 { return ESTRUCTURA; }
"if"                                      { return IF; }
"while"                                   { return WHILE; }
"else"                                    { return ELSE; }
"<<"|">>"                             {return OPERLISTA;}
"*"|"/"|"%"|"^"|"=="|"**"|"!="|"and"|"or"|"xor"|"<"|">"|"<="|">="  {return OPBINARIO;}
"--"                                    { return OPUNARIOBINARIO; }
"++"                                    { return OPERMASMAS; }
"+"|"-"                                {return MASMENOS;}
"//"|"not"|"$"|"?"               { return OPUNARIO; }
\"[^\"]*\"                                 { return CADENA; }
([0-9]+)|([0-9]*\.[0-9]*)|"true"|"false"|\'[^\']\'                              { return CONSTANTE; }
[a-z|A-Z][a-z|A-Z|0-9|_]*                                                            { return ID; }
<*>.|\n                            { printf("Error en la línea %d. Lexema %s no reconocible.\n", yylineno, yytext); }
%%
    int main (){
    int val;
    val = yylex();

    while (val != 0){
      printf("%d\n", val);
      val = yylex();
    }

    exit(0);
}


