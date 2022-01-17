/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.5.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "analizador_semantico.y"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define YYERROR_VERBOSE

FILE* yyout;

#define min(a, b) ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a < _b ? _a : _b; })
#define max(a, b) ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a > _b ? _a : _b; })

void yyerror( const char * msg );

#define MAX_TAM_TS 500

extern int yylineno;


char msgError[512];

// Esto elimina un Warning, no debería cambiar nada más.
int yylex();

/************************/
/* ESTRUCTURA DE LA TS */
/***********************/

// Para bucles if
typedef struct {
  char* etiquetaEntrada;
  char* etiquetaSalida;
  char* etiquetaElse;
} DescriptorDeInstrControl;

// Tipo de entrada
typedef enum {
  marca,
  funcion,
  variable,
  parametroFormal,
  descriptor
} TipoEntrada;

// Si TipoEntrada es función, variable, o parametro-formal; indica el tipo de dato
// No se usa en otro caso
typedef enum {
  entero,
  real,
  booleano,
  caracter,
  cadena, // Dato reservado a cadenas, no se puede declarar
  listaEntero,
  listaReal,
  listaBooleano,
  listaCaracter,
  error // Si da un error con expresiones
} TipoDato;

typedef struct {
  TipoEntrada tipoEntrada;    // Tipo de entrada
  char* nombre;               // Nombre del identificador (no se usa con marca)
  TipoDato tipoDato;          // Tipo de dato
  int parametros;             // Nº de parámetros formales (sólo se usa con función)
  DescriptorDeInstrControl* descriptor; // Descriptor de control (bucles IF - intermedio)
} EntradaTS;

// La Tabla de Símbolos
EntradaTS ts[MAX_TAM_TS];

// Última entrada de la TS usada.
long int tope = -1;

// Tipo auxiliar para declaración de variables
TipoDato tipoTmp;

// Si entramos en un bloque de un subprograma
// Si es 0 es un bloque de un subprograma; en caso contrario no
int subProg = 0;

// Variables usadas para pasar argumentos a una función
#define MAX_ARGS 50
TipoDato argumentos_tipo_datos[MAX_ARGS];
int n_argumentos = 0;

typedef struct {
  int atributo;
  char* lexema;
  TipoDato dtipo;
} Atributos;

char* tipoAString(TipoDato tipo_dato) {
  switch (tipo_dato) {
    case real:
      return "float";
    case entero:
      return "int";
    case booleano:
      return "bool";
    case caracter:
      return "char";
    case listaReal:
      return "list_of float";
    case listaEntero:
      return "list_of int";
    case listaCaracter:
      return "list_of char";
    case listaBooleano:
      return "list_of bool";
    case error:
      return "error";
    default:
      fprintf(stderr, "Error en tipoAString(), no se conoce el tipo dato\n");
      exit(EXIT_FAILURE);
  }
}

TipoDato tipoLista(TipoDato tipo_dato) {
  switch (tipo_dato) {
    case listaEntero:
      return entero;
    case listaCaracter:
      return caracter;
    case listaBooleano:
      return booleano;
    case listaReal:
      return real;
    case error:
      return error;
    default:
      fprintf(stderr, "Error en tipoLista(), tipo no es lista.\n");
      exit(EXIT_FAILURE);
  }
}

TipoDato aTipoLista(TipoDato td) {
  switch (td) {
    case entero:
      return listaEntero;
    case real:
      return listaReal;
    case caracter:
      return listaCaracter;
    case booleano:
      return listaBooleano;
    case error:
      return error;
    default:
      fprintf(stderr, "Error en tipoLista(), tipo no es lista.\n");
      exit(EXIT_FAILURE);
  }
}

//TODO Revisar
TipoDato tipoOp(TipoDato td, char * op) {
  if (!strcmp(op, "+") || !strcmp(op, "-") || !strcmp(op, "*") || !strcmp(op, "/") ||
        !strcmp(op, "**") || !strcmp(op, "--") || !strcmp(op, "%"))
    return td;

  if (!strcmp(op, "!") || !strcmp(op, "&&") || !strcmp(op, "||") || !strcmp(op, ">") || !strcmp(op, "<") ||
        !strcmp(op, ">=") || !strcmp(op, "<=") || !strcmp(op, "!=") || !strcmp(op, "=="))
    return booleano;

  if (!strcmp(op, "#"))
    return entero;

  if (!strcmp(op, "?") || !strcmp(op, "@"))
    return tipoLista(td);
}

int esLista(TipoDato tipo_dato){
  return tipo_dato == listaEntero || tipo_dato == listaReal || tipo_dato == listaBooleano || tipo_dato == listaCaracter;
}

int esNumero(TipoDato tipo_dato){
  return tipo_dato == entero || tipo_dato == real;
}

void imprimir() {
  for (int i = 0; i <= tope; ++i) {
    printf("[%i]: ", i);
    switch(ts[i].tipoEntrada) {
      case variable:
        printf("Variable %s, tipo: %s\n", ts[i].nombre,
            tipoAString(ts[i].tipoDato));
        break;
      case funcion:
        printf("Funcion %s, tipo: %s, nº parametros: %i\n", ts[i].nombre,
            tipoAString(ts[i].tipoDato), ts[i].parametros);
        break;
      case marca:
        printf("Marca\n");
        break;
      case parametroFormal:
        printf("Parametro formal %s, tipo: %s\n", ts[i].nombre,
            tipoAString(ts[i].tipoDato));
        break;
      default:
        fprintf(stderr, "Error en imprimir(), no debería salir\n");
        exit(EXIT_FAILURE);
    }
  }
}

void idRepetida(char* id) {
  // Miramos si id estaba declarado después de la última marca
  int repetida = 0;
  for (int i = tope; !repetida && ts[i].tipoEntrada != marca; --i) {
    if (ts[i].tipoEntrada != parametroFormal && !strcmp(ts[i].nombre, id)) {
      sprintf(msgError, "ERROR SINTÁCTICO: identificador %s ya declarado\n", id);
      yyerror(msgError);
      repetida = 1;
    }
  }
}

void insertarEntrada(TipoEntrada te, char* nombre, TipoDato tipo_dato, int nParam, DescriptorDeInstrControl* descp) {
  // Hacemos la entrada
  EntradaTS entrada = {
    te,
    strdup(nombre),
    tipo_dato,
    nParam,
    descp
  };

  // Si la tabla está llena da error
  if (tope + 1 >= MAX_TAM_TS) {
    sprintf(msgError, "ERROR SINTÁCTICO: La tabla de símbolos está llena\n");
    yyerror(msgError);
  }
  // Aumentamos el tope
  ++tope;
  // Añadimos la nueva entrada
  ts[tope] = entrada;
}

// Busca una entrada en la TS con el id especificado en el ámbito del programa
// actual. Si no lo encuentra, devuelve -1. No gestiona errores!
int buscarEntrada(char* id) {
  int i = tope;
  while(i >= 0 && (ts[i].tipoEntrada == parametroFormal || strcmp(id, ts[i].nombre)))
    --i;

  if (i < 0) {
    sprintf(msgError, "ERROR SINTÁCTICO: identificador %s no declarado\n", id);
    yyerror(msgError);
  }
  return i;
}

/****************/
/* FUNCIONES TS */
/****************/

void insertarMarca() {
  // Metemos la marca
  insertarEntrada(marca, "", -1, -1, NULL);
  // Si es subprograma añadimos las variables al bloque
  if (subProg) {
    for (int i = tope - 1; ts[i].tipoEntrada != funcion; --i) {
      insertarEntrada(variable, ts[i].nombre, ts[i].tipoDato, -1, NULL);
    }
    subProg = 0;
  }
}

