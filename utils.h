#ifndef UTILS_H_
#define UTILS_H_

/* Sets N and d by reading dimensions from CSV at given path. Returns 0 on success, -1 on failure. */
int get_datapoints_dimensions(char* path, int* N, int* d);

/* Returns pointer to data array (size N × d) loaded from CSV. Returns NULL on failure. */
double* get_datapoints(char* path, int N, int d);

/* Prints matrix (rows × cols) to stdout in CSV format. Returns 0 on success, -1 on failure. */
int print_matrix(double* mat, int rows, int cols);

/* Returns squared Euclidean distance between two vectors of dimension d. */
double get_sqrd_euc_distance(double* vec1, double* vec2, int d);

/* Raises diagonal elements of matrix (size N × N) to the given power, in-place. */
void diag_matrix_pow(double* mat, int N, double exp);

/* Multiplies two N × N matrices and stores result in pre-allocated target. */
void mult_sqr_mats(double* mat1, double* mat2, double* target, int N);

/* Multiplies mat1 (mat1_rows × mat1_cols) with mat2 (mat1_cols × mat2_cols), stores result in target (mat1_rows × mat2_cols). */
void mult_mats(double* mat1, double* mat2, double* target, int mat1_rows, int mat1_cols, int mat2_cols);

/* Returns squared Frobenius norm of (mat1 - mat2) for matrices of size rows × cols. */
double frobenius_norm_sqr_diff(double* mat1, double* mat2, int rows, int cols);

/* Stores transpose of mat (rows × cols) into target (cols × rows). */
void transpose_mat(double* mat, double* target, int rows, int cols);

#endif
