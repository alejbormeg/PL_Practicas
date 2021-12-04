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
%token ESTRUCTURA
%token ID
/* Ternario */
%right OPERMASMAS ARROBA

/* Or lógico */
%left ORLOG 

/* AND lógico */
%left ANDLOG 

/* XOR lógico */
%left XOR 

/* Igual distinto */
%left EQN

/* Relacional */
%left REL

%left PORPOR

%left PORPOT

/* Sum rest */
%left MASMENOS

/* Multiplicación división */
%left MULDIV

/* Menos menos */
%right MENOSMENOS

%right NOT

%precedence INT DOLLAR BAR
%%

programa : MAIN PARIZQ PARDER bloque ;

bloque : INICIOBLOQUE { insertarMarca(); }  
	 declar_de_variables_locales 
	 declar_de_subprogs sentencias 
	 FINBLOQUE { vaciarEntradas(); };


declar_de_subprogs : declar_de_subprogs declar_subprog
                   | %empty ;

declar_subprog : cabecera_subprog { subProg = 1; } bloque ;

declar_de_variables_locales : VAR INICIOBLOQUE variables_locales FINBLOQUE
                            | %empty ;

variables_locales : variables_locales cuerpo_declar_variables
                  | cuerpo_declar_variables ;

cuerpo_declar_variables : tipo { tipoTmp = $1.dtipo; } lista_variables PYC
                        | error ;
tipo  : PRIMITIVO
      | ESTRUCTURA tipo;

lista_variables : ID COMA lista_variables { insertarVariable($1.lexema); }
                | ID { insertarVariable($1.lexema); };

cabecera_subprog : tipo ID { insertarFuncion($1.dtipo, $2.lexema); } PARIZQ parametros PARDER 
                 | error ;


lista_expresiones : lista_expresiones COMA expresion {
                    argumentos_tipo_datos[n_argumentos] = $3.dtipo;
                    n_argumentos++;
                  }
                  | expresion {
                    argumentos_tipo_datos[n_argumentos] = $1.dtipo;
                    n_argumentos++;
                  };

parametros : parametros COMA tipo ID { insertarParametro($1.dtipo, $2.lexema); }
           | tipo ID { insertarParametro($1.dtipo, $2.lexema); }
           | error;

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

sentencia_asignacion : ID ASIGN expresion PYC { comprobarAsignacion($1.lexema, $3.dtipo); };

sentencia_if : IF PARIZQ expresion PARDER sentencia sentencia_else  {expresionBooleana($3.dtipo); } ;

sentencia_else  : ELSE sentencia
                | %empty ;

sentencia_while : WHILE PARIZQ expresion PARDER sentencia { expresionBooleana($3.dtipo); } ;

sentencia_entrada : INPUT lista_id PYC ;
lista_id  : lista_id COMA ID
          | ID ;

sentencia_salida : OUTPUT lista_expresion_cadena PYC ;

lista_expresion_cadena  : lista_expresion_cadena COMA expresion_cadena
                        | expresion_cadena ;

expresion_cadena : expresion
                 | CADENA ;

sentencia_return : RETURN expresion PYC { comprobarReturn($2.dtipo); };

sentencia_repeat_until  : REPEAT sentencia UNTIL PARIZQ expresion PARDER PYC { expresionBooleana($5.dtipo); } ;

sentencia_lista : expresion OPERLISTA PYC { sentenciaLista($1.dtipo, $2.lexema); }
                | INT expresion PYC
                | DOLLAR expresion PYC { sentenciaLista($1.dtipo, $2.lexema); };

expresion : PARIZQ expresion PARDER { $$.dtipo = $2.dtipo; }
          | MASMENOS expresion %prec NOT { $$.dtipo = masMenos($1.atributo, $2.dtipo); }
          | NOT expresion { $$.dtipo = not($2.dtipo); } 
          | INT expresion { $$.dtipo = interrogacion($2.dtipo); }
          | MENOSMENOS expresion
          | expresion MENOSMENOS expresion
          | expresion ORLOG expresion { $$.dtipo = orLog($1.dtipo, $3.dtipo); }
          | expresion ANDLOG expresion { $$.dtipo = andLog($1.dtipo, $3.dtipo); }
          | expresion ARROBA expresion  { $$.dtipo = arroba($1.dtipo, $3.dtipo); }
          | expresion XOR expresion
          | expresion REL expresion { $$.dtipo = rel($1.dtipo, $2.atributo, $3.dtipo); }
          | expresion MASMENOS expresion { $$.dtipo = masmenos($1.dtipo, $2.atributo, $3.dtipo); }
          | expresion PORPOR expresion { $$.dtipo = porPor($1.dtipo, $3.dtipo); }
          | expresion PORPOT expresion
          | expresion MULDIV expresion { $$.dtipo = porDiv($1.dtipo, $2.atributo, $3.dtipo); }
          | expresion EQN expresion { $$.dtipo = eqn($1.dtipo, $2.atributo, $3.dtipo); }
          | expresion OPERMASMAS expresion ARROBA expresion { $$.dtipo = ternario($1.dtipo, $3.dtipo, $5.dtipo); } 
          | ID { $$.dtipo = buscarID($1.lexema); }
          | constante { $$.dtipo = $1.dtipo; }
          | funcion  { $$.dtipo = $1.dtipo; }
          | error ;

