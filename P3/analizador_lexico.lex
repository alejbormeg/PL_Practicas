%option yylineno
%option noyywrap
%{
	//#include "tabla.h"
	#include "y.tab.h"
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
"@"                                     { return ARROBA; }
"input "                                { return INPUT; }
"output"                                { return OUTPUT; }
"return"                                { return RETURN; }
"var"                                   { return VAR; }
"main"                                  { return MAIN; }
"repeat"                                { return REPEAT; }
"until"                                 { return UNTIL; }
"int"                                   { return PRIMITIVO; }
"float"                                 { return PRIMITIVO; }
"char"                                  { return PRIMITIVO; }
"bool"                                  { return PRIMITIVO; }
"list of"                               { return ESTRUCTURA; }
"if"                                    { return IF; }
"while"                                 { return WHILE; }
"else"                                  { return ELSE; }
">>"|"<<"                               {return OPERLISTA;}
"*"|"/"                                 {return MULDIV;}
"%"|"^"                                 {return PORPOT;}
"=="|"!="                               {return EQN;}
"**"                                    {return PORPOR;}
"and"                                   {return ANDLOG;}
"or"                                    {return ORLOG;}
"xor"                                   {return XOR;}
"<"|">"|"<="|">="                       {return REL;}
"--"                                    {return MENOSMENOS; }
"++"                                    {return OPERMASMAS; }
"+"                                     {return MASMENOS;}
"-"                                     {return MASMENOS;}
"//"                                    {return BAR; }
"?"                                     {return INT; }
"not"                                   {return NOT; }
"$"                                     {return DOLLAR; }
\"[^\"]*\"                              {return CADENA; }
([0-9]+)|([0-9]*\.[0-9]*)|"true"|"false"|\'[^\']\'                              { return CONSTANTE; }
[a-z|A-Z][a-z|A-Z|0-9|_]*                                                            { return ID; }
<*>.|\n                            { printf("Error en la línea %d. Lexema %s no reconocible.\n", yylineno, yytext); }
%%
/*
    int main (){
    int val;
    val = yylex();

    while (val != 0){
      printf("%d\n", val);
      val = yylex();
    }

    exit(0);
}
*/


