%{
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void yyerror( const char * msg );

#define YYERROR_VERBOSE
#define min(a, b) ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a < _b ? _a : _b; })
#define max(a, b) ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a > _b ? _a : _b; })

void yyerror( const char * msg );

#define MAX_TAM_TS 500

extern int yylineno;

char msgError[256];

int yylex();


/********************************/
/* ESTRUCTURA TABLA DE SIMBOLOS */
/********************************/

//Tipo de entrada
typedef enum {
  marca,
  funcion,
  variable,
  parametroFormal
} TipoEntrada;

//Si TipoEntrada es función, variable ... indica el tipo de dato
typedef enum {
  entero,
  real,
  booleano,
  caracter,
  listaEntero,
  listaReal,
  listaBooleano,
  listaCaracter,
  error //Si da un error con expresiones
} TipoDato;

typedef struct {
  TipoEntrada tipoEntrada;    // Tipo de entrada
  char* nombre;               // Nombre del identificador (no se usa con marca)
  TipoDato tipoDato;          // Tipo de dato
  int parametros;             // Nº de parámetros (para funciones)
} EntradaTS;

//La Tabla de Símbolos
EntradaTS ts[MAX_TAM_TS];

//Última entrada de la TS usada.
long int tope = -1;

//Tipo auxiliar para declaración de variables
TipoDato tipoTmp;

//Si entramos en un bloque de un subprograma
//Si es 0 es un bloque de un subprograma; en caso contrario no
int subProg = 0;

//Variables usadas para pasar argumentos a una función
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
      return "list of float";
    case listaEntero:
      return "list of int";
    case listaCaracter:
      return "list of char";
    case listaBooleano:
      return "list of bool";
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
  //Miramos si id estaba declarado después de la última marca
  int repetida = 0;
  for (int i = tope; !repetida && ts[i].tipoEntrada != marca; --i) {
    if (ts[i].tipoEntrada != parametroFormal && !strcmp(ts[i].nombre, id)) {
      sprintf(msgError, "ERROR SINTÁCTICO: identificador %s ya declarado\n", id);
      yyerror(msgError);
      repetida = 1;
    }
  }
}

void insertarEntrada(TipoEntrada te, char* nombre, TipoDato tipo_dato, int nParam) {
  //Hacemos la entrada
  EntradaTS entrada = {
    te,
    strdup(nombre),
    tipo_dato,
    nParam
  };

  //Si la tabla está llena da error
  if (tope + 1 >= MAX_TAM_TS) {
    sprintf(msgError, "ERROR SINTÁCTICO: La tabla de símbolos está llena\n");
    yyerror(msgError);
  }
  //Aumentamos el tope
  ++tope;
  //Añadimos la nueva entrada
  ts[tope] = entrada;
}

//Busca una entrada en la TS con el id especificado en el ámbito del programa
//actual. Si no lo encuentra, devuelve -1. No gestiona errores!
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

/*************************************/
/* FUNCIONES DE LA TABLA DE SIMBOLOS */
/*************************************/

void insertarMarca() {
  //Metemos la marca
  insertarEntrada(marca, "", -1, -1);
  //Si es subprograma añadimos las variables al bloque
  if (subProg) {
    for (int i = tope - 1; ts[i].tipoEntrada != funcion; --i) {
      insertarEntrada(variable, ts[i].nombre, ts[i].tipoDato, -1);
    }
    subProg = 0;
  }
}

void vaciarEntradas() {
  //Hasta la última marca borramos todo
  while (ts[tope].tipoEntrada != marca)
    --tope;
  //Elimina la última marca
  --tope;
}

void insertarVariable(char* id) {
  //Comprobamos que no esté repetida la id
  idRepetida(id);
  insertarEntrada(variable, id, tipoTmp, -1);
}

void insertarFuncion(TipoDato tipoDato, char* id) {
  //Comprobamos que el id no esté usado ya
  idRepetida(id);
  insertarEntrada(funcion, id, tipoDato, 0);
}

void insertarParametro(TipoDato tipoDato, char* id) {
  //Comprobamos que no haya parámetros con nombres repetidos
  //Además guardamos el índice de la función
  int i;
  int parametroRepetido = 0;
  for (i = tope; !parametroRepetido && ts[i].tipoEntrada != funcion; --i) {
    if (!strcmp(ts[i].nombre, id)) {
      sprintf(msgError, "ERROR SINTÁCTICO: identificador del parámetro %s ya declarado\n", id);
      yyerror(msgError);
      parametroRepetido = 1;
    }
  }
  //Añadimos la entrada
  insertarEntrada(parametroFormal, id, tipoDato, -1);
  //Actualizamos el nº de parámetros de la función
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
            id, tipoAString(td), tipoAString(td));
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

