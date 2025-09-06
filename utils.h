#ifndef UTILS_H_
#define UTILS_H_

int get_datapoints_dimensions(char* path, int* N, int* d);
double* get_datapoints(char* path, int N, int d);
void print_matrix(double* mat, int rows, int cols);
double get_euc_distance(double* vec1, double* vec2, int d);
void diag_matrix_pow(double* mat, int N, double exp);
void mult_sqr_mats(double* mat1, double* mat2, double* target, int N);
void mult_mats(double* mat1, double* mat2, double* target, int mat1_rows, int mat1_cols, int mat2_cols);
double frobenius_norm_sqr_diff(double* mat1, double* mat2, int rows, int cols);
void transpose_mat(double* mat, double* target, int rows, int cols);

#endif