funcion : ID PARIZQ argumentos PARDER { $$.dtipo = comprobarFuncion($1.lexema); };

argumentos : lista_expresiones
           | %empty ;
           
constante : CONSTANTE { $$.dtipo = $1.dtipo; }
          | lista { $$.dtipo = $1.dtipo; };
          
lista : CORCHETEIZQ lista_expresiones CORCHETEDER { $$.dtipo = aTipoLista($2.dtipo); }
      | CORCHETEIZQ CORCHETEDER ;
%%

#include "lex.yy.c"

void yyerror(const char *msg){
  fprintf(stderr, "[Linea %d]: %s\n", yylineno, msg);
}

int main(){
  yyparse();

  return 0;
}








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
%token ESTRUCTURA
%token ID
/* Ternario */
%right OPERMASMAS ARROBA

/* Or lógico */
%left ORLOG 

/* AND lógico */
%left ANDLOG 

/* XOR lógico */
%left XOR 

/* Igual distinto */
%left EQN

/* Relacional */
%left REL

%left PORPOR

%left PORPOT

/* Sum rest */
%left MASMENOS

/* Multiplicación división */
%left MULDIV

/* Menos menos */
%right MENOSMENOS

%right NOT

%precedence INT DOLLAR BAR
%%

programa : MAIN PARIZQ PARDER bloque ;

bloque : INICIOBLOQUE { insertarMarca(); }  
	 declar_de_variables_locales 
	 declar_de_subprogs sentencias 
	 FINBLOQUE { vaciarEntradas(); };


declar_de_subprogs : declar_de_subprogs declar_subprog
                   | %empty ;

declar_subprog : cabecera_subprog { subProg = 1; } bloque ;

declar_de_variables_locales : VAR INICIOBLOQUE variables_locales FINBLOQUE
                            | %empty ;

variables_locales : variables_locales cuerpo_declar_variables
                  | cuerpo_declar_variables ;

cuerpo_declar_variables : tipo { tipoTmp = $1.dtipo; } lista_variables PYC
                        | error ;
tipo  : PRIMITIVO
      | ESTRUCTURA tipo;

lista_variables : ID COMA lista_variables { insertarVariable($1.lexema); }
                | ID { insertarVariable($1.lexema); };

cabecera_subprog : tipo ID { insertarFuncion($1.dtipo, $2.lexema); } PARIZQ parametros PARDER 
                 | error ;


lista_expresiones : lista_expresiones COMA expresion {
                    argumentos_tipo_datos[n_argumentos] = $3.dtipo;
                    n_argumentos++;
                  }
                  | expresion {
                    argumentos_tipo_datos[n_argumentos] = $1.dtipo;
                    n_argumentos++;
                  };

parametros : parametros COMA tipo ID { insertarParametro($1.dtipo, $2.lexema); }
           | tipo ID { insertarParametro($1.dtipo, $2.lexema); }
           | error;

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

sentencia_asignacion : ID ASIGN expresion PYC { comprobarAsignacion($1.lexema, $3.dtipo); };

sentencia_if : IF PARIZQ expresion PARDER sentencia sentencia_else  {expresionBooleana($3.dtipo); } ;

sentencia_else  : ELSE sentencia
                | %empty ;

sentencia_while : WHILE PARIZQ expresion PARDER sentencia { expresionBooleana($3.dtipo); } ;

sentencia_entrada : INPUT lista_id PYC ;
lista_id  : lista_id COMA ID
          | ID ;

sentencia_salida : OUTPUT lista_expresion_cadena PYC ;

lista_expresion_cadena  : lista_expresion_cadena COMA expresion_cadena
                        | expresion_cadena ;

expresion_cadena : expresion
                 | CADENA ;

sentencia_return : RETURN expresion PYC { comprobarReturn($2.dtipo); };

sentencia_repeat_until  : REPEAT sentencia UNTIL PARIZQ expresion PARDER PYC { expresionBooleana($5.dtipo); } ;

