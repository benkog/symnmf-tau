#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "symnmf.h"
#include "utils.h"

static PyObject* symnmf_sym(PyObject *self, PyObject *args)
{ 
    return get_pylist_matrix_by_func(args, sym);
}

static PyObject* symnmf_ddg(PyObject *self, PyObject *args)
{ 
    return get_pylist_matrix_by_func(args, ddg);
}

static PyObject* symnmf_norm(PyObject *self, PyObject *args)
{ 
    return get_pylist_matrix_by_func(args, norm);
}

static PyObject* symnmf_symnmf(PyObject *self, PyObject *args)
{ 
    int N, k, max_iter;
    double *H, *W, *symnmf_result ,eps;
    PyObject *H_pylist, *W_pylist, *symnmf_result_pylist;

    if(!PyArg_ParseTuple(args, "OOiiid", &H, &W, &N, &k, &max_iter, &eps)) {
        return NULL; 
    }

    H = get_array_from_pylist_matrix(H, N, k);
    W = get_array_from_pylist_matrix(W, N, N);
    symnmf_result = get_symnmf_result(H, W, N, k, max_iter, eps);

    symnmf_result_pylist = get_pylist_matrix_from_array(symnmf_result, N, k);

    free(H);
    free(W);
    free(symnmf_result);
    return symnmf_result_pylist;
}

static PyMethodDef symnmfMethods[] = {
    {"sym",  (PyCFunction) symnmf_sym, METH_VARARGS, PyDoc_STR("Returns the similarity matrix of the symNMF algorithm.")},
    {"ddg", (PyCFunction) symnmf_ddg, METH_VARARGS, PyDoc_STR("Returns the diagonal degree matrix of the symNMF algorithm.")},
    {"norm", (PyCFunction) symnmf_norm, METH_VARARGS, PyDoc_STR("Returns the normalized similarity matrix of the symNMF algorithm.")},
    {"symnmf", (PyCFunction) symnmf_symnmf, METH_VARARGS, PyDoc_STR("Returns the result of the symNMF algorithm.")},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef symnmfmodule = {
    PyModuleDef_HEAD_INIT,
    "symnmfmodule",
    NULL, 
    -1,  
    symnmfMethods
};

PyMODINIT_FUNC PyInit_symnmfmodule(void)
{
    PyObject *m;
    m = PyModule_Create(&symnmfmodule);
    if (!m) {
        return NULL;
    }
    return m;
}
