#include <python3.12/Python.h>
#include <python3.12/object.h>
#include <python3.12/pyerrors.h>
#include <python3.12/structmember.h>
#include <stdint.h>
#include "../include/pyListaEncadeada.h"
#include "../include/listaEncadeada.h"
#include "../include/pyLivro.h"


static PyObject *
ListaEncadeada_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    PyListaEncadeada *self = (PyListaEncadeada *)type->tp_alloc(type, 0);

    if (self != NULL) {
        self->root = NULL;
        self->length = 0;
    }

    return (PyObject *)self;
}


static int
ListaEncadeada_init(PyListaEncadeada *self, PyObject *args, PyObject *kwds) {
    static char *kwlist[] = {
        "livro",
        NULL
    };
    PyObject *livro = NULL;


    if (
        !PyArg_ParseTupleAndKeywords(
            args,
            kwds,
            "O",
            kwlist,
            &livro
        )
    ) {
        return -1;
    }

    if (livro == Py_None) {
        return 0;
    }

    if (!PyObject_TypeCheck(livro, &PyLivroType)) {
        PyErr_SetString(
            PyExc_TypeError,
            "Livro deve ser do tipo Livro ou None."
        );
        return -1;
    }

    PyLivro *pyLivroPtr = (PyLivro *)livro;

    Livro* _livro = livroFromPyLivro(pyLivroPtr);

    if(_livro == NULL){
        PyErr_NoMemory();
        return -1;
    }

    self->root = criarLista(
        _livro
    );
    if(self->root == NULL){
        liberarLivro(_livro);
        PyErr_NoMemory();
        return -1;
    }

    self->length = self->root != NULL;

    return 0;
}

PyListaEncadeada*
pyListaEncadeadaFromListaEncadeada(ListaEncadeada*lista){
    PyListaEncadeada *obj =
        PyObject_New(PyListaEncadeada, &PyListaEncadeadaType);

    if (obj == NULL) {
        return NULL;
    }

    obj->root = lista;
    obj->length = ListaEncadeadaGetLength(lista);

    return obj;
}



static void
ListaEncadeada_dealloc(PyListaEncadeada *self) {
    if (self->root != NULL) {
        liberarLista(self->root);
    }
    Py_TYPE(self)->tp_free((PyObject *)self);
}

static PyObject*
ListaEncadeada_append_at(
    PyListaEncadeada *self,
    PyObject *args,
    PyObject *kwds
){
    if(self->root != NULL && !self->root->liberar){
        PyErr_SetString(
            PyExc_TypeError,
            "Não é possível adicionar itens em uma lista temporária"
        );
        return NULL;
    }

    static char *kwlist[] = {
        "livro",
        "idx",
        "replace",
        NULL
    };

    PyObject *livro = NULL;
    uint32_t pos;
    char replace = 0;

    if(
        !PyArg_ParseTupleAndKeywords(
            args,
            kwds,
            "OI|b",
            kwlist,
            &livro,
            &pos,
            &replace
        )
    ){
        return NULL;
    }

    if(replace != 0 && replace != 1){
        PyErr_SetString(
            PyExc_ValueError,
            "replace deve ser 0 ou 1"
        );
        return NULL;
    }

    if(!PyObject_TypeCheck(livro, &PyLivroType)){
        PyErr_SetString(
            PyExc_TypeError,
            "Livro deve ser do tipo Livro."
        );
        return NULL;
    }

    Livro *novoLivro =
        livroFromPyLivro((PyLivro*)livro);

    if(novoLivro == NULL){
        PyErr_NoMemory();
        return NULL;
    }

    if(self->root == NULL){
        self->root = criarLista(novoLivro);

        if(self->root == NULL){
            liberarLivro(novoLivro);
            PyErr_NoMemory();
            return NULL;
        }

        self->length = 1;
        Py_RETURN_NONE;
    }

    if(pos == 0){
        ListaEncadeada *novaRoot =
            adicionarItemInicio(
                self->root,
                novoLivro
            );

        if(novaRoot == NULL){
            liberarLivro(novoLivro);
            PyErr_SetString(
                PyExc_RuntimeError,
                "Falha ao adicionar o item"
            );
            return NULL;
        }

        self->root = novaRoot;
        self->length++;

        Py_RETURN_NONE;
    }

    if(pos > self->length){
        if(!adicionarItemFim(
            self->root,
            novoLivro
        )){
            liberarLivro(novoLivro);

            PyErr_SetString(
                PyExc_RuntimeError,
                "Falha ao adicionar o item"
            );
            return NULL;
        }

        self->length++;

        Py_RETURN_NONE;
    }

    if(!adicionarItemIndice(
        self->root,
        pos,
        novoLivro,
        replace
    )){
        liberarLivro(novoLivro);

        PyErr_SetString(
            PyExc_RuntimeError,
            "Falha ao adicionar o item"
        );
        return NULL;
    }

    if(!replace){
        self->length++;
    }

    Py_RETURN_NONE;
}