void vaciarEntradas() {
  // Hasta la última marca borramos todo
  while (ts[tope].tipoEntrada != marca)
    --tope;
  // Elimina la última marca
  --tope;
}

void insertarVariable(char* id) {
  // Comprobamos que no esté repetida la id
  idRepetida(id);
  insertarEntrada(variable, id, tipoTmp, -1, NULL);
}

void insertarFuncion(TipoDato tipoDato, char* id) {
  // Comprobamos que el id no esté usado ya
  idRepetida(id);
  insertarEntrada(funcion, id, tipoDato, 0, NULL);
}

//TODO Revisar
void insertarDescriptor(char* etqEntrada, char* etqSalida, char* etqElse) {
  DescriptorDeInstrControl* descp = (DescriptorDeInstrControl*) malloc(sizeof(DescriptorDeInstrControl));
  descp->etiquetaEntrada = strdup(etqEntrada);
  descp->etiquetaSalida = strdup(etqSalida);
  descp->etiquetaElse = strdup(etqElse);
  insertarEntrada(descriptor, "", -1, -1, descp);
}


void insertarParametro(TipoDato tipoDato, char* id) {
  // Comprobamos que no haya parámetros con nombres repetidos
  // Además guardamos el índice de la función
  int i;
  int parametroRepetido = 0;
  for (i = tope; !parametroRepetido && ts[i].tipoEntrada != funcion; --i) {
    if (!strcmp(ts[i].nombre, id)) {
      sprintf(msgError, "ERROR SINTÁCTICO: identificador del parámetro %s ya declarado\n", id);
      yyerror(msgError);
      parametroRepetido = 1;
    }
  }
  // Añadimos la entrada
  insertarEntrada(parametroFormal, id, tipoDato, -1, NULL);
  // Actualizamos el nº de parámetros de la función
  ++ts[i].parametros;
}

TipoDato buscarID(char* id) {
  int i = buscarEntrada(id);

  if (i < 0)
    return error;
  return ts[i].tipoDato;
}

void comprobarAsignacion(char* id, TipoDato td) {
  int i = buscarEntrada(id);
  if (i >= 0) {
    if (ts[i].tipoEntrada != variable) {
      sprintf(msgError, "ERROR SINTÁCTICO: se intenta asignar a %s, y no es una variable\n", id);
      yyerror(msgError);
    } else {
      if (td != error && td != ts[i].tipoDato) {
        sprintf(msgError, "ERROR SINTÁCTICO: asignación incorrecta, %s es tipo %s y se obtuvo %s\n",
            id, tipoAString(td), tipoAString(ts[i].tipoDato));
        yyerror(msgError);
      }
    }
  }
}

void expresionBooleana(TipoDato td) {
  if (td != error && td != booleano) {
    sprintf(msgError, "ERROR SINTÁCTICO: condición no es de tipo booleano, se tiene tipo %s",
        tipoAString(td));
    yyerror(msgError);
  }
}

void sentenciaLista(TipoDato td, char* sentencia) {
  if (td != error && !esLista(td)) {
    sprintf(msgError, "ERROR SINTÁCTICO: sentencia %s no aplicable al tipo %s\n",
        sentencia, tipoAString(td));
    yyerror(msgError);
  }
}

TipoDato mismoTipoLista(TipoDato t1, TipoDato t2) {

  if (t1 == error || t2 == error)
    return error;

  if (t1 != t2) {
    sprintf(msgError, "ERROR SINTÁCTICO: lista con dos tipos distintos %s y %s\n",
        tipoAString(t1), tipoAString(t2));
    yyerror(msgError);
    return error;
  }
  return t1;
}

TipoDato masMenos(int atr, TipoDato td) {
  if (td == error)
    return error;

  char* operador = atr ? "-" : "+";
  if (!esNumero(td)) {
    sprintf(msgError, "ERROR SINTÁCTICO: operador %s no aplicable al tipo %s\n",
        operador, tipoAString(td));
    yyerror(msgError);
    return error;
  }
  return td;
}

TipoDato excl(TipoDato td) {
  if (td == error)
    return error;
  if (td != booleano) {
    sprintf(msgError, "ERROR SINTÁCTICO: operador ! no aplicable al tipo %s\n",
        tipoAString(td));
    yyerror(msgError);
    return error;
  }
  return booleano;
}

TipoDato intHash(int atr, TipoDato td) {
  if (td == error)
    return error;

  char* operador = atr ? "#" : "?";
  if (!esLista(td)) {
    sprintf(msgError, "ERROR SINTÁCTICO: operador %s no aplicable al tipo %s\n",
        operador, tipoAString(td));
    yyerror(msgError);
    return error;
  }

  if (atr)
    return tipoLista(td);
  else
    return entero;
}

TipoDato at(TipoDato td1, TipoDato td2) {
  if (td1 == error || td2 == error)
    return error;

  if (!esLista(td1) || td2 != entero) {
    sprintf(msgError, "ERROR SINTÁCTICO: operador @ no aplicable a los tipos %s, %s\n",
        tipoAString(td1), tipoAString(td2));
    yyerror(msgError);
    return error;
  }

  return tipoLista(td1);
}

TipoDato andLog(TipoDato td1, TipoDato td2) {
  if (td1 == error || td2 == error)
    return error;

  if (td1 != booleano || td2 != booleano) {
    sprintf(msgError, "ERROR SINTÁCTICO: operador && no aplicable a los tipos %s, %s\n",
        tipoAString(td1), tipoAString(td2));
    yyerror(msgError);
    return error;
  }

  return booleano;
}

TipoDato orLog(TipoDato td1, TipoDato td2) {
  if (td1 == error || td2 == error)
    return error;

  if (td1 != booleano || td2 != booleano) {
    sprintf(msgError, "ERROR SINTÁCTICO: operador || no aplicable a los tipos %s, %s\n",
        tipoAString(td1), tipoAString(td2));
    yyerror(msgError);
    return error;
  }

  return booleano;
}

TipoDato eqn(TipoDato td1, int atr, TipoDato td2) {
  if (td1 == error || td2 == error)
    return error;

  char* operador = atr ? "!=" : "==";
  if (td1 != td2) {
    sprintf(msgError, "ERROR SINTÁCTICO: operador %s no aplicable a los tipos %s, %s\n",
        operador, tipoAString(td1), tipoAString(td2));
    yyerror(msgError);
    return error;
  }

  return booleano;
}

// Comprueba el tipo de la operación binaria realizada. En caso de error, lo
// gestiona. En caso contrario, devuelve el tipo tras la operación binaria.
// IMPORTANTE: Se asume que op1 esta asociado al valor 1 del atributo (atr)
// mientras que op2 está asociado al valor 0.
// IMPORTANE: Se asume que el op1 es simétrico y que el op2 no es simétrico y
// unicamente funciona de la forma "T op2 T" o bien "list_of T op2 T".
TipoDato op2Binario(TipoDato td1, int atr, TipoDato td2, char* op1, char* op2) {
  if (td1 == error || td2 == error)
    return error;

  char* operador = atr ? op1 : op2;
  int l1 = esLista(td1);
  int l2 = esLista(td2);
  TipoDato tipo1 = l1 ? tipoLista(td1) : td1;
  TipoDato tipo2 = l2 ? tipoLista(td2) : td2;

  int op_error = (l1 && l2) ||
              (tipo1 != tipo2) ||
              !esNumero(tipo1);

  TipoDato resultado = td1;

  if (!op_error && (l1 || l2) ) {
    // Llegado a este punto hay exactamente una lista. Vemos si el tipo de
    // la lista encaja con el tipo del otro atributo:
    if ( (operador == op2) || l1 ) {
      resultado = l1 ? td1 : td2;
    } else {
      op_error = 1;
    }
  }

  if (op_error) {
    sprintf(msgError, "ERROR SINTÁCTICO: operador %s no aplicable a los tipos %s, %s\n",
        operador, tipoAString(td1), tipoAString(td2));
    yyerror(msgError);
    return error;
  }
  return resultado;
}

