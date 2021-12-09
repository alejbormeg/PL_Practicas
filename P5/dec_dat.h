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

int* pInt(int n) {
  int* p = malloc(sizeof(int));
  *p = n;
  return p;
}

char* pChar(char c) {
  char* p = malloc(sizeof(char));
  *p = c;
  return p;
}

float* pFloat(float f) {
  float* p = malloc(sizeof(float));
  *p = f;
  return p;
}

Lista inicializar(TipoLista tipo) {
  Lista l = malloc(sizeof(VecDin));
  l->tam = 0;
  l->capacidad = 4;
  l->cursor = -1;
  l->tipo = tipo;
  l->datos = malloc(sizeof(void *) * l->capacidad);
  return l;
}

void redimensionar(Lista l, int capacidad) {
  void** datos = realloc(l->datos, sizeof(void*) * capacidad);
  if (datos) {
    l->datos = datos;
    l->capacidad = capacidad;
  }
}

void insertar(Lista l, void* dato) {
  if (l->capacidad == l->tam)
    redimensionar(l, l->capacidad * 2);
  l->datos[l->tam++] = dato;
  if (l->tam == 1)
    l->cursor = 0;
}

// Operador #
int getTam(Lista l) {
  return l->tam;
}

// Operador >>
void avanza(Lista l) {
  if (l->tam >= 0) {
    if (l->cursor + 1 < l->tam)
      l->cursor++;
    else {
      error("ERROR avanza(): cursor ya está en el final.\n");
    }
  } else {
      error("ERROR avanza(): lista vacía.\n");
    }
}

// Operador <<
void retrocede(Lista l) {
  if (l->tam >= 0) {
    if (l->cursor - 1 >= 0)
      l->cursor--;
    else {
      error("ERROR retrocede(): cursor ya está al principio.\n");
    }
  } else {
      error("ERROR avanza(): lista vacía.\n");
    }
}

// Operador $
void inicio(Lista l) {
  if (l->tam >= 0) {
    l->cursor = 0;
  } else {
      error("ERROR inicio(): lista vacía.\n");
    }
}

// Operador ?
void* getActual(Lista l) {
  if (l->cursor < 0) {
    error("ERROR getActual(): la lista está vacía.\n");
  }
  return l->datos[l-> cursor];
}

void set(Lista l, int pos, void* dato) {
  if (pos >= 0 && pos < l->tam)
    l->datos[pos] = dato;
  else {
    error("ERROR: set(): %s pos no válida, tam: %s.\n", pos, l->tam);
  }
}

// Operador @
void* get(Lista l, int pos) {
  if (pos >= 0 && pos < l->tam)
    return l->datos[pos];
  else {
    error("ERROR: get(): %s pos no válida, tam: %s.\n", pos, l->tam);
  }
}

void borrar(Lista l, int pos) {
  if (pos < 0 || pos >= l->tam) {
    error("ERROR: borrar(): %s pos no válida, tam: %s.\n", pos, l->tam);
  }

  l->datos[pos] = NULL;
  for (int i = pos; i < l->tam; ++i) {
    l->datos[i] = l->datos[i + 1];
    l->datos[i + 1] = NULL;
  }

  l->tam--;

  if(l->tam == 0)
    l->cursor = -1;

  if (l->tam > 0 && l->tam == l->capacidad / 4)
    redimensionar(l, l->capacidad / 2);
}

Lista copiar(Lista l, int pos) {
  Lista nueva = inicializar(l->tipo);
  for (int i = 0; i < pos; ++i) {
    void* aux = malloc(sizeof(void*));
    memcpy(aux, l->datos[i], sizeof(void*));
    insertar(nueva, aux);
  }

  return nueva;
}

// Operador --
Lista borrarEn(Lista l, int pos) {
  Lista nueva = copiar(l, l->tam);
  borrar(nueva, pos);
  return nueva;
}

// Operador %
Lista borrarAPartirDe(Lista l, int pos) {
  return copiar(l, pos + 1);
}

// Operador ++ @
Lista insertarEn(Lista l, void* dato, int pos) {
  Lista nueva = copiar(l, pos);
  insertar(nueva, dato);
  for (int i = pos; i < l->tam; ++i) {
    void* aux = malloc(sizeof(void*));
    memcpy(aux, l->datos[i], sizeof(void*));
    insertar(nueva, aux);
  }
  return nueva;
}

// Operador **
Lista concatenar(Lista l1, Lista l2) {
  Lista nueva = copiar(l1, l1->tam);
  for (int i = 0; i < l2->tam; ++i) {
    void* aux = malloc(sizeof(void*));
    memcpy(aux, l2->datos[i], sizeof(void*));
    insertar(nueva, aux);
  }
  return nueva;
}