static PyObject*
ListaEncadeada_obterLivroIndice(PyListaEncadeada *self, PyObject *args, PyObject *kwds){
    static char *kwlist[] = {
        "idx",
        NULL
    };

    uint32_t idx;

    if(
        !PyArg_ParseTupleAndKeywords(
            args,
            kwds,
            "I",
            kwlist,
            &idx
        )
    ){
        return NULL;
    }

    Livro *livro = obterLivroIndice(self->root, idx);
    if(livro == NULL){
        Py_RETURN_NONE;
    }
    return pyLivroFromLivro(livro);
}

static PyObject*
ListaEncadeada_obterLivroRange(PyListaEncadeada *self, PyObject *args, PyObject *kwds){
    static char *kwlist[] = {
        "init",
        "end",
        NULL
    };

    uint32_t init, end = UINT32_MAX;

    if(
        !PyArg_ParseTupleAndKeywords(
            args,
            kwds,
            "I|I",
            kwlist,
            &init,
            &end
        )
    ){
        return NULL;
    }
    if(end == UINT32_MAX)
        end = self->length - 1;

    ListaEncadeada *livros = obterLivroRange(self->root, init, end);
    if(livros == NULL){
        Py_RETURN_NONE;
    }
    return (PyObject*)pyListaEncadeadaFromListaEncadeada(livros);
}

static PyObject*
ListaEncadeada_obterLivroCodigo(PyListaEncadeada *self, PyObject *args, PyObject *kwds){
    static char *kwlist[] = {
        "codigo",
        NULL
    };

    uint32_t codigo = 1;

    if(
        !PyArg_ParseTupleAndKeywords(
            args,
            kwds,
            "I",
            kwlist,
            &codigo
        )
    ){
        return NULL;
    }

    Livro* livro = compararCodigo(self->root, codigo);
    if(livro){
        return pyLivroFromLivro(livro);
    }
    Py_RETURN_NONE;
}

static PyObject*
ListaEncadeada_obterAnoPublicacao(PyListaEncadeada *self, PyObject *args, PyObject *kwds){
    static char *kwlist[] = {
        "ano_publicacao",
        "comparacao",
        NULL
    };

    int16_t anoPublicacao;
    uint32_t comparacao;

    if(
        !PyArg_ParseTupleAndKeywords(
            args,
            kwds,
            "hI",
            kwlist,
            &anoPublicacao,
            &comparacao
        )
    ){
        return NULL;
    }

    if(comparacao > ANO_MAIOR_IGUAL){
        PyErr_SetString(PyExc_TypeError, "comparacao deve ser 0(=), 1(<), 2(<=), 3(>) ou 4(>=)");
        return NULL;
    }

    ListaEncadeada *lista = compararAnoPublicacao(self->root, anoPublicacao, comparacao);
    if(lista != NULL){
        return (PyObject *)pyListaEncadeadaFromListaEncadeada(lista);
    }
    Py_RETURN_NONE;
}

static PyObject*
ListaEncadeada_obterDisponivel(PyListaEncadeada *self, PyObject *args, PyObject *kwds){
    static char *kwlist[] = {
        "disponibilidade",
        NULL
    };

    char disponibilidade;

    if(
        !PyArg_ParseTupleAndKeywords(
            args,
            kwds,
            "b",
            kwlist,
            &disponibilidade
        )
    ){
        return NULL;
    }

    ListaEncadeada *lista = compararDisponivel(self->root, disponibilidade);
    if(lista != NULL){
        return (PyObject *)pyListaEncadeadaFromListaEncadeada(lista);
    }
    Py_RETURN_NONE;
}

static PyObject*
ListaEncadeada_obterTituloIgual(PyListaEncadeada *self, PyObject *args, PyObject *kwds){
    static char *kwlist[] = {
        "titulo",
        NULL
    };

    const char* titulo;

    if(
        !PyArg_ParseTupleAndKeywords(
            args,
            kwds,
            "s",
            kwlist,
            &titulo
        )
    ){
        return NULL;
    }

    ListaEncadeada *lista = compararTituloIgual(self->root, titulo);
    if(lista != NULL){
        return (PyObject *)pyListaEncadeadaFromListaEncadeada(lista);
    }
    Py_RETURN_NONE;
}

