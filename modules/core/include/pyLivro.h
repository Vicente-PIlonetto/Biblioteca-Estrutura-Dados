#ifndef PY_LIVRO_H
#define PY_LIVRO_H

#include <python3.12/Python.h>

#include "../include/livro.h"

typedef struct{
  PyObject_HEAD
  char *titulo;
  char *autor;
  uint32_t codigo;
  uint16_t anoPublicacao;
  uint16_t quantidadeDisponivel;
} PyLivro;

Livro* livroFromPyLivro(PyLivro* obj);

PyObject* pyLivroFromLivro(Livro* livro);

extern PyMemberDef Py_Livro_members[];

extern PyTypeObject PyLivroType;


#endif