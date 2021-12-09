#ifndef __DAT__
#define __DAT__
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define error(f_, ...) { fprintf(stderr, f_, ##__VA_ARGS__); fflush(stderr); exit(EXIT_FAILURE); }

typedef enum {
  tInt,
  tFloat,
  tChar,
  tBool
} TipoLista;

typedef struct {
  void** datos;
  int tam;
  int capacidad;
  int cursor;
  TipoLista tipo;
} VecDin;

typedef VecDin* Lista;

int* pInt(int n);

char* pChar(char c);

float* pFloat(float f);

Lista inicializar(TipoLista tipo);

void redimensionar(Lista l, int capacidad);

void insertar(Lista l, void* dato);

// Operador #
int getTam(Lista l);

// Operador >>
void avanza(Lista l);
// Operador <<
void retrocede(Lista l);

// Operador $
void inicio(Lista l);
// Operador ?
void* getActual(Lista l);

void set(Lista l, int pos, void* dato);

// Operador @
void* get(Lista l, int pos);

void borrar(Lista l, int pos);

Lista copiar(Lista l, int pos);

// Operador --
Lista borrarEn(Lista l, int pos);

// Operador %
Lista borrarAPartirDe(Lista l, int pos);

// Operador ++ @
Lista insertarEn(Lista l, void* dato, int pos);

// Operador **
Lista concatenar(Lista l1, Lista l2);

char* concat(const char *s1, const char *s2);

char* aBool(int n);

int aInt(char* cad);

char* listaAString(Lista l);

char* tipoAString(TipoLista tl);

Lista leerLista(TipoLista tl);

void destruir(Lista l);

Lista sumarLista(Lista l, float n);

Lista multiplicarLista(Lista l, float n);

Lista restarLista(Lista l, float n);

Lista dividirLista(Lista l, float n);

#endif
