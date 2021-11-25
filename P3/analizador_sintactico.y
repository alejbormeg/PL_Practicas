%{
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void yyerror( const char * msg );

#define YYERROR_VERBOSE
%}

%start programa
%token MAIN
%token VAR
%token PRIMITIVO
%token PYC
%token INPUT OUTPUT
%token CADENA
%token RETURN
%token IF ELSE
%token REPEAT UNTIL
%token WHILE
%token OPERLISTA
%token CONSTANTE
%token INICIOBLOQUE FINBLOQUE
%token CORCHETEIZQ CORCHETEDER
%token COMA
%token ASIGN
%token PARDER PARIZQ
%token OPUNARIO
%token ESTRUCTURA
%token ID

/* Ternario */
%right OPERMASMAS ARROBA


/* Binario */
%left OPBINARIO 


/* Sum rest */
%left MASMENOS


/* Unarios */
%precedence INTHASH EXCL
%right OPUNARIOBINARIO
%right OPUNARIO


%%

programa : MAIN PARIZQ PARDER bloque ;

bloque : INICIOBLOQUE declar_de_variables_locales declar_de_subprogs sentencias FINBLOQUE ;


declar_de_subprogs : declar_de_subprogs declar_subprog
                   | %empty ;

declar_subprog : cabecera_subprog bloque ;

declar_de_variables_locales : VAR INICIOBLOQUE variables_locales FINBLOQUE
                            | %empty ;

variables_locales : variables_locales cuerpo_declar_variables
                  | cuerpo_declar_variables ;

cuerpo_declar_variables : tipo lista_variables PYC
                        | error ;
tipo  : PRIMITIVO
      | ESTRUCTURA ;

lista_variables : ID COMA lista_variables
                | ID ;

cabecera_subprog : tipo ID PARIZQ parametros PARDER ;


lista_expresiones : lista_expresiones COMA expresion
                  | expresion ;


parametros : parametros COMA tipo ID
           | tipo ID ;

sentencias : sentencias sentencia
           | %empty ;

sentencia : bloque
          | sentencia_asignacion 
          | sentencia_if
          | sentencia_while
          | sentencia_entrada
          | sentencia_salida
          | sentencia_return
          | sentencia_repeat_until
          | sentencia_lista ;

sentencia_asignacion : ID ASIGN expresion PYC ;

sentencia_if : IF PARIZQ expresion PARDER sentencia sentencia_else ;

sentencia_else  : ELSE sentencia
                | %empty ;

sentencia_while : WHILE PARIZQ expresion PARDER sentencia ;

sentencia_entrada : INPUT lista_id PYC ;
lista_id  : lista_id COMA ID
          | ID ;

sentencia_salida : OUTPUT lista_expresion_cadena PYC ;

lista_expresion_cadena  : lista_expresion_cadena COMA expresion_cadena
                        | expresion_cadena ;

expresion_cadena : expresion
                 | CADENA ;

sentencia_return : RETURN expresion PYC ;

sentencia_repeat_until  : REPEAT sentencia UNTIL PARIZQ expresion PARDER PYC ;

sentencia_lista : expresion OPERLISTA
                | OPUNARIO expresion ;

expresion : PARIZQ expresion PARDER
          | OPUNARIO expresion
          | expresion OPBINARIO expresion
          | expresion OPERMASMAS expresion ARROBA expresion
          | ID
          | CONSTANTE
          | funcion ;

funcion : ID PARIZQ lista_expresiones PARDER

%%

#include "lex.yy.c"

void yyerror(const char *msg){
  fprintf(stderr, "[Linea %d]: %s\n", yylineno, msg);
}

int main(){
  yyparse();

  return 0;
}
