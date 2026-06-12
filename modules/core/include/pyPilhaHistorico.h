#ifndef PT_PILHA_HISTORICO_HPP
#define PT_PILHA_HISTORICO_HPP

#include <python3.12/Python.h>
#include "pilhaHistorico.h"
#include "pyHistorico.h"

typedef struct {
    PyObject_HEAD

    PilhaHistorico *root;
    uint32_t length;
} PyPilhaHistorico;

PyPilhaHistorico*
pyPilhaHistoricoFromPilhaHistorico(
    PilhaHistorico *pilha
);

extern PyTypeObject PyPilhaHistoricoType;
extern PyMemberDef PyPilhaHistorico_members[];

#endif