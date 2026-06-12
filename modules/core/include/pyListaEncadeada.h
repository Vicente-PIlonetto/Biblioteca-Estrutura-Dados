#ifndef LISTA_ENCADEADA_H
#define LISTA_ENCADEADA_H

#include "listaEncadeada.h"
#include <python3.12/Python.h>
#include <stdint.h>


typedef struct {
  PyObject_HEAD;
  ListaEncadeada *root;
  uint32_t length;
} PyListaEncadeada;


PyListaEncadeada* pyListaEncadeadaFromListaEncadeada(ListaEncadeada*lista);

extern PyMemberDef PyListaEncadeada_members[];

extern PyTypeObject PyListaEncadeadaType;


#endif