static PyObject*
ListaEncadeada_obterTituloDentro(PyListaEncadeada *self, PyObject *args, PyObject *kwds){
    static char *kwlist[] = {
        "titulo",
        NULL
    };

    const char* titulo;

    if(
        !PyArg_ParseTupleAndKeywords(
            args,
            kwds,
            "s",
            kwlist,
            &titulo
        )
    ){
        return NULL;
    }

    ListaEncadeada *lista = compararTituloDentro(self->root, titulo);
    if(lista != NULL){
        return (PyObject *)pyListaEncadeadaFromListaEncadeada(lista);
    }
    Py_RETURN_NONE;
}


static PyObject*
ListaEncadeada_obterAutorIgual(PyListaEncadeada *self, PyObject *args, PyObject *kwds){
    static char *kwlist[] = {
        "autor",
        NULL
    };

    const char* autor;

    if(
        !PyArg_ParseTupleAndKeywords(
            args,
            kwds,
            "s",
            kwlist,
            &autor
        )
    ){
        return NULL;
    }

    ListaEncadeada *lista = compararAutorIgual(self->root, autor);
    if(lista != NULL){
        return (PyObject *)pyListaEncadeadaFromListaEncadeada(lista);
    }
    Py_RETURN_NONE;
}

static PyObject*
ListaEncadeada_obterAutorDentro(PyListaEncadeada *self, PyObject *args, PyObject *kwds){
    static char *kwlist[] = {
        "autor",
        NULL
    };

    const char* autor;

    if(
        !PyArg_ParseTupleAndKeywords(
            args,
            kwds,
            "s",
            kwlist,
            &autor
        )
    ){
        return NULL;
    }

    ListaEncadeada *lista = compararAutorDentro(self->root, autor);
    if(lista != NULL){
        return (PyObject *)pyListaEncadeadaFromListaEncadeada(lista);
    }
    Py_RETURN_NONE;
}

PyMethodDef PyListaEncadeada_methods[] = {
    {
        "append_at",
        (PyCFunction) ListaEncadeada_append_at,
        METH_VARARGS | METH_KEYWORDS,
        NULL
    },
    {
        "obter_livro_indice",
        (PyCFunction) ListaEncadeada_obterLivroIndice,
        METH_VARARGS | METH_KEYWORDS,
        NULL
    },
    {
        "obter_livro_range",
        (PyCFunction) ListaEncadeada_obterLivroRange,
        METH_VARARGS | METH_KEYWORDS,
        NULL
    },
    {
        "obter_livro_codigo",
        (PyCFunction) ListaEncadeada_obterLivroCodigo,
        METH_VARARGS | METH_KEYWORDS,
        NULL
    },
    {
        "obter_livro_ano_publicacao",
        (PyCFunction) ListaEncadeada_obterAnoPublicacao,
        METH_VARARGS | METH_KEYWORDS,
        NULL
    },
    {
        "obter_livro_disponibilidade",
        (PyCFunction) ListaEncadeada_obterDisponivel,
        METH_VARARGS | METH_KEYWORDS,
        NULL
    },
    {
        "obter_livro_titulo",
        (PyCFunction) ListaEncadeada_obterTituloIgual,
        METH_VARARGS | METH_KEYWORDS,
        NULL
    },
    {
        "obter_livro_titulo_parcial",
        (PyCFunction) ListaEncadeada_obterTituloDentro,
        METH_VARARGS | METH_KEYWORDS,
        NULL
    },
    {
        "obter_livro_autor",
        (PyCFunction) ListaEncadeada_obterAutorIgual,
        METH_VARARGS | METH_KEYWORDS,
        NULL
    },
    {
        "obter_livro_autor_parcial",
        (PyCFunction) ListaEncadeada_obterAutorDentro,
        METH_VARARGS | METH_KEYWORDS,
        NULL
    },
    {NULL}
};


PyMemberDef PyListaEncadeada_members[] = {
    {
        "length",
        T_UINT,
        offsetof(PyListaEncadeada, length),
        READONLY,
        NULL
    },
    {NULL}
};

PyTypeObject PyListaEncadeadaType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "core.ListaEncadeada",
    .tp_basicsize = sizeof(PyListaEncadeada),
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = ListaEncadeada_new,
    .tp_init = (initproc)ListaEncadeada_init,
    .tp_dealloc = (destructor)ListaEncadeada_dealloc,
    .tp_members = PyListaEncadeada_members,
    .tp_methods=PyListaEncadeada_methods
};