#ifndef UTILS_H_
#define UTILS_H_

/**
 * @brief Get the number of datapoints and their dimension from a csv file.
 *
 * @param path Path to the input file containing the data.
 * @param N Number of data points.
 * @param d Dimension of each data point.
 * @return 0 on success, -1 on failure.
 */
int get_datapoints_dimensions(char* path, int* N, int* d);

/**
 * @brief Loads datapoints from a csv file.
 *
 * @param path Path to the input file containing the data.
 * @param N Number of datapoints (rows).
 * @param d Dimension of each datapoint (columns).
 * @return Pointer to a dynamically allocated array of size N × d.
 *         Returns NULL on failure.
 *
 * @note Caller is responsible for freeing the returned array.
 */
double* get_datapoints(char* path, int N, int d);

/**
 * @brief Prints a matrix to stdout in csv format.
 *
 * @param mat Pointer to the matrix of size rows × cols.
 * @param rows Number of rows.
 * @param cols Number of columns.
 * @return 0 on success, -1 on failure.
 */
int print_matrix(double* mat, int rows, int cols);

/**
 * @brief Computes the squared Euclidean distance between two vectors.
 *
 * @param vec1 Pointer to the first vector of dimension d.
 * @param vec2 Pointer to the second vector of dimension d.
 * @param d Dimension of the vectors.
 * @return Squared Euclidean distance as a double.
 */
double get_sqrd_euc_distance(double* vec1, double* vec2, int d);

/**
 * @brief Raises each diagonal element of a square matrix to a given power.
 *
 * @param mat Pointer to the square matrix of size N × N.
 * @param N Dimension of the matrix.
 * @param exp Exponent to apply to each diagonal element.
 *
 * @note This function modifies the matrix in-place.
 */
void diag_matrix_pow(double* mat, int N, double exp);

/**
 * @brief Multiplies two square matrices of size N × N.
 *
 * @param mat1 Pointer to the first matrix.
 * @param mat2 Pointer to the second matrix.
 * @param target Pointer to the target matrix to store the result.
 * @param N Dimension of the square matrices.
 *
 * @note Result is stored in `target`, which must be pre-allocated.
 */
void mult_sqr_mats(double* mat1, double* mat2, double* target, int N);

/**
 * @brief Multiplies two matrices: (mat1 of size rows × cols1) and (mat2 of size cols1 × cols2).
 *
 * @param mat1 Pointer to the first matrix.
 * @param mat2 Pointer to the second matrix.
 * @param target Pointer to the result matrix (size rows × cols2).
 * @param mat1_rows Number of rows in mat1.
 * @param mat1_cols Number of columns in mat1 (and rows in mat2).
 * @param mat2_cols Number of columns in mat2.
 *
 * @note Result is stored in `target`, which must be pre-allocated.
 */
void mult_mats(double* mat1, double* mat2, double* target, int mat1_rows, int mat1_cols, int mat2_cols);

/**
 * @brief Computes the squared Frobenius norm of the difference between two matrices.
 *
 * @param mat1 Pointer to the first matrix.
 * @param mat2 Pointer to the second matrix.
 * @param rows Number of rows in each matrix.
 * @param cols Number of columns in each matrix.
 * @return The squared Frobenius norm of (mat1 - mat2).
 */
double frobenius_norm_sqr_diff(double* mat1, double* mat2, int rows, int cols);

/**
 * @brief Computes the transpose of a matrix.
 *
 * @param mat Pointer to the input matrix of size rows × cols.
 * @param target Pointer to the output matrix of size cols × rows.
 * @param rows Number of rows in the input matrix.
 * @param cols Number of columns in the input matrix.
 *
 * @note Result is stored in `target`, which must be pre-allocated.
 */
void transpose_mat(double* mat, double* target, int rows, int cols);

#endif