TipoDato addSub(TipoDato td1, int atr, TipoDato td2) {
  return op2Binario(td1, atr, td2, "-", "+");
}

TipoDato porDiv(TipoDato td1, int atr, TipoDato td2) {
  return op2Binario(td1, atr, td2, "/", "*");
}

TipoDato porPor(TipoDato td1, TipoDato td2) {
  if (td1 == error || td2 == error)
    return error;

  if (td1 != td2 || !esLista(td1) || !esLista(td2)) {
    sprintf(msgError, "ERROR SINTÁCTICO: operador ** no aplicable a los tipos %s, %s\n",
        tipoAString(td1), tipoAString(td2));
    yyerror(msgError);
    return error;
  }

  return td1;
}

TipoDato borrList(TipoDato td1, int atr, TipoDato td2) {
  if (td1 == error || td2 == error)
    return error;

  char* operador = atr ? "%" : "--";
  if (!esLista(td1) || td2 != entero) {
    sprintf(msgError, "ERROR SINTÁCTICO: operador %s no aplicable a los tipos %s, %s\n",
        operador, tipoAString(td1), tipoAString(td2));
    yyerror(msgError);
    return error;
  }

  return td1;
}

TipoDato rel(TipoDato td1, int atr, TipoDato td2) {
  if (td1 == error || td2 == error)
    return error;

  char* operador;
  switch (atr) {
    case 0:
      operador = "<";
      break;
    case 1:
      operador = ">";
      break;
    case 2:
      operador = "<=";
      break;
    case 3:
      operador = ">=";
      break;
  }

  if (td1 != td2 || !esNumero(td1) || !esNumero(td2)) {
    sprintf(msgError, "ERROR SINTÁCTICO: operador %s no aplicable a los tipos %s, %s\n",
        operador, tipoAString(td1), tipoAString(td2));
    yyerror(msgError);
    return error;
  }

  return booleano;
}

TipoDato ternario(TipoDato td1, TipoDato td2, TipoDato td3) {
  if (td1 == error || td2 == error || td3 == error)
    return error;

  if (!esLista(td1) || tipoLista(td1) != td2 || td3 != entero) {
    sprintf(msgError, "ERROR SINTÁCTICO: operador ++ @ no aplicable a los tipos %s, %s, %s\n",
        tipoAString(td1), tipoAString(td2), tipoAString(td3));
    yyerror(msgError);
    return error;
  }
  return td1;
}

void comprobarReturn(TipoDato td) {
  int i = tope;
  int marcaEncontrada = 0;
  int funcionEncontrada = 0;

  while (i >= 1 && !funcionEncontrada) {
    funcionEncontrada = marcaEncontrada && ts[i].tipoEntrada == funcion;
    marcaEncontrada = (!marcaEncontrada && ts[i].tipoEntrada == marca) ||
                      (marcaEncontrada && ts[i].tipoEntrada == parametroFormal);
    --i;
  }

  if (funcionEncontrada) ++i;

  if (i <= 0) {
    sprintf(msgError, "ERROR SINTÁCTICO: return no asignado a ninguna función\n");
    yyerror(msgError);
  } else if (td != error && td != ts[i].tipoDato) {
    sprintf(msgError, "ERROR SINTÁCTICO: return devuelve tipo %s, y función es de tipo %s\n",
        tipoAString(td), tipoAString(ts[i].tipoDato));
    yyerror(msgError);
  }
}

TipoDato comprobarFuncion(char* id) {
  int iFuncion = buscarEntrada(id);

  if (iFuncion < 0)
    return error;

  if (ts[iFuncion].tipoEntrada != funcion) {
    sprintf(msgError, "ERROR SINTÁCTICO: %s no es una función y no puede ser llamada\n", id);
    yyerror(msgError);
    return error;
  }

  int n_argumentos_esperados = ts[iFuncion].parametros;

  if ( n_argumentos != n_argumentos_esperados ) {
    sprintf(msgError, "ERROR SINTÁCTICO: número de argumentos errónea al llamar a la función %s. Esperados: %d, encontrados: %d\n",
        id, n_argumentos_esperados, n_argumentos);
    yyerror(msgError);
  }

  n_argumentos = min(n_argumentos, n_argumentos_esperados);

  for (int i = 0; i < n_argumentos; i++){
    TipoDato tipoEsperado = ts[iFuncion + i + 1].tipoDato;
    TipoDato tipoObtenido = argumentos_tipo_datos[i];
    if (tipoObtenido != error && tipoEsperado != tipoObtenido){
      sprintf(msgError, "ERROR SINTÁCTICO: argumento número %d de tipo erróneo al llamar a la función %s. Esperado: %s, encontrado: %s\n",
          i, id, tipoAString(tipoEsperado), tipoAString(tipoObtenido));
      yyerror(msgError);
    }
  }

  // De esta forma mostramos el máximo número de errores posibles.

  // Borramos los argumentos recibidos.
  n_argumentos = 0;

  // Devolvemos el tipo de la función.
  return ts[iFuncion].tipoDato;
}

// TODO REVISAR SECCION NUEVA
// **********************************
// ** Generación código intermedio **
// **********************************

int hayError = 0;
int deep = 0;
FILE * fMain;
FILE * fFunc;

#define addTab() { for (int i = 0; i < deep - (yyout != fMain); ++i) fprintf(yyout, "\t"); }
#define gen(f_, ...) { if (!hayError) {addTab(); fprintf(yyout, f_, ##__VA_ARGS__); fflush(yyout);} }

char* temporal() {
  static int indice = 1;
  char* temp = malloc(sizeof(char) * 10);
  sprintf(temp, "temp%i", indice++);
  return temp;
}

char* etiqueta() {
  static int indice = 1;
  char* etiqueta = malloc(sizeof(char) * 14);
  sprintf(etiqueta, "etiqueta%i", indice++);
  return etiqueta;
}

char* tipoIntermedio(TipoDato td) {
  if (esLista(td))
    return "Lista";
  else if (td == booleano)
    return "int";
  else
    return tipoAString(td);
}

char* leerOp(TipoDato td1, char* exp1, char* op, char* exp2, TipoDato td2) {
  char* etiqueta = temporal();
  TipoDato tdPrimario = td1;
  char* expPrimaria = exp1;
  char* expSecundaria = exp2;
  if (esLista(td2) && (!strcmp("+", op) || !strcmp("*", op))) {
    tdPrimario = td2;
    expPrimaria = exp2;
    expSecundaria = exp1;
  }


  gen("%s %s;\n", tipoIntermedio(tipoOp(tdPrimario, op)), etiqueta);

  if (!strcmp("#", op)) {
    gen("%s = getTam(%s);\n", etiqueta, exp1);
  } else if (!strcmp("?", op)) {
    gen("%s = *(%s*)getActual(%s);\n", etiqueta, tipoIntermedio(tipoLista(td1)), exp1);
  } else if (!strcmp("@", op)) {
    gen("%s = *(%s*)get(%s, %s);\n", etiqueta, tipoIntermedio(tipoLista(td1)), exp1, exp2);
  } else if (!strcmp("--", op)) {
    gen("%s = borrarEn(%s, %s);\n", etiqueta, exp1, exp2);
  } else if (!strcmp("%", op)) {
    gen("%s = borrarAPartirDe(%s, %s);\n", etiqueta, exp1, exp2);
  } else if (!strcmp("**", op)) {
    gen("%s = concatenar(%s, %s);\n", etiqueta, exp1, exp2);
  } else if (esLista(tdPrimario)) {
    if (!strcmp("+", op)) {
      gen("%s = sumarLista(%s, %s);\n", etiqueta, expPrimaria, expSecundaria)
    } else if (!strcmp("-", op)) {
        gen("%s = restarLista(%s, %s);\n", etiqueta, expPrimaria, expSecundaria);
    } else if (!strcmp("*", op)) {
      gen("%s = multiplicarLista(%s, %s);\n", etiqueta, expPrimaria, expSecundaria);
    } else if (!strcmp("/", op)) {
      gen("%s = dividirLista(%s, %s);\n", etiqueta, expPrimaria, expSecundaria);
    }
  } else if (!strcmp("", exp2)) {
    gen("%s = %s %s;\n", etiqueta, op, exp1);
  } else {
    gen("%s = %s %s %s;\n", etiqueta, exp1, op, exp2);
  }
  return etiqueta;
}

