#ifndef PYTHON_UTILS_H_
#define PYTHON_UTILS_H_

#include<Python.h>

int get_pylist_matrix_dimensions(PyObject* pylist, int* rows, int* cols);
double* get_array_from_pylist_matrix(PyObject* pylist, int rows, int cols);
PyObject* get_pylist_matrix_from_array(double* arr, int rows, int cols);
PyObject* get_pylist_matrix_by_func(PyObject* args, double* (*func)(double*, int, int));

#endif