sentencia_lista : expresion OPERLISTA PYC { sentenciaLista($1.dtipo, $2.lexema); }
                | INT expresion PYC
                | DOLLAR expresion PYC { sentenciaLista($1.dtipo, $2.lexema); };

expresion : PARIZQ expresion PARDER { $$.dtipo = $2.dtipo; }
          | MASMENOS expresion %prec NOT { $$.dtipo = masMenos($1.atributo, $2.dtipo); }
          | NOT expresion { $$.dtipo = not($2.dtipo); } 
          | INT expresion { $$.dtipo = interrogacion($2.dtipo); }
          | MENOSMENOS expresion
          | expresion MENOSMENOS expresion
          | expresion ORLOG expresion { $$.dtipo = orLog($1.dtipo, $3.dtipo); }
          | expresion ANDLOG expresion { $$.dtipo = andLog($1.dtipo, $3.dtipo); }
          | expresion ARROBA expresion  { $$.dtipo = arroba($1.dtipo, $3.dtipo); }
          | expresion XOR expresion
          | expresion REL expresion { $$.dtipo = rel($1.dtipo, $2.atributo, $3.dtipo); }
          | expresion MASMENOS expresion { $$.dtipo = masmenos($1.dtipo, $2.atributo, $3.dtipo); }
          | expresion PORPOR expresion { $$.dtipo = porPor($1.dtipo, $3.dtipo); }
          | expresion PORPOT expresion
          | expresion MULDIV expresion { $$.dtipo = porDiv($1.dtipo, $2.atributo, $3.dtipo); }
          | expresion EQN expresion { $$.dtipo = eqn($1.dtipo, $2.atributo, $3.dtipo); }
          | expresion OPERMASMAS expresion ARROBA expresion { $$.dtipo = ternario($1.dtipo, $3.dtipo, $5.dtipo); } 
          | ID { $$.dtipo = buscarID($1.lexema); }
          | constante { $$.dtipo = $1.dtipo; }
          | funcion  { $$.dtipo = $1.dtipo; }
          | error ;

funcion : ID PARIZQ argumentos PARDER { $$.dtipo = comprobarFuncion($1.lexema); };

argumentos : lista_expresiones
           | %empty ;
           
constante : CONSTANTE { $$.dtipo = $1.dtipo; }
          | lista { $$.dtipo = $1.dtipo; };
          
lista : CORCHETEIZQ lista_expresiones CORCHETEDER { $$.dtipo = aTipoLista($2.dtipo); }
      | CORCHETEIZQ CORCHETEDER ;

















/**

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

/* Or lógico */
%left ORLOG 

/* AND lógico */
%left ANDLOG 

/* XOR lógico */
%left XOR 

/* Igual distinto */
%left EQN

/* Relacional */
%left REL

%left PORPOR

%left PORPOT

/* Sum rest */
%left MASMENOS

/* Multiplicación división */
%left MULDIV

/* Menos menos */
%right MENOSMENOS

%right NOT

%precedence INT BAR DOLLAR 
%%

programa : MAIN PARIZQ PARDER bloque ;

bloque : INICIOBLOQUE { insertarMarca(); } 
	 declar_de_variables_locales 
	 declar_de_subprogs 
	 sentencias 
	 FINBLOQUE { vaciarEntradas(); };


declar_de_subprogs : declar_de_subprogs declar_subprog
                   | %empty ;

declar_subprog : cabecera_subprog { subProg = 1; } bloque ;

declar_de_variables_locales : VAR INICIOBLOQUE variables_locales FINBLOQUE
                            | %empty ;

variables_locales : variables_locales cuerpo_declar_variables
                  | cuerpo_declar_variables ;

// TODO  Comprobar muy bien esta REGLA
cuerpo_declar_variables : tipo { tipoTmp = $1.dtipo; } lista_variables PYC
                        | error ;
tipo  : PRIMITIVO
      | ESTRUCTURA tipo;


lista_variables : ID COMA lista_variables  { insertarVariable($1.lexema); }
                | ID { insertarVariable($1.lexema); };

cabecera_subprog : tipo ID { insertarFuncion($1.dtipo, $2.lexema); } PARIZQ parametros PARDER 
                 | error;

// TODO  Revisar con cuidado
lista_expresiones : lista_expresiones COMA expresion {
                    argumentos_tipo_datos[n_argumentos] = $3.dtipo;
                    n_argumentos++;
                  }
                  | expresion {
                    argumentos_tipo_datos[n_argumentos] = $1.dtipo;
                    n_argumentos++;
                  };


// TODO  Repasar por si el error está aquí
parametros : parametros COMA tipo ID { insertarParametro($1.dtipo, $2.lexema); }
           | tipo ID { insertarParametro($1.dtipo, $2.lexema); }
           | error;

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