// https://stackoverflow.com/questions/8465006/how-do-i-concatenate-two-strings-in-c
char* concat(const char *s1, const char *s2)
{
    const size_t len1 = strlen(s1);
    const size_t len2 = strlen(s2);
    char *result = malloc(len1 + len2 + 1); // +1 for the null-terminator
    // in real code you would check for errors in malloc here
    memcpy(result, s1, len1);
    memcpy(result + len1, s2, len2 + 1); // +1 to copy the null-terminator
    return result;
}

char* aBool(int n) {
  if (n == 1)
    return "true";
  else if (n == 0)
    return "false";
  else {
    error("ERROR en aBool(); no es 1 ni 0.\n");
  }
}

int aInt(char* cad) {
  if (!strcmp("true", cad))
    return 1;
  else if (!strcmp("false", cad))
    return 0;
  else {
    error("ERROR en aInt(). no es true ni false.\n")
  }
}

char* listaAString(Lista l) {
  char* result = "[";
  for (int i = 0; i < l->tam; ++i) {
    char* value = malloc(sizeof(char) * 10);
    if (l->tipo == tInt) {
      sprintf(value, "%d", *(int*)l->datos[i]);
    } else if (l->tipo == tFloat) {
      sprintf(value, "%0.2f", *(float*)l->datos[i]);
    } else if(l->tipo == tChar) {
      sprintf(value, "%c", *(char*)l->datos[i]);
    } else if(l->tipo == tBool) {
      sprintf(value, "%s", aBool(*(int*)l->datos[i]));
    } else {
      error("ERROR en listaAstring(): tipo no válido.\n");
    }
    result = concat(result, value);

    if (i + 1 < l->tam)
      result = concat(result, ", ");
  }
  result = concat(result, "]");

  return result;
}

char* tipoAString(TipoLista tl) {
  switch (tl) {
    case tInt:
      return "int";
    case tFloat:
      return "float";
    case tBool:
      return "bool";
    case tChar:
      return "char";
  }
}

Lista leerLista(TipoLista tl) {
  Lista l = inicializar(tl);
  int tam;
  printf("Tipo de lista: %s. Introduce tamaño: ", tipoAString(tl));
  scanf("%d", &tam);
  printf("Introduce los elementos: ");
  fflush(stdin);
  for (int i = 0; i < tam; ++i) {
    if (tl == tInt) {
      int aux;
      scanf("%d", &aux);
      insertar(l, pInt(aux));
    } else if (tl == tFloat) {
      float aux;
      scanf("%f", &aux);
      insertar(l, pFloat(aux));
    } else if (tl == tChar) {
      char aux;
      scanf("%c", &aux);
      insertar(l, pChar(aux));
    } else if (tl == tBool) {
      char aux[5];
      scanf("%s", aux);
      insertar(l, pInt(aInt(aux)));
    }
    fflush(stdin);
  }
  return l;
}

void destruir(Lista l) {
  free(l->datos);
  l->tam = 0;
  l->capacidad = 0;
  l->cursor = -1;
  l->tipo = -1;
}

Lista sumarLista(Lista l, float n) {
  l = copiar(l, l->tam);
  if (l->tipo == tInt || l->tipo == tBool) {
    for (int i = 0; i < l->tam; ++i)
      *(int*)l->datos[i] = *(int*)l->datos[i] + (int) n;
  } else if (l->tipo == tFloat) {
    for (int i = 0; i < l->tam; ++i)
      *(float*)l->datos[i] = *(float*)(l->datos[i]) + n;
  }
  return l;
}

Lista multiplicarLista(Lista l, float n) {
  l = copiar(l, l->tam);
  if (l->tipo == tInt || l->tipo == tBool) {
    for (int i = 0; i < l->tam; ++i)
      *(int*)l->datos[i] = *(int*)l->datos[i] * (int) n;
  } else if (l->tipo == tFloat) {
    for (int i = 0; i < l->tam; ++i)
      *(float*)l->datos[i] = *(float*)(l->datos[i]) * n;
  }
  return l;
}

Lista restarLista(Lista l, float n) {
  l = copiar(l, l->tam);
  if (l->tipo == tInt || l->tipo == tBool) {
    for (int i = 0; i < l->tam; ++i)
      *(int*)l->datos[i] = *(int*)l->datos[i] - (int) n;
  } else if (l->tipo == tFloat) {
    for (int i = 0; i < l->tam; ++i)
      *(float*)l->datos[i] = *(float*)(l->datos[i]) - n;
  }
  return l;
}

Lista dividirLista(Lista l, float n) {
  l = copiar(l, l->tam);
  if (l->tipo == tInt || l->tipo == tBool) {
    for (int i = 0; i < l->tam; ++i)
      *(int*)l->datos[i] = *(int*)l->datos[i] / (int) n;
  } else if (l->tipo == tFloat) {
    for (int i = 0; i < l->tam; ++i)
      *(float*)l->datos[i] = *(float*)(l->datos[i]) / n;
  }
  return l;
}
