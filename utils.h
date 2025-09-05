#ifndef UTILS_H_
#define UTILS_H_

int get_datapoints_dimensions(char* path, int* N, int* d);
double* get_datapoints(char* path, int N, int d);
void print_matrix(double* mat, int n, int d);

#endif
