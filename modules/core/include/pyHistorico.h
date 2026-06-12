#ifndef PY_HISTORICO_H
#define PY_HISTORICO_H

#include <python3.12/Python.h>

#include "../include/historico.h"
#include "../include/pyLivro.h"

typedef struct{
  PyObject_HEAD
  PyLivro* livro;

  char* dataEmprestimo;
  char* dataDevolucao;

  unsigned int idUsuario;
  enum StatusHistorico status;
} PyHistorico;

Historico* historicoFromPyHistorico(PyHistorico* obj);

PyHistorico* pyHistoricoFromHistorico(Historico* historico);

extern PyMemberDef Py_Historico_members[];

extern PyTypeObject PyHistoricoType;


#endif