#ifndef SYMMNMF_H_
#define SYMMNMF_H_

/**
 * @brief Computes the symmetric similarity matrix from the given datapoints.
 *
 * @param datapoints Pointer to the input data array of size N × d.
 * @param N Number of data points.
 * @param d Dimension of each data point.
 * @return Pointer to the symmetric similarity matrix of size N × N.
 *         Returns NULL on failure.
 *
 * @note The caller is responsible for freeing the returned matrix.
 */
double* sym(double* datapoints, int N, int d);

/**
 * @brief Computes the diagonal degree matrix (DDG) based on the symmetric similarity matrix.
 *
 * @param datapoints Pointer to the input data array of size N × d.
 * @param N Number of data points.
 * @param d Dimension of each data point.
 * @return Pointer to the DDG matrix of size N × N.
 *         Returns NULL on failure.
 *
 * @note The caller is responsible for freeing the returned matrix.
 */
double* ddg(double* datapoints, int N, int d);

/**
 * @brief Computes the normalized similarity matrix: D^(-1/2) * A * D^(-1/2),
 *        where D is the diagonal degree matrix and A is the symmetric similarity matrix.
 *
 * @param datapoints Pointer to the input data array of size N × d.
 * @param N Number of data points.
 * @param d Dimension of each data point.
 * @return Pointer to the normalized similarity matrix of size N × N.
 *         Returns NULL on failure.
 *
 * @note The caller is responsible for freeing the returned matrix.
 */
double* norm(double* datapoints, int N, int d);

/**
 * @brief Performs the SymNMF algorithm.
 *
 * @param datapoints Pointer to the input data array of size N × d.
 * @param H Pointer to the initial nonnegative factor matrix of size N × k.
 * @param N Number of data points.
 * @param d Dimension of each data point.
 * @param k Target rank for the factorization.
 * @param max_iter Maximum number of iterations for the optimization algorithm.
 * @param eps Convergence threshold for stopping criterion based on relative Frobenius norm change.
 *
 * @return Pointer to the final nonnegative factor matrix H of size N × k.
 *         Returns NULL on failure.
 *
 * @note The returned matrix is dynamically allocated; the caller must free it.
 */
double* symnmf(double* datapoints, double* H, int N, int d, int k, int max_iter, double eps);

#endif