sentencia_asignacion : ID ASIGN expresion PYC { comprobarAsignacion($1.lexema, $3.dtipo); };

sentencia_if : IF PARIZQ expresion PARDER sentencia sentencia_else  {expresionBooleana($3.dtipo); } ;

sentencia_else  : ELSE sentencia
                | %empty ;

sentencia_while : WHILE PARIZQ expresion PARDER sentencia { expresionBooleana($3.dtipo); } ;

sentencia_entrada : INPUT lista_id PYC ;
lista_id  : lista_id COMA ID
          | ID ;

sentencia_salida : OUTPUT lista_expresion_cadena PYC ;

lista_expresion_cadena  : lista_expresion_cadena COMA expresion_cadena
                        | expresion_cadena ;

expresion_cadena : expresion
                 | CADENA ;

sentencia_return : RETURN expresion PYC { comprobarReturn($2.dtipo); };


// TODO Hay que adaptarlo para REPEAT-UNTIL
sentencia_repeat_until  : REPEAT sentencia UNTIL PARIZQ expresion PARDER PYC { expresionBooleana($5.dtipo); };

// TODO  Revisar estas funciones porque nuestras listas son diferentes
sentencia_lista : expresion OPERLISTA PYC { sentenciaLista($1.dtipo, $2.lexema); }
                | INT expresion PYC
                | DOLLAR expresion PYC { sentenciaLista($2.dtipo, $1.lexema); };


// TODO  Revisar todas las funciones, para nosotros NOT==EXCL
expresion : PARIZQ expresion PARDER        { $$.dtipo = $2.dtipo; }
          | MASMENOS expresion %prec NOT   { $$.dtipo = masMenos($1.atributo, $2.dtipo); } // TODO  Revisar not---EXCL
          | NOT expresion                  { $$.dtipo = not($2.dtipo); } // TODO Revisar not -- EXCL
          | INT expresion                  { $$.dtipo = interrogacion($2.dtipo); }
          | MENOSMENOS expresion
          | expresion MENOSMENOS expresion // TODO  Hacer funcion para el MENOSMENOS
          | expresion ORLOG expresion     { $$.dtipo = orLog($1.dtipo, $3.dtipo); } // TODO Debería estar bien esta función, pero revisar
          | expresion ANDLOG expresion    { $$.dtipo = andLog($1.dtipo, $3.dtipo); } // TODO Debería estar bien esta función, pero revisar
          | expresion ARROBA expresion    { $$.dtipo = arroba($1.dtipo, $3.dtipo); } // TODO Cambiar at por arroba y revisar
          | expresion XOR expresion       // TODO Crear una función para XOR
          | expresion REL expresion       { $$.dtipo = rel($1.dtipo, $2.atributo, $3.dtipo); } // TODO Debería estar bien
          | expresion MASMENOS expresion  { $$.dtipo = masmenos($1.dtipo, $2.atributo, $3.dtipo); } // TODO Revisar y adaptar nombre a MASMENOS
          | expresion PORPOR expresion    { $$.dtipo = porPor($1.dtipo, $3.dtipo); } // TODO Debería estar bien 
          | expresion PORPOT expresion    // TODO Hacer función para PORPOT
          | expresion MULDIV expresion    { $$.dtipo = porDiv($1.dtipo, $2.atributo, $3.dtipo); } // TODO Debería estar bien
          | expresion EQN expresion       { $$.dtipo = eqn($1.dtipo, $2.atributo, $3.dtipo); } // TODO Debería estar bien
          | expresion OPERMASMAS expresion ARROBA expresion { $$.dtipo = ternario($1.dtipo, $3.dtipo, $5.dtipo); } // TODO Deberia estar bien
          | ID                            { $$.dtipo = buscarID($1.lexema); } // TODO Debería estar bien
          | constante                     { $$.dtipo = $1.dtipo; } // TODO Debería estar bien
          | funcion 			   { $$.dtipo = $1.dtipo; } // TODO Debería estar bien  
          | error;                

// TODO Revisar
funcion : ID PARIZQ argumentos PARDER { $$.dtipo = comprobarFuncion($1.lexema); } ;

argumentos : lista_expresiones
           | %empty ;

// TODO Revisar           
constante : CONSTANTE { $$.dtipo = $1.dtipo; }
          | lista { $$.dtipo = $1.dtipo; };

// TODO Revisar         
lista : CORCHETEIZQ lista_expresiones CORCHETEDER { $$.dtipo = aTipoLista($2.dtipo); }
      | CORCHETEIZQ CORCHETEDER ;
      
      
// TODO  Cuidado con el lista_expresiones de ellos, que aquí al final faltan cosas

*/
