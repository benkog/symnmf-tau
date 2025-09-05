#ifndef UTILS_H_
#define UTILS_H_

int get_datapoints_dimensions(char* path, int* N, int* d);
double* read_datapoints_file(char* path, int* N, int* d);
void print_mat(double* mat, int* n, int* d);

#endif
