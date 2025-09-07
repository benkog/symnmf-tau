#include <stdlib.h>
#include "python_utils.h"

int get_pylist_matrix_dimensions(PyObject* pylist, int* rows, int* cols)
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

double* get_array_from_pylist_matrix(PyObject* pylist, int rows, int cols)
{
    int i, j;
    double *arr;
    PyObject *row;

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
            arr[i * cols + j] = PyFloat_AsDouble(PyList_GetItem(row, j));
        }
    }

    return arr;
}

PyObject* get_pylist_matrix_from_array(double* arr, int rows, int cols)
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

PyObject* get_pylist_matrix_by_func(PyObject* args, double* (*func)(double*, int, int))
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
