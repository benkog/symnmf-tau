#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "symnmf.h"

/* Sets rows and cols from a Python list-of-lists matrix. Returns 0 on success, -1 on failure. */
static int get_pylist_matrix_dimensions(PyObject* pylist, int* rows, int* cols)
{
    PyObject *first_row;

    if (!PyList_Check(pylist)) {
        return -1;
    }

    *rows = PyList_Size(pylist);

    first_row = PyList_GetItem(pylist, 0);
    if (!PyList_Check(first_row)) {
        return -1;
    }

    *cols = PyList_Size(first_row);
    
    return 0;
}

/* Returns pointer to C array (size rows × cols) extracted from a Python list-of-lists. */
static double* get_array_from_pylist_matrix(PyObject* pylist, int rows, int cols)
{
    int i, j;
    double *arr;
    PyObject *row, *item;

    if (!PyList_Check(pylist)) {
        return NULL;
    }

    arr = malloc(rows * cols * sizeof(double));
    if (arr == NULL) {
        return NULL;
    }

    for (i = 0; i < rows; i++) {
        row = PyList_GetItem(pylist, i);
        if (!PyList_Check(row)) {
            free(arr);
            return NULL;
        }
        for (j = 0; j < cols; j++) {
            item = PyList_GetItem(row, j);
            if (!PyFloat_Check(item)) {
                free(arr);
                return NULL;
            }
            arr[i * cols + j] = PyFloat_AsDouble(item);
        }
    }

    return arr;
}

/* Returns Python list-of-lists from C array (size rows × cols). */
static PyObject* get_pylist_matrix_from_array(double* arr, int rows, int cols)
{
    int i, j;
    PyObject *pylist, *row;
    
    pylist = PyList_New(rows);

    for (i = 0; i < rows; i++) {
        row = PyList_New(cols);
        for (j = 0; j < cols; j++) {
            PyList_SetItem(row, j, PyFloat_FromDouble(arr[i * cols + j]));
        }
        PyList_SetItem(pylist, i, row);
    }

    return pylist;
}

/* Calls given func on array extracted from Python args and returns result as Python list-of-lists. 
   func can be one of: sym, ddg, norm */
static PyObject* get_pylist_matrix_by_func(PyObject* args, double* (*func)(double*, int, int))
{
    int N, d; 
    PyObject *datapoints_pylist, *pylist_matrix;
    double *datapoints, *matrix; 

    if(!PyArg_ParseTuple(args, "O", &datapoints_pylist)) {
        return NULL; 
    }

    if (get_pylist_matrix_dimensions(datapoints_pylist, &N, &d) != 0) {
        return NULL;
    }

    datapoints = get_array_from_pylist_matrix(datapoints_pylist, N, d);
    matrix = func(datapoints, N, d);
    
    pylist_matrix = get_pylist_matrix_from_array(matrix, N, N); /* The result of sym, ddg or norm is a matrix of size (N * N) */

    free(datapoints);
    free(matrix);
    return pylist_matrix;
}

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

    if(!PyArg_ParseTuple(args, "OOiiid", &H_pylist, &W_pylist, &N, &k, &max_iter, &eps)) {
        return NULL; 
    }

    H = get_array_from_pylist_matrix(H_pylist, N, k);
    W = get_array_from_pylist_matrix(W_pylist, N, N);
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