char* leerCte(char* cte, TipoDato td) {
  if (td == booleano) {
    if (!strcmp("true", cte))
      return "1";
    else
      return "0";
  }
  return cte;
}

char* insertarDato(char* id, TipoDato td) {
  char* buffer = malloc(sizeof(char) * 100);
  switch (td) {
    case entero:
    case booleano:
      sprintf(buffer, "pInt(%s)", id);
      return buffer;
    case real:
      sprintf(buffer, "pFloat(%s)", id);
      return buffer;
    case caracter:
      sprintf(buffer, "pChar(%s)", id);
      return buffer;
    default:
      if (!hayError) {
        sprintf(msgError, "ERROR INTERMEDIO: tipo no básico en insertarDato().\n");
        yyerror(msgError);
        exit(EXIT_FAILURE);
      }
  }
}

char* tipoImprimir(TipoDato tipo) {
  if (tipo == entero)
    return "%d";
  else if (tipo == real)
    return "%f";
  else if (esLista(tipo) || tipo == booleano || tipo == cadena)
    return "%s";
  else if (tipo == caracter)
    return "%c";
  else {
    if (!hayError) {
      sprintf(msgError, "ERROR INTERMEDIO: tipoImprimir() tipo no válido.\n");
      yyerror(msgError);
      exit(EXIT_FAILURE);
    }
  }
}

char* inicializaTipoLista(TipoDato tipo) {
  if (tipo == entero)
    return "tInt";
  else if (tipo == real)
    return "tFloat";
  else if (tipo == caracter)
    return "tChar";
  else if (tipo == booleano)
    return "tBool";
  else {
    if (!hayError) {
      sprintf(msgError, "ERROR INTERMEDIO: tipo no válido en inicializaTipoLista().\n");
      yyerror(msgError);
      exit(EXIT_FAILURE);
    }
  }
}

#define YYSTYPE Atributos

#line 877 "y.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    MAIN = 258,
    VAR = 259,
    TIPO = 260,
    PYC = 261,
    INPUT = 262,
    OUTPUT = 263,
    CADENA = 264,
    RETURN = 265,
    IF = 266,
    ELSE = 267,
    REPEAT = 268,
    UNTIL = 269,
    WHILE = 270,
    OPERLISTA = 271,
    CONST = 272,
    INIBLOQUE = 273,
    FINBLOQUE = 274,
    CORIZQ = 275,
    CORDER = 276,
    COMA = 277,
    ASIGN = 278,
    PARDER = 279,
    PARIZQ = 280,
    DOLLAR = 281,
    ID = 282,
    MASMAS = 283,
    ARROBA = 284,
    ORLOG = 285,
    ANDLOG = 286,
    EQN = 287,
    REL = 288,
    PORPOR = 289,
    BORRLIST = 290,
    MASMENOS = 291,
    MULDIV = 292,
    INTHASH = 293,
    EXCL = 294
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */



#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))

/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  4
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   232

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  40
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  54
/* YYNRULES -- Number of rules.  */
#define YYNRULES  99
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  169

#define YYUNDEFTOK  2
#define YYMAXUTOK   294


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   858,   858,   858,   868,   869,   870,   868,   877,   878,
     879,   881,   882,   884,   884,   886,   888,   893,   896,   897,
     899,   899,   902,   902,   907,   908,   909,   911,   915,   917,
     925,   925,   926,   929,   930,   931,   932,   933,   934,   935,
     936,   937,   938,   943,   951,   959,   967,   968,   973,   974,
     967,   986,   986,   988,   992,  1000,   992,  1017,  1025,  1026,
    1017,  1038,  1040,  1041,  1043,  1057,  1059,  1060,  1062,  1064,
    1075,  1078,  1082,  1083,  1087,  1091,  1095,  1099,  1103,  1107,
    1111,  1115,  1119,  1123,  1127,  1131,  1137,  1138,  1139,  1140,
    1144,  1150,  1151,  1153,  1159,  1166,  1167,  1169,  1171,  1176
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "MAIN", "VAR", "TIPO", "PYC", "INPUT",
  "OUTPUT", "CADENA", "RETURN", "IF", "ELSE", "REPEAT", "UNTIL", "WHILE",
  "OPERLISTA", "CONST", "INIBLOQUE", "FINBLOQUE", "CORIZQ", "CORDER",
  "COMA", "ASIGN", "PARDER", "PARIZQ", "DOLLAR", "ID", "MASMAS", "ARROBA",
  "ORLOG", "ANDLOG", "EQN", "REL", "PORPOR", "BORRLIST", "MASMENOS",
  "MULDIV", "INTHASH", "EXCL", "$accept", "programa", "$@1", "bloque",
  "$@2", "$@3", "$@4", "declar_de_variables_locales", "variables_locales",
  "cuerpo_declar_variables", "$@5", "lista_variables",
  "declar_de_subprogs", "declar_subprog", "$@6", "cabecera_subprog", "$@7",
  "cabecera_argumentos", "parametros", "parametro", "sentencias", "$@8",
  "sentencia", "sentencia_asignacion", "sentencia_lista", "sentencia_if",
  "$@9", "$@10", "$@11", "$@12", "bloque_else", "$@13", "sentencia_while",
  "$@14", "$@15", "sentencia_do_until", "$@16", "$@17", "$@18",
  "sentencia_entrada", "lista_id", "id_cin", "sentencia_salida",
  "lista_expresiones_o_cadena", "expresion_cout", "expresion_cadena",
  "sentencia_return", "expresion", "llamada_funcion", "argumentos",
  "lista_argumentos", "constante", "lista", "lista_expresiones", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294
};
# endif