TipoDato not(TipoDato td) {
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

// Nos hemos quedado aqui
TipoDato interrogacion(TipoDato td) {
  if (td == error)
    return error;

// WARNING Esto se hace suponiendo que la interrogación está sola en la tabla tokens
  char* operador = "?";
  if (!esLista(td)) {
    sprintf(msgError, "ERROR SINTÁCTICO: operador %s no aplicable al tipo %s\n",
        operador, tipoAString(td));
    yyerror(msgError);
    return error;
  }

    return tipoLista(td);
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

// TODO  Comprueba el tipo de la operación binaria realizada. En caso de error, lo
// TODO  gestiona. En caso contrario, devuelve el tipo tras la operación binaria.
// TODO  IMPORTANTE: Se asume que op1 esta asociado al valor 1 del atributo (atr)
// TODO  mientras que op2 está asociado al valor 0.
// TODO  IMPORTANE: Se asume que el op1 es simétrico y que el op2 no es simétrico y
// TODO  unicamente funciona de la forma "T op2 T" o bien "list_of T op2 T".
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
    // TODO  Llegado a este punto hay exactamente una lista. Vemos si el tipo de
    // TODO  la lista encaja con el tipo del otro atributo:
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
        yylineno, tipoAString(td1), tipoAString(td2), tipoAString(td3));
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

  // TODO  De esta forma mostramos el máximo número de errores posibles.

  // TODO  Borramos los argumentos recibidos.
  n_argumentos = 0;

  // TODO  Devolvemos el tipo de la función.
  return ts[iFuncion].tipoDato;
}

#define YYSTYPE Atributos

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


// TODO  Hay que adaptarlo para REPEAT-UNTIL
sentencia_repeat_until  : REPEAT sentencia UNTIL PARIZQ expresion PARDER PYC { expresionBooleana($5.dtipo); };

// TODO  Revisar estas funciones porque nuestras listas son diferentes
sentencia_lista : expresion OPERLISTA PYC { sentenciaLista($1.dtipo, $2.lexema); }
                | INT expresion PYC
                | DOLLAR expresion PYC { sentenciaLista($2.dtipo, $1.lexema); };


// TODO  Revisar todas las funciones, para nosotros NOT==EXCL
expresion : PARIZQ expresion PARDER        { $$.dtipo = $2.dtipo; }
          | MASMENOS expresion %prec NOT   { $$.dtipo = masMenos($1.atributo, $2.dtipo); } // TODO  Revisar not---EXCL
          | BAR expresion                  { $$.dtipo = interrogacion($1.atributo, $2.dtipo); } // TODO   Revisar Hash--Bar
          | NOT expresion                  { $$.dtipo = not($2.dtipo); } // TODO Revisar not -- EXCL
          | INT expresion                  { $$.dtipo = interrogacion($1.atributo, $2.dtipo); } // TODO  Revisar y adaptar para INT
          | MENOSMENOS expresion
          | expresion MENOSMENOS expresion // TODO  Hacer funcion para el MENOSMENOS
          | expresion ORLOG expresion     { $$.dtipo = orLog($1.dtipo, $3.dtipo); } // TODO Debería estar bien esta función, pero revisar
          | expresion ANDLOG expresion    { $$.dtipo = andLog($1.dtipo, $3.dtipo); } // TODO Debería estar bien esta función, pero revisar
          | expresion ARROBA expresion    { $$.dtipo = at($1.dtipo, $3.dtipo); } // TODO Cambiar at por arroba y revisar
          | expresion XOR expresion       // TODO Crear una función para XOR
          | expresion REL expresion       { $$.dtipo = rel($1.dtipo, $2.atributo, $3.dtipo); } // TODO Debería estar bien
          | expresion MASMENOS expresion  { $$.dtipo = addSub($1.dtipo, $2.atributo, $3.dtipo); } // TODO Revisar y adaptar nombre a MASMENOS
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

%%

#include "lex.yy.c"

void yyerror(const char *msg){
  fprintf(stderr, "[Linea %d]: %s\n", yylineno, msg);
}

int main(){
  yyparse();

  return 0;
}
