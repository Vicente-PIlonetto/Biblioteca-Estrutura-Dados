#include "../include/pyLivro.h"
#include <python3.12/structmember.h>
#include <stdlib.h>
#include <string.h>

static PyObject *
Livro_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    PyLivro *self = (PyLivro *)type->tp_alloc(type, 0);

    if (self != NULL) {
        self->titulo = NULL;
        self->autor = NULL;
    }

    return (PyObject *)self;
}

static int
Livro_init(PyLivro *self, PyObject *args, PyObject *kwds) {
    static char *kwlist[] = {
        "titulo",
        "autor",
        "codigo",
        "ano_publicacao",
        "quantidade_disponivel",
        NULL
    };

    const char *titulo;
    const char *autor;

    if (
        !PyArg_ParseTupleAndKeywords(
            args,
            kwds,
            "ssIhh",
            kwlist,
            &titulo,
            &autor,
            &self->codigo,
            &self->anoPublicacao,
            &self->quantidadeDisponivel
        )
    ) return -1;

    self->titulo = strdup(titulo);
    if (self->titulo == NULL) {
        PyErr_NoMemory();
        return -1;
    }
    self->autor = strdup(autor);

    if (self->autor == NULL) {
        free(self->titulo);
        self->titulo = NULL;
        PyErr_NoMemory();
        return -1;
    }

    return 0;
}

static void
Livro_dealloc(PyLivro *self) {
    free(self->titulo);
    free(self->autor);

    Py_TYPE(self)->tp_free((PyObject *)self);
}

Livro* livroFromPyLivro(PyLivro* livro){
    return criarLivro(
        livro->titulo,
        livro->autor,
        livro->anoPublicacao,
        livro->quantidadeDisponivel,
        livro->codigo
    );
}

PyObject* pyLivroFromLivro(Livro* livro){
    if (livro == NULL) {
        Py_RETURN_NONE;
    }

    PyLivro* py_livro = (PyLivro*)PyLivroType.tp_alloc(&PyLivroType, 0);
    if (py_livro == NULL) {
        return NULL;
    }

    py_livro->titulo = strdup(livro->titulo);
    if(py_livro->titulo == NULL){
        Py_DECREF(py_livro);
        return NULL;
    }
    py_livro->autor = strdup(livro->autor);
    if(py_livro->autor == NULL){
        free(py_livro->titulo);
        Py_DECREF(py_livro);

        return NULL;
    }
    py_livro->codigo = livro->codigo;
    py_livro->quantidadeDisponivel = livro->quantidadeDisponivel;
    py_livro->anoPublicacao = livro->anoPublicacao;

    return (PyObject*)py_livro;
}

PyMemberDef Py_Livro_members[] = {
    {
        "titulo",
        T_STRING,
        offsetof(PyLivro, titulo),
        READONLY,
        NULL
    },
    {
        "autor",
        T_STRING,
        offsetof(PyLivro, autor),
        READONLY,
        NULL
    },
    {
        "codigo",
        T_UINT,
        offsetof(PyLivro, codigo),
        READONLY,
        NULL
    },
    {
        "ano_publicacao",
        T_SHORT,
        offsetof(PyLivro, anoPublicacao),
        READONLY,
        NULL
    },
    {
        "quantidade_disponivel",
        T_USHORT,
        offsetof(PyLivro, quantidadeDisponivel),
        0,
        NULL
    },
    {NULL}
};

PyTypeObject PyLivroType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "core.Livro",
    .tp_basicsize = sizeof(PyLivro),
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = PyType_GenericNew,
    .tp_init = (initproc)Livro_init,
    .tp_members = Py_Livro_members
};