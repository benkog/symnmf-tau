#ifndef SYMMNMF_H_
#define SYMMNMF_H_

/* Returns a pointer to the symmetric similarity matrix (N × N) from datapoints (N × d). */
double* sym(double* datapoints, int N, int d);

/* Returns a pointer to the diagonal degree matrix (N × N) from datapoints (N × d). */
double* ddg(double* datapoints, int N, int d);

/* Returns a pointer to the normalized similarity matrix (N × N) from datapoints (N × d). */
double* norm(double* datapoints, int N, int d);

/* Returns a pointer to the SymNMF result matrix (N × k) using initial H (N * k) and the normalized similarity matrix (N × N). */
double* get_symnmf_result(double* initial_H_matrix, double* norm_matrix, int N, int k, int max_iter, double eps);

#endif