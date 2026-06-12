#include <python3.12/Python.h>
#include <python3.12/pytypedefs.h>
#include <python3.12/structmember.h>
#include <stdint.h>

#include "../include/pyLivro.h"
#include "../include/pyListaEncadeada.h"
#include "../include/pyHistorico.h"


static struct PyModuleDef core = {
    PyModuleDef_HEAD_INIT,
    "core",
    NULL,
    -1,
    NULL
};

PyMODINIT_FUNC PyInit_core(void) {
    PyObject *m;

    PyTypeObject* modulos[] = {
        &PyLivroType,
        &PyListaEncadeadaType,
        &PyHistoricoType,
    };
    const size_t qntModulos =sizeof(modulos) / sizeof(modulos[0]);

    for(uint32_t i=0; i < qntModulos;i++){
        if (PyType_Ready(modulos[i]) < 0)
            return NULL;
    }

    m = PyModule_Create(&core);
    if (m == NULL)
        return NULL;

    for(uint32_t i=0; i < qntModulos;i++){
        if (PyModule_AddType(m, modulos[i]) < 0) {
            Py_DECREF(m);
            return NULL;
        }
    }

    return m;
}