#define YYPACT_NINF (-77)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-93)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
       1,     9,    18,    13,   -77,   -77,    21,   -77,   -77,    51,
      53,   -77,    19,   -77,   -77,   -77,   -77,    47,   -77,    70,
      52,   -77,   -77,    57,   -77,   -77,   -77,    64,    81,   -77,
      69,    21,    52,   -77,    65,   -77,    15,   -77,   -77,    12,
     -77,    67,    76,    72,    73,   -77,    77,   -77,    72,    72,
      72,     6,    72,    72,    72,   -77,   -77,   -77,   -77,   -77,
     -77,   -77,   -77,   -77,   -77,   111,   -77,   -77,   -77,   -77,
      68,    80,    85,   -77,   -77,    -3,   -77,   -77,    84,    -1,
     -77,   -77,   133,   122,   -77,    15,   -77,   133,    -7,   168,
     143,    72,    99,   -77,   -77,   -77,   -77,   112,    72,    72,
      72,    72,    72,    72,    72,    72,    72,    72,   -77,   -77,
     115,   -77,    67,   -77,    76,   -77,    72,   -77,    72,   -77,
      72,   -77,   -77,   154,   133,    98,   103,   -77,   178,   133,
     185,    97,    10,    33,    46,    46,   144,   -77,   -77,   -77,
     -77,   133,   107,   133,   133,   -77,   -77,    72,    72,   169,
     170,   199,   133,   133,   -77,    72,    15,    15,   133,   -77,
     -77,   200,   124,   188,   -77,   -77,   -77,    15,   -77
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     0,     1,     2,     0,     4,     3,    10,
       0,     5,     0,    19,    15,    13,     9,     0,    12,     6,
       0,     8,    11,     0,    32,    18,    20,    17,     0,    22,
      30,     0,     0,    14,     0,     7,     0,    21,    16,     0,
      89,     0,     0,     0,     0,    57,     0,    95,     0,     0,
       0,    87,     0,     0,     0,    33,    31,    35,    36,    37,
      38,    41,    39,    40,    42,     0,    86,    88,    96,    26,
       0,     0,    24,    28,    64,     0,    63,    70,    87,     0,
      67,    68,    69,     0,    46,     0,    54,    99,     0,     0,
       0,     0,     0,    73,    75,    74,    34,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    29,    23,
       0,    61,     0,    65,     0,    71,     0,    58,     0,    97,
       0,    72,    45,     0,    94,     0,    91,    44,     0,    76,
      78,    77,    79,    84,    82,    83,    80,    81,    27,    62,
      66,    47,     0,    55,    98,    43,    90,     0,     0,     0,
       0,     0,    93,    85,    48,     0,     0,     0,    59,    56,
      49,     0,    53,     0,    51,    50,    60,     0,    52
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -77,   -77,   -77,    -4,   -77,   -77,   -77,   -77,   -77,   208,
     -77,   194,   -77,   -77,   -77,   -77,   -77,   -77,   -77,   117,
     -77,   -77,   -76,   -77,   -77,   -77,   -77,   -77,   -77,   -77,
     -77,   -77,   -77,   -77,   -77,   -77,   -77,   -77,   -77,   -77,
     -77,   116,   -77,   -77,   118,   -77,   -77,   -42,   -77,   -77,
     -77,   -77,   -77,   -77
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,     6,    55,     9,    13,    24,    11,    17,    18,
      20,    28,    19,    25,    31,    26,    34,    71,    72,    73,
      30,    36,    56,    57,    58,    59,   116,   149,   157,   162,
     165,   167,    60,   118,   151,    61,    85,   142,   161,    62,
      75,    76,    63,    79,    80,    81,    64,    65,    66,   125,
     126,    67,    68,    88
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      82,    83,     8,   111,     1,   113,    87,    89,    90,   117,
      93,    94,    95,    69,   119,   120,    40,    70,     4,   112,
      14,   114,    41,    42,    15,    43,    44,    37,    45,    91,
      46,    92,    47,     7,     3,    48,   -25,     5,    16,     7,
      49,    50,    51,   103,   104,   105,   106,   107,    14,   123,
     124,    52,    15,    53,    54,    10,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,    21,   104,   105,   106,
     107,    12,    82,    40,   141,    23,   143,    40,   144,    27,
     159,   160,   106,   107,    29,    77,    32,    33,    35,    47,
      39,   168,    48,    47,    74,   108,    48,    49,    84,    78,
      40,    49,    86,    78,   109,   152,   153,   110,    52,    92,
      53,    54,    52,   158,    53,    54,    47,    96,   127,    48,
      70,   150,   146,   -92,    49,   147,    78,    97,   115,   102,
     103,   104,   105,   106,   107,    52,   164,    53,    54,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   122,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     145,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   107,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   121,   154,   166,   155,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,    98,   148,   100,   101,
     102,   103,   104,   105,   106,   107,   101,   102,   103,   104,
     105,   106,   107,   156,   163,    22,    38,   138,   139,     0,
       0,     0,   140
};

