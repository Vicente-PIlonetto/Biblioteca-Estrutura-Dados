#include "../include/pyPilhaHistorico.h"

#include <python3.12/structmember.h>


static PyObject *
PyPilhaHistorico_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    PyPilhaHistorico *self =
        (PyPilhaHistorico *)type->tp_alloc(type, 0);

    if (self != NULL) {
        self->root = NULL;
        self->length = 0;
    }

    return (PyObject *)self;
}

static int
PyPilhaHistorico_init(
    PyPilhaHistorico *self,
    PyObject *args,
    PyObject *kwds
) {
    static char *kwlist[] = {
        "historico",
        NULL
    };

    PyObject *historico = Py_None;

    if (
        !PyArg_ParseTupleAndKeywords(
            args,
            kwds,
            "|O",
            kwlist,
            &historico
        )
    ) {
        return -1;
    }

    if (historico == Py_None) {
        return 0;
    }

    if (!PyObject_TypeCheck(historico, &PyHistoricoType)) {
        PyErr_SetString(
            PyExc_TypeError,
            "historico deve ser do tipo Historico ou None"
        );
        return -1;
    }

    Historico *hist =
        historicoFromPyHistorico(
            (PyHistorico *)historico
        );

    if (hist == NULL) {
        return -1;
    }

    self->root = criarPilhaHistorico(hist);

    if (self->root == NULL) {
        liberarHistorico(hist);
        PyErr_NoMemory();
        return -1;
    }

    self->length = 1;

    return 0;
}

static void
PyPilhaHistorico_dealloc(PyPilhaHistorico *self) {
    if (self->root != NULL) {
        liberarPilhaHistorico(self->root);
    }

    Py_TYPE(self)->tp_free((PyObject *)self);
}

PyPilhaHistorico *
pyPilhaHistoricoFromPilhaHistorico(
    PilhaHistorico *pilha
) {
    PyPilhaHistorico *obj =
        PyObject_New(
            PyPilhaHistorico,
            &PyPilhaHistoricoType
        );

    if (obj == NULL) {
        return NULL;
    }

    obj->root = pilha;

    uint32_t len = 0;
    PilhaHistorico *tmp = pilha;

    while (tmp != NULL) {
        len++;
        tmp = tmp->prox;
    }

    obj->length = len;

    return obj;
}

static PyObject *
PyPilhaHistorico_push(
    PyPilhaHistorico *self,
    PyObject *args,
    PyObject *kwds
) {
    static char *kwlist[] = {
        "historico",
        NULL
    };

    PyObject *historico;

    if (
        !PyArg_ParseTupleAndKeywords(
            args,
            kwds,
            "O",
            kwlist,
            &historico
        )
    ) {
        return NULL;
    }

    if (!PyObject_TypeCheck(historico, &PyHistoricoType)) {
        PyErr_SetString(
            PyExc_TypeError,
            "historico deve ser do tipo Historico"
        );
        return NULL;
    }

    Historico *hist =
        historicoFromPyHistorico(
            (PyHistorico *)historico
        );

    if (hist == NULL) {
        return NULL;
    }

    PilhaHistorico *nova =
        adicionarHistorico(
            self->root,
            hist
        );

    if (nova == self->root && self->root != NULL) {
        liberarHistorico(hist);
        PyErr_NoMemory();
        return NULL;
    }

    self->root = nova;
    self->length++;

    Py_RETURN_NONE;
}

static PyObject *
PyPilhaHistorico_top(PyPilhaHistorico *self, PyObject *Py_UNUSED(ignored)) {
    if (self->root == NULL) {
        Py_RETURN_NONE;
    }

    return (PyObject *)
        pyHistoricoFromHistorico(
            self->root->item
        );
}

static PyObject *
PyPilhaHistorico_pop(PyPilhaHistorico *self, PyObject *Py_UNUSED(ignored)) {
    if (self->root == NULL) {
        Py_RETURN_NONE;
    }

    PilhaHistorico *topo = self->root;
    self->root = topo->prox;

    PyObject *ret =
        (PyObject *)
        pyHistoricoFromHistorico(
            topo->item
        );

    topo->prox = NULL;
    liberarPilhaHistorico(topo);

    self->length--;

    return ret;
}

PyMethodDef PyPilhaHistorico_methods[] = {
    {
        "push",
        (PyCFunction)PyPilhaHistorico_push,
        METH_VARARGS | METH_KEYWORDS,
        NULL
    },
    {
        "pop",
        (PyCFunction)PyPilhaHistorico_pop,
        METH_NOARGS,
        NULL
    },
    {
        "top",
        (PyCFunction)PyPilhaHistorico_top,
        METH_NOARGS,
        NULL
    },
    {NULL}
};

PyMemberDef PyPilhaHistorico_members[] = {
    {
        "length",
        T_UINT,
        offsetof(PyPilhaHistorico, length),
        READONLY,
        NULL
    },
    {NULL}
};

PyTypeObject PyPilhaHistoricoType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "core.PilhaHistorico",
    .tp_basicsize = sizeof(PyPilhaHistorico),
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = PyPilhaHistorico_new,
    .tp_init = (initproc)PyPilhaHistorico_init,
    .tp_dealloc = (destructor)PyPilhaHistorico_dealloc,
    .tp_methods = PyPilhaHistorico_methods,
    .tp_members = PyPilhaHistorico_members
};