#ifndef PYTHON_UTILS_H_
#define PYTHON_UTILS_H_

#include<Python.h>

/* Sets rows and cols from a Python list-of-lists matrix. Returns 0 on success, -1 on failure. */
int get_pylist_matrix_dimensions(PyObject* pylist, int* rows, int* cols);

/* Returns pointer to C array (size rows × cols) extracted from a Python list-of-lists. */
double* get_array_from_pylist_matrix(PyObject* pylist, int rows, int cols);

/* Returns Python list-of-lists from C array (size rows × cols). */
PyObject* get_pylist_matrix_from_array(double* arr, int rows, int cols);

/* Calls given func on array extracted from Python args and returns result as Python list-of-lists. */
PyObject* get_pylist_matrix_by_func(PyObject* args, double* (*func)(double*, int, int));

#endif