static const yytype_int16 yycheck[] =
{
      42,    43,     6,     6,     3,     6,    48,    49,    50,    85,
      52,    53,    54,     1,    21,    22,     1,     5,     0,    22,
       1,    22,     7,     8,     5,    10,    11,    31,    13,    23,
      15,    25,    17,    18,    25,    20,    24,    24,    19,    18,
      25,    26,    27,    33,    34,    35,    36,    37,     1,    91,
      92,    36,     5,    38,    39,     4,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,    19,    34,    35,    36,
      37,    18,   114,     1,   116,     5,   118,     1,   120,    27,
     156,   157,    36,    37,    27,     9,    22,     6,    19,    17,
      25,   167,    20,    17,    27,    27,    20,    25,    25,    27,
       1,    25,    25,    27,    24,   147,   148,    22,    36,    25,
      38,    39,    36,   155,    38,    39,    17,     6,     6,    20,
       5,    14,    24,    24,    25,    22,    27,    16,     6,    32,
      33,    34,    35,    36,    37,    36,    12,    38,    39,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,     6,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
       6,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    37,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    24,    24,     6,    25,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    31,    32,    33,    34,
      35,    36,    37,    24,    24,    17,    32,   110,   112,    -1,
      -1,    -1,   114
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,    41,    25,     0,    24,    42,    18,    43,    44,
       4,    47,    18,    45,     1,     5,    19,    48,    49,    52,
      50,    19,    49,     5,    46,    53,    55,    27,    51,    27,
      60,    54,    22,     6,    56,    19,    61,    43,    51,    25,
       1,     7,     8,    10,    11,    13,    15,    17,    20,    25,
      26,    27,    36,    38,    39,    43,    62,    63,    64,    65,
      72,    75,    79,    82,    86,    87,    88,    91,    92,     1,
       5,    57,    58,    59,    27,    80,    81,     9,    27,    83,
      84,    85,    87,    87,    25,    76,    25,    87,    93,    87,
      87,    23,    25,    87,    87,    87,     6,    16,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    27,    24,
      22,     6,    22,     6,    22,     6,    66,    62,    73,    21,
      22,    24,     6,    87,    87,    89,    90,     6,    87,    87,
      87,    87,    87,    87,    87,    87,    87,    87,    59,    81,
      84,    87,    77,    87,    87,     6,    24,    22,    29,    67,
      14,    74,    87,    87,    24,    25,    24,    68,    87,    62,
      62,    78,    69,    24,    12,    70,     6,    71,    62
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    40,    42,    41,    44,    45,    46,    43,    47,    47,
      47,    48,    48,    50,    49,    49,    51,    51,    52,    52,
      54,    53,    56,    55,    57,    57,    57,    58,    58,    59,
      61,    60,    60,    62,    62,    62,    62,    62,    62,    62,
      62,    62,    62,    63,    64,    64,    66,    67,    68,    69,
      65,    71,    70,    70,    73,    74,    72,    76,    77,    78,
      75,    79,    80,    80,    81,    82,    83,    83,    84,    85,
      85,    86,    87,    87,    87,    87,    87,    87,    87,    87,
      87,    87,    87,    87,    87,    87,    87,    87,    87,    87,
      88,    89,    89,    90,    90,    91,    91,    92,    93,    93
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     5,     0,     0,     0,     8,     4,     3,
       0,     2,     1,     0,     4,     1,     3,     1,     2,     0,
       0,     3,     0,     6,     1,     0,     1,     3,     1,     2,
       0,     3,     0,     1,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     4,     3,     3,     0,     0,     0,     0,
      10,     0,     3,     0,     0,     0,     7,     0,     0,     0,
      10,     3,     3,     1,     1,     3,     3,     1,     1,     1,
       1,     3,     3,     2,     2,     2,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     5,     1,     1,     1,     1,
       4,     1,     0,     3,     1,     1,     1,     3,     3,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyo, yytype, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[+yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
#  else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                yy_state_t *yyssp, int yytoken)
{
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Actual size of YYARG. */
  int yycount = 0;
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[+*yyssp];
      YYPTRDIFF_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
      yysize = yysize0;
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYPTRDIFF_T yysize1
                    = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    /* Don't count the "%s"s in the final size, but reserve room for
       the terminator.  */
    YYPTRDIFF_T yysize1 = yysize + (yystrlen (yyformat) - 2 * yycount) + 1;
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss;
    yy_state_t *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYPTRDIFF_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2:
#line 858 "analizador_semantico.y"
                              {
              gen("#include <stdlib.h>\n");
              gen("#include <stdio.h>\n");
              gen("#include \"dec_dat.h\"\n\n");
              gen("#include \"dec_fun.h\"\n");
            }
#line 2284 "y.tab.c"
    break;

  case 4:
#line 868 "analizador_semantico.y"
                   { insertarMarca(); if (yyout != fMain || (yyout == fMain && deep > 0)) { gen("{\n"); ++deep; } }
#line 2290 "y.tab.c"
    break;

  case 5:
#line 869 "analizador_semantico.y"
                                      { if (deep == 0) yyout = fFunc; }
#line 2296 "y.tab.c"
    break;

  case 6:
#line 870 "analizador_semantico.y"
                             { if (deep == 0) { yyout = fMain; gen("int main()\n{\n"); ++deep; } }
#line 2302 "y.tab.c"
    break;

  case 7:
#line 872 "analizador_semantico.y"
                    { vaciarEntradas(); --deep; gen("}\n\n"); }
#line 2308 "y.tab.c"
    break;

  case 8:
#line 877 "analizador_semantico.y"
                                                                        { gen("\n"); }
#line 2314 "y.tab.c"
    break;

  case 9:
#line 878 "analizador_semantico.y"
                                                      { gen("\n"); }
#line 2320 "y.tab.c"
    break;

  case 13:
#line 884 "analizador_semantico.y"
                               { tipoTmp = yyvsp[0].dtipo; }
#line 2326 "y.tab.c"
    break;

  case 14:
#line 885 "analizador_semantico.y"
                                              { gen("%s %s;\n", tipoIntermedio(yyvsp[-3].dtipo), yyvsp[-1].lexema); }
#line 2332 "y.tab.c"
    break;

  case 16:
#line 888 "analizador_semantico.y"
                                          {
                    insertarVariable(yyvsp[-2].lexema);
                    yyval.lexema = malloc(sizeof(char) * (strlen(yyvsp[-2].lexema) + strlen(yyvsp[0].lexema) + 3));
                    sprintf(yyval.lexema, "%s, %s", yyvsp[-2].lexema, yyvsp[0].lexema);
                  }
#line 2342 "y.tab.c"
    break;

  case 17:
#line 893 "analizador_semantico.y"
                     { insertarVariable(yyvsp[0].lexema); yyval.lexema = yyvsp[0].lexema; }
#line 2348 "y.tab.c"
    break;

  case 20:
#line 899 "analizador_semantico.y"
                                  { subProg = 1; }
#line 2354 "y.tab.c"
    break;

  case 22:
#line 902 "analizador_semantico.y"
                           { insertarFuncion(yyvsp[-1].dtipo, yyvsp[0].lexema); }
#line 2360 "y.tab.c"
    break;

  case 23:
#line 903 "analizador_semantico.y"
                                                      {
                      gen("%s %s(%s)\n", tipoIntermedio(yyvsp[-5].dtipo), yyvsp[-4].lexema, yyvsp[-1].lexema);
                    }
#line 2368 "y.tab.c"
    break;

  case 24:
#line 907 "analizador_semantico.y"
                                 { yyval.lexema = yyvsp[0].lexema; }
#line 2374 "y.tab.c"
    break;

  case 25:
#line 908 "analizador_semantico.y"
                                 { yyval.lexema = ""; }
#line 2380 "y.tab.c"
    break;

  case 27:
#line 911 "analizador_semantico.y"
                                       {
                yyval.lexema = malloc(strlen(yyvsp[-2].lexema) + strlen(yyvsp[0].lexema) + 3);
                sprintf(yyval.lexema, "%s, %s", yyvsp[-2].lexema, yyvsp[0].lexema);
              }
#line 2389 "y.tab.c"
    break;

  case 28:
#line 915 "analizador_semantico.y"
                        { yyval.lexema = yyvsp[0].lexema; }
#line 2395 "y.tab.c"
    break;

  case 29:
#line 917 "analizador_semantico.y"
                    {
              insertarParametro(yyvsp[-1].dtipo, yyvsp[0].lexema);
              yyval.lexema = malloc(strlen(yyvsp[-1].lexema) + strlen(yyvsp[0].lexema) + 2);
              sprintf(yyval.lexema, "%s %s", tipoIntermedio(yyvsp[-1].dtipo), yyvsp[0].lexema);
            }
#line 2405 "y.tab.c"
    break;

  case 30:
#line 925 "analizador_semantico.y"
                        {gen("{\n"); ++deep; }
#line 2411 "y.tab.c"
    break;

  case 31:
#line 925 "analizador_semantico.y"
                                                         { --deep; gen("}\n"); }
#line 2417 "y.tab.c"
    break;

  case 34:
#line 930 "analizador_semantico.y"
                          { gen("%s;\n", yyvsp[-1].lexema); }
#line 2423 "y.tab.c"
    break;

  case 43:
#line 943 "analizador_semantico.y"
                                              {
                          comprobarAsignacion(yyvsp[-3].lexema, yyvsp[-1].dtipo);
                          gen("%s = %s;\n", yyvsp[-3].lexema, yyvsp[-1].lexema);
                       }
#line 2432 "y.tab.c"
    break;

  case 44:
#line 951 "analizador_semantico.y"
                                          {
                      sentenciaLista(yyvsp[-2].dtipo, yyvsp[-1].lexema);
                      if (!strcmp(">>", yyvsp[-1].lexema)) {
                        gen("avanza(%s);\n", yyvsp[-2].lexema);
                      } else {
                        gen("retrocede(%s);\n", yyvsp[-2].lexema);
                      }
                    }
#line 2445 "y.tab.c"
    break;

  case 45:
#line 959 "analizador_semantico.y"
                                       {
                      sentenciaLista(yyvsp[-1].dtipo, yyvsp[-2].lexema);
                      gen("inicio(%s);\n", yyvsp[-1].lexema);
                    }
#line 2454 "y.tab.c"
    break;

  case 46:
#line 967 "analizador_semantico.y"
                         { insertarDescriptor("", etiqueta(), etiqueta()); }
#line 2460 "y.tab.c"
    break;

  case 47:
#line 968 "analizador_semantico.y"
                          {
                    expresionBooleana(yyvsp[0].dtipo);
                    gen("\n");
                    gen("if (!%s) goto %s;\n", yyvsp[0].lexema, ts[tope].descriptor->etiquetaElse);
                  }
#line 2470 "y.tab.c"
    break;

  case 48:
#line 973 "analizador_semantico.y"
                       { gen("{\n"); ++deep; }
#line 2476 "y.tab.c"
    break;

  case 49:
#line 974 "analizador_semantico.y"
                          {
                    { --deep; gen("}\n"); }
                    DescriptorDeInstrControl* ds = ts[tope].descriptor;
                    gen("goto %s;\n\n", ds->etiquetaSalida);
                    gen("%s:", ds->etiquetaElse);
                  }
#line 2487 "y.tab.c"
    break;

  case 50:
#line 980 "analizador_semantico.y"
                            {
                    gen("\n");
                    gen("%s: {} \n", ts[tope].descriptor->etiquetaSalida);
                    --tope;
                  }
#line 2497 "y.tab.c"
    break;

  case 51:
#line 986 "analizador_semantico.y"
                   { gen("\n"); gen("{\n"); ++deep; }
#line 2503 "y.tab.c"
    break;

  case 52:
#line 987 "analizador_semantico.y"
                          { --deep; gen("}\n"); }
#line 2509 "y.tab.c"
    break;

  case 53:
#line 988 "analizador_semantico.y"
                     { int aux = deep; deep = 0; gen(" {}\n"); deep = aux; }
#line 2515 "y.tab.c"
    break;

  case 54:
#line 992 "analizador_semantico.y"
                               {
                      insertarDescriptor(etiqueta(), etiqueta(), "");
                      gen("%s:\n", ts[tope].descriptor->etiquetaEntrada);
                      gen("{\n");
                      ++deep;
                      gen("{\n");
                      ++deep;
                    }
#line 2528 "y.tab.c"
    break;

  case 55:
#line 1000 "analizador_semantico.y"
                            {
                      expresionBooleana(yyvsp[0].dtipo);
                      gen("\n");
                      gen("if (!%s) goto %s;\n", yyvsp[0].lexema, ts[tope].descriptor->etiquetaSalida);
                      --deep;
                      gen("}\n\n");
                    }
#line 2540 "y.tab.c"
    break;

  case 56:
#line 1007 "analizador_semantico.y"
                                   {
                      gen("goto %s;\n\n", ts[tope].descriptor->etiquetaEntrada);
                      --deep;
                      gen("}\n");
                      gen("%s: {}\n", ts[tope].descriptor->etiquetaSalida);
                      --tope;
                    }
#line 2552 "y.tab.c"
    break;

  case 57:
#line 1017 "analizador_semantico.y"
                            {
                          insertarDescriptor(etiqueta(), "", "");
                          gen("%s:\n", ts[tope].descriptor->etiquetaEntrada);
                          gen("{\n");
                          ++deep;
                          gen("{\n");
                          ++deep;
                        }
#line 2565 "y.tab.c"
    break;

  case 58:
#line 1025 "analizador_semantico.y"
                                { --deep; gen("}\n"); }
#line 2571 "y.tab.c"
    break;

  case 59:
#line 1026 "analizador_semantico.y"
                                             {
                          expresionBooleana(yyvsp[0].dtipo);
                          gen("\n");
                          gen("if (%s) goto %s;\n", yyvsp[0].lexema, ts[tope].descriptor->etiquetaEntrada);
                          --deep;
                          gen("}\n");
                          --tope;
                        }
#line 2584 "y.tab.c"
    break;

  case 64:
#line 1043 "analizador_semantico.y"
            {
            TipoDato td = buscarID(yyvsp[0].lexema);
            if (td == booleano) {
              gen("char aux[32];\n");
              gen("scanf(\"%s\", aux);\n", "%s");
              gen("%s = aInt(aux);\n", yyvsp[0].lexema);
            } else if (esLista(td)) {
              gen("%s = leerLista(%s);\n", yyvsp[0].lexema, inicializaTipoLista(tipoLista(td)));
            } else {
              gen("scanf(\"%s\", &%s);\n", tipoImprimir(td), yyvsp[0].lexema);
            }
          }
#line 2601 "y.tab.c"
    break;

  case 65:
#line 1057 "analizador_semantico.y"
                                                         { gen("printf(\"\\n\");\n"); }
#line 2607 "y.tab.c"
    break;

  case 68:
#line 1062 "analizador_semantico.y"
                                 { gen("printf(\"%s\", %s);\n", tipoImprimir(yyvsp[0].dtipo), yyvsp[0].lexema); gen("fflush(stdout);\n"); }
#line 2613 "y.tab.c"
    break;

  case 69:
#line 1064 "analizador_semantico.y"
                             {
                    if (esLista(yyvsp[0].dtipo)) {
                      yyval.lexema = malloc(sizeof(char) * (15 + strlen(yyvsp[0].lexema)));
                      sprintf(yyval.lexema, "listaAString(%s)", yyvsp[0].lexema);
                    } else if (yyvsp[0].dtipo == booleano) {
                      yyval.lexema = malloc(sizeof(char) * (8 + strlen(yyvsp[0].lexema)));
                      sprintf(yyval.lexema, "aBool(%s)", yyvsp[0].lexema);
                    } else
                      yyval.lexema = yyvsp[0].lexema;
                    yyval.dtipo = yyvsp[0].dtipo;
                  }
#line 2629 "y.tab.c"
    break;

  case 70:
#line 1075 "analizador_semantico.y"
                          { yyval.lexema = yyvsp[0].lexema; yyval.dtipo = yyvsp[0].dtipo; }
#line 2635 "y.tab.c"
    break;

  case 71:
#line 1078 "analizador_semantico.y"
                                        { comprobarReturn(yyvsp[-1].dtipo); gen("return %s;\n", yyvsp[-1].lexema); }
#line 2641 "y.tab.c"
    break;

  case 72:
#line 1082 "analizador_semantico.y"
                                    { yyval.lexema = yyvsp[-1].lexema; yyval.dtipo = yyvsp[-1].dtipo; }
#line 2647 "y.tab.c"
    break;

  case 73:
#line 1083 "analizador_semantico.y"
                                          {
              yyval.lexema = leerOp(yyvsp[0].dtipo, yyvsp[0].lexema, yyvsp[-1].lexema, "", -1);
              yyval.dtipo = masMenos(yyvsp[-1].atributo, yyvsp[0].dtipo);
            }
#line 2656 "y.tab.c"
    break;

  case 74:
#line 1087 "analizador_semantico.y"
                           {
              yyval.lexema = leerOp(yyvsp[0].dtipo, yyvsp[0].lexema, yyvsp[-1].lexema, "", -1);
              yyval.dtipo = excl(yyvsp[0].dtipo);
            }
#line 2665 "y.tab.c"
    break;

  case 75:
#line 1091 "analizador_semantico.y"
                              {
              yyval.lexema = leerOp(yyvsp[0].dtipo, yyvsp[0].lexema, yyvsp[-1].lexema, "", -1);
              yyval.dtipo = intHash(yyvsp[-1].atributo, yyvsp[0].dtipo);
            }
#line 2674 "y.tab.c"
    break;

  case 76:
#line 1095 "analizador_semantico.y"
                                       {
              yyval.lexema = leerOp(yyvsp[-2].dtipo, yyvsp[-2].lexema, yyvsp[-1].lexema, yyvsp[0].lexema, yyvsp[0].dtipo);
              yyval.dtipo = at(yyvsp[-2].dtipo, yyvsp[0].dtipo);
            }
#line 2683 "y.tab.c"
    break;

  case 77:
#line 1099 "analizador_semantico.y"
                                       {
              yyval.lexema = leerOp(yyvsp[-2].dtipo, yyvsp[-2].lexema, yyvsp[-1].lexema, yyvsp[0].lexema, yyvsp[0].dtipo);
              yyval.dtipo = andLog(yyvsp[-2].dtipo, yyvsp[0].dtipo);
            }
#line 2692 "y.tab.c"
    break;

  case 78:
#line 1103 "analizador_semantico.y"
                                      {
              yyval.lexema = leerOp(yyvsp[-2].dtipo, yyvsp[-2].lexema, yyvsp[-1].lexema, yyvsp[0].lexema, yyvsp[0].dtipo);
              yyval.dtipo = orLog(yyvsp[-2].dtipo, yyvsp[0].dtipo);
            }
#line 2701 "y.tab.c"
    break;

  case 79:
#line 1107 "analizador_semantico.y"
                                    {
              yyval.lexema = leerOp(yyvsp[-2].dtipo, yyvsp[-2].lexema, yyvsp[-1].lexema, yyvsp[0].lexema, yyvsp[0].dtipo);
              yyval.dtipo = eqn(yyvsp[-2].dtipo, yyvsp[-1].atributo, yyvsp[0].dtipo);
            }
#line 2710 "y.tab.c"
    break;

  case 80:
#line 1111 "analizador_semantico.y"
                                         {
              yyval.lexema = leerOp(yyvsp[-2].dtipo, yyvsp[-2].lexema, yyvsp[-1].lexema, yyvsp[0].lexema, yyvsp[0].dtipo);
              yyval.dtipo = addSub(yyvsp[-2].dtipo, yyvsp[-1].atributo, yyvsp[0].dtipo);
            }
#line 2719 "y.tab.c"
    break;

  case 81:
#line 1115 "analizador_semantico.y"
                                       {
              yyval.lexema = leerOp(yyvsp[-2].dtipo, yyvsp[-2].lexema, yyvsp[-1].lexema, yyvsp[0].lexema, yyvsp[0].dtipo);
              yyval.dtipo = porDiv(yyvsp[-2].dtipo, yyvsp[-1].atributo, yyvsp[0].dtipo);
            }
#line 2728 "y.tab.c"
    break;

  case 82:
#line 1119 "analizador_semantico.y"
                                       {
              yyval.lexema = leerOp(yyvsp[-2].dtipo, yyvsp[-2].lexema, yyvsp[-1].lexema, yyvsp[0].lexema, yyvsp[0].dtipo);
              yyval.dtipo = porPor(yyvsp[-2].dtipo, yyvsp[0].dtipo);
            }
#line 2737 "y.tab.c"
    break;

  case 83:
#line 1123 "analizador_semantico.y"
                                         {
              yyval.lexema = leerOp(yyvsp[-2].dtipo, yyvsp[-2].lexema, yyvsp[-1].lexema, yyvsp[0].lexema, yyvsp[0].dtipo);
              yyval.dtipo = borrList(yyvsp[-2].dtipo, yyvsp[-1].atributo, yyvsp[0].dtipo);
            }
#line 2746 "y.tab.c"
    break;

  case 84:
#line 1127 "analizador_semantico.y"
                                    {
              yyval.lexema = leerOp(yyvsp[-2].dtipo, yyvsp[-2].lexema, yyvsp[-1].lexema, yyvsp[0].lexema, yyvsp[0].dtipo);
              yyval.dtipo = rel(yyvsp[-2].dtipo, yyvsp[-1].atributo, yyvsp[0].dtipo);
            }
#line 2755 "y.tab.c"
    break;

  case 85:
#line 1131 "analizador_semantico.y"
                                                         {
              yyval.dtipo = ternario(yyvsp[-4].dtipo, yyvsp[-2].dtipo, yyvsp[0].dtipo);
              yyval.lexema = temporal();
              gen("Lista %s;\n", yyval.lexema);
              gen("%s = insertarEn(%s, %s, %s);\n", yyval.lexema, yyvsp[-4].lexema, insertarDato(yyvsp[-2].lexema, yyvsp[-2].dtipo), yyvsp[0].lexema);
            }
#line 2766 "y.tab.c"
    break;

  case 86:
#line 1137 "analizador_semantico.y"
                                                     { yyval.lexema = yyvsp[0].lexema; yyval.dtipo = yyvsp[0].dtipo; }
#line 2772 "y.tab.c"
    break;

  case 87:
#line 1138 "analizador_semantico.y"
                                                     { yyval.lexema = yyvsp[0].lexema; yyval.dtipo = buscarID(yyvsp[0].lexema); }
#line 2778 "y.tab.c"
    break;

  case 88:
#line 1139 "analizador_semantico.y"
                                                     { yyval.lexema = yyvsp[0].lexema; yyval.dtipo = yyvsp[0].dtipo; }
#line 2784 "y.tab.c"
    break;

  case 90:
#line 1144 "analizador_semantico.y"
                                              {
                    yyval.dtipo = comprobarFuncion(yyvsp[-3].lexema);
                    yyval.lexema = malloc(sizeof(char) * (strlen(yyvsp[-3].lexema) + strlen(yyvsp[-1].lexema) + 3));
                    sprintf(yyval.lexema, "%s(%s)", yyvsp[-3].lexema, yyvsp[-1].lexema);
                }
#line 2794 "y.tab.c"
    break;

  case 91:
#line 1150 "analizador_semantico.y"
                              { yyval.lexema = yyvsp[0].lexema; }
#line 2800 "y.tab.c"
    break;

  case 92:
#line 1151 "analizador_semantico.y"
                    { yyval.lexema = ""; }
#line 2806 "y.tab.c"
    break;

  case 93:
#line 1153 "analizador_semantico.y"
                                                   {
                    yyval.lexema = malloc(sizeof(char) * (strlen(yyvsp[-2].lexema) + strlen(yyvsp[0].lexema) + 3));
                    sprintf(yyval.lexema, "%s, %s", yyvsp[-2].lexema, yyvsp[0].lexema);
                    argumentos_tipo_datos[n_argumentos] = yyvsp[0].dtipo;
                    n_argumentos++;
                  }
#line 2817 "y.tab.c"
    break;

  case 94:
#line 1159 "analizador_semantico.y"
                             {
                    yyval.lexema = yyvsp[0].lexema;
                    argumentos_tipo_datos[n_argumentos] = yyvsp[0].dtipo;
                    n_argumentos++;
                  }
#line 2827 "y.tab.c"
    break;

  case 95:
#line 1166 "analizador_semantico.y"
                  { yyval.lexema = leerCte(yyvsp[0].lexema, yyvsp[0].dtipo); yyval.dtipo = yyvsp[0].dtipo; }
#line 2833 "y.tab.c"
    break;

  case 96:
#line 1167 "analizador_semantico.y"
                  { yyval.lexema = yyvsp[0].lexema; yyval.dtipo = yyvsp[0].dtipo; }
#line 2839 "y.tab.c"
    break;

  case 97:
#line 1169 "analizador_semantico.y"
                                        { yyval.lexema = yyvsp[-1].lexema; yyval.dtipo = aTipoLista(yyvsp[-1].dtipo); }
#line 2845 "y.tab.c"
    break;

  case 98:
#line 1171 "analizador_semantico.y"
                                                     {
                      yyval.lexema = yyvsp[-2].lexema;
                      yyval.dtipo = mismoTipoLista(yyvsp[-2].dtipo, yyvsp[0].dtipo);
                      gen("insertar(%s, %s);\n", yyval.lexema, insertarDato(yyvsp[0].lexema, yyvsp[0].dtipo));
                    }
#line 2855 "y.tab.c"
    break;

  case 99:
#line 1176 "analizador_semantico.y"
                              {
                      yyval.lexema = temporal();
                      yyval.dtipo = yyvsp[0].dtipo;
                      gen("Lista %s;\n", yyval.lexema);
                      gen("%s = inicializar(%s);\n", yyval.lexema, inicializaTipoLista(yyval.dtipo));
                      gen("insertar(%s, %s);\n", yyval.lexema, insertarDato(yyvsp[0].lexema, yyvsp[0].dtipo));
                    }
#line 2867 "y.tab.c"
    break;


#line 2871 "y.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *, YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[+*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 1184 "analizador_semantico.y"


#include "lex.yy.c"

void yyerror(const char *msg){
  fprintf(stderr, "[Linea %d] %s\n", yylineno, msg);
  fflush(stderr);
  hayError = 1;
}

int main(){
  fMain = fopen("prog.c", "w");
  fFunc = fopen("dec_fun.h", "w");

  yyout = fMain ;
  yyparse();

  fclose(fMain);
  fclose(fFunc);

  if (hayError) {
    remove("prog.c");
    remove("dec_fun.h");
  }

  return 0;
}
