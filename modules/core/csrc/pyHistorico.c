// pyHistorico.c

#include "../include/pyHistorico.h"
#include "../include/historico.h"

#include <python3.12/structmember.h>
#include <stdlib.h>
#include <string.h>


static void PyHistorico_dealloc(PyHistorico* self){
    Py_XDECREF((PyObject*)self->livro);

    free(self->dataEmprestimo);
    free(self->dataDevolucao);

    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject*
PyHistorico_new(
    PyTypeObject *type,
    PyObject *args,
    PyObject *kwds
){
    PyHistorico *self =
        (PyHistorico*)type->tp_alloc(type, 0);

    if(self != NULL){
        self->livro = NULL;
        self->dataEmprestimo = NULL;
        self->dataDevolucao = NULL;
        self->idUsuario = 0;
        self->status = 0;
    }

    return (PyObject*)self;
}

static int
PyHistorico_init(
    PyHistorico *self,
    PyObject *args,
    PyObject *kwds
){
    static char *kwlist[] = {
        "id_usuario",
        "livro",
        "data_emprestimo",
        "data_devolucao",
        "status",
        NULL
    };

    unsigned int idUsuario;
    PyObject *livroObj;
    const char *dataEmprestimo;
    const char *dataDevolucao;
    int status;

    if(
        !PyArg_ParseTupleAndKeywords(
            args,
            kwds,
            "IOssi",
            kwlist,
            &idUsuario,
            &livroObj,
            &dataEmprestimo,
            &dataDevolucao,
            &status
        )
    ){
        return -1;
    }

    if(!PyObject_TypeCheck(livroObj, &PyLivroType)){
        PyErr_SetString(
            PyExc_TypeError,
            "livro deve ser do tipo Livro"
        );
        return -1;
    }

    if(
        NORMAL != status &&
        ENTREGUE != status &&
        ATRASADO != status
    ){
        PyErr_SetString(
            PyExc_ValueError,
            "status inválido"
        );
        return -1;
    }

    Py_INCREF(livroObj);

    self->livro = (PyLivro*)livroObj;

    self->dataEmprestimo =
        strdup(dataEmprestimo);

    if(self->dataEmprestimo == NULL){
        Py_DECREF(livroObj);
        self->livro = NULL;
        PyErr_NoMemory();
        return -1;
    }

    self->dataDevolucao =
        strdup(dataDevolucao);

    if(self->dataDevolucao == NULL){
        free(self->dataEmprestimo);
        self->dataEmprestimo = NULL;

        Py_DECREF(livroObj);
        self->livro = NULL;

        PyErr_NoMemory();
        return -1;
    }

    self->idUsuario = idUsuario;
    self->status = status;

    return 0;
}

Historico* historicoFromPyHistorico(PyHistorico* obj){
    if(obj == NULL){
        return NULL;
    }

    Livro* livro = livroFromPyLivro(obj->livro);

    if(livro == NULL){
        PyErr_NoMemory();
        return NULL;
    }

    Historico* historico = criarHistorico(
        obj->idUsuario,
        livro,
        obj->dataEmprestimo,
        obj->dataDevolucao,
        obj->status
    );

    if(historico == NULL){
        liberarLivro(livro);
        PyErr_NoMemory();
        return  NULL;
    }


    return historico;
}


PyHistorico* pyHistoricoFromHistorico(Historico* historico){
    if(historico == NULL){
        Py_RETURN_NONE;
    }

    PyHistorico* obj =
        (PyHistorico*)PyHistoricoType.tp_alloc(
            &PyHistoricoType,
            0
        );

    if(obj == NULL){
        return NULL;
    }

    obj->livro =
        (PyLivro*)pyLivroFromLivro(historico->livro);

    if(obj->livro == NULL){
        Py_DECREF(obj);
        return NULL;
    }

    obj->dataEmprestimo =
        malloc(strlen(historico->dataEmprestimo) + 1);

    if(obj->dataEmprestimo == NULL){
        Py_DECREF((PyObject*)obj->livro);
        Py_DECREF(obj);
        PyErr_NoMemory();
        return NULL;
    }

    strcpy(
        obj->dataEmprestimo,
        historico->dataEmprestimo
    );

    obj->dataDevolucao =
        malloc(strlen(historico->dataDevolucao) + 1);

    if(obj->dataDevolucao == NULL){
        free(obj->dataEmprestimo);
        Py_DECREF((PyObject*)obj->livro);
        Py_DECREF(obj);
        PyErr_NoMemory();
        return NULL;
    }

    strcpy(
        obj->dataDevolucao,
        historico->dataDevolucao
    );

    obj->idUsuario = historico->idUsuario;
    obj->status = historico->status;

    return obj;
}


PyMemberDef Py_Historico_members[] = {
    {
        "livro",
        T_OBJECT_EX,
        offsetof(PyHistorico, livro),
        READONLY,
        "Livro associado ao histórico"
    },
    {
        "data_emprestimo",
        T_STRING,
        offsetof(PyHistorico, dataEmprestimo),
        READONLY,
        "Data do empréstimo"
    },
    {
        "data_devolucao",
        T_STRING,
        offsetof(PyHistorico, dataDevolucao),
        READONLY,
        "Data da devolução"
    },
    {
        "id_usuario",
        T_UINT,
        offsetof(PyHistorico, idUsuario),
        READONLY,
        "ID do usuário"
    },
    {
        "status",
        T_INT,
        offsetof(PyHistorico, status),
        READONLY,
        "Status do histórico"
    },
    {NULL}
};


PyTypeObject PyHistoricoType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "core.Historico",
    .tp_basicsize = sizeof(PyHistorico),
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_members = Py_Historico_members,
    .tp_dealloc = (destructor)PyHistorico_dealloc,
};