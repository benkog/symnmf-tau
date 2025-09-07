#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "utils.h"

typedef enum { sym_a, ddg_a, norm_a, invalid_a } goal; /* a suffix stands for argument */

goal get_goal_from_arg(char* arg)
{
    if (strcmp(arg, "sym") == 0) return sym_a;
    if (strcmp(arg, "ddg") == 0) return ddg_a;
    if (strcmp(arg, "norm") == 0) return norm_a;
    return invalid_a;
}

double* get_ddg_matrix(double* sym_matrix, int N)
{
    int i, j;
    double edge_sum = 0;
    double *D; /* The diagonal degree matrix to be calculated */

    D = malloc(N * N * sizeof(double));
    if (D == NULL || sym_matrix == NULL) {
        free(D);
        return NULL;
    }

    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            edge_sum += sym_matrix[i * N + j];
        }
        D[i * N + i] = edge_sum;
        edge_sum = 0;
    }

    return D;
}

double* get_norm_matrix(double* sym_matrix, double* ddg_matrix, int N)
{
    double *W; /* The normalized similarity matrix to be calculated */
    double *tmp_mat, *ddg_matrix_cpy;

    W = malloc(N * N * sizeof(double));
    tmp_mat = malloc(N * N * sizeof(double));
    ddg_matrix_cpy = malloc(N * N * sizeof(double));

    if (W == NULL || tmp_mat == NULL || sym_matrix == NULL || ddg_matrix == NULL || ddg_matrix_cpy == NULL) {
        free(W); free(tmp_mat); free(ddg_matrix_cpy);
        return NULL;
    }

    memcpy(ddg_matrix_cpy, ddg_matrix, N * N * sizeof(double)); /* Making a copy to not cause side-effects */

    diag_matrix_pow(ddg_matrix_cpy, N, -0.5); 
    mult_sqr_mats(ddg_matrix_cpy, sym_matrix, tmp_mat, N); 
    mult_sqr_mats(tmp_mat, ddg_matrix_cpy, W, N); 

    free(tmp_mat);
    return W;
}

double* get_symnmf_result(double* initial_H_matrix, double* norm_matrix, int N, int k, int max_iter, double eps)
{   
    double *H, *H_next, *H_T, *HH_T, *HH_TH, *WH, *tmp, *W = norm_matrix, beta = 0.5;
    int i, j, index, curr_iter = 0;

    H = malloc(N * k * sizeof(double));         /* copy of initial H matrix to avoid side effects */
    H_next = malloc(N * k * sizeof(double));    /* stores the result of current iteration */
    H_T = malloc(N * k * sizeof(double));       /* H transposed */
    HH_T = malloc(N * N * sizeof(double));      /* (H transposed) * H */
    HH_TH = malloc(N * k * sizeof(double));     /* H * (H transpoed) * H */
    WH = malloc(N * k * sizeof(double));        /* W * H , W is the normalized similarity matrix*/

    if (H == NULL || H_next == NULL || H_T == NULL || HH_T == NULL || HH_TH == NULL || WH == NULL || initial_H_matrix == NULL || norm_matrix == NULL) {
        free(H); free(H_next); free(H_T); free(HH_T); free(HH_TH); free(WH);
        return NULL;
    }

    memcpy(H, initial_H_matrix, N * k * sizeof(double));

    do {
        transpose_mat(H, H_T, N, k);
        mult_mats(H, H_T, HH_T, N, k, N);
        mult_mats(HH_T, H, HH_TH, N, N, k);
        mult_mats(W, H, WH, N, N, k);

        /* Calculate each element of H_next according to the formula, 
           if the divisor is equal to zero replacing it with 1e-6 */
        for (i = 0; i < N; i++) {
            for (j = 0; j < k; j++) {
                index = i * k + j;
                H_next[index] = H[index] * (1 - beta + beta * (WH[index] / ((HH_TH[index] == 0) ? 1e-6 : HH_TH[index])));
            }
        }

        /* Swap the pointers so H points to H_next and vice versa for next iteration,
           then H_next can be safely overwritten */
        tmp = H;
        H = H_next;
        H_next = tmp;
        curr_iter++;
    } while (curr_iter < max_iter && frobenius_norm_sqr_diff(H, H_next, N, k) >= eps);
    
    free(H_next); free(H_T); free(HH_T); free(HH_TH); free(WH);
    return H;
}

double* sym(double* datapoints, int N, int d)
{
    int i, j;
    double euc_distance, sym_value;
    double *A; /* The similarity matrix to be calculated */

    A = malloc(N * N * sizeof(double));
    if (A == NULL || datapoints == NULL) {
        free(A);
        return NULL;
    }

    for (i = 0; i < N; i++) {
        for (j = i; j < N; j++) {
            if (i == j) {
                A[i * N + j] = 0;
            } else {
                euc_distance = get_sqrd_euc_distance((datapoints + i * d), (datapoints + j * d), d);
                sym_value = exp(-0.5 * euc_distance);
                A[i * N + j] = sym_value;
                A[j * N + i] = sym_value;
            }   
        }
    }

    return A;
}

double* ddg(double* datapoints, int N, int d)
{
    double *sym_matrix, *ddg_matrix;

    sym_matrix = sym(datapoints, N, d);
    ddg_matrix = get_ddg_matrix(sym_matrix, N);

    free(sym_matrix);
    return ddg_matrix;
}

double* norm(double* datapoints, int N, int d)
{
    double *sym_matrix, *ddg_matrix, *norm_matrix;

    sym_matrix = sym(datapoints, N, d);
    ddg_matrix = get_ddg_matrix(sym_matrix, N);
    norm_matrix = get_norm_matrix(sym_matrix, ddg_matrix, N);

    free(sym_matrix); free(ddg_matrix);
    return norm_matrix;
}

/* Calculate the result of one of the following functions: sym, ddg, norm and then print the result */
int print_matrix_from_func(double* (*func)(double*, int, int), double* datapoints, int N, int d)
{
    int status;
    double* result_matrix;

    result_matrix = func(datapoints, N, d);
    status = print_matrix(result_matrix, N, N); /* The result of sym, ddg, or norm is of size (N * N) */
    free(result_matrix);

    return status;
}

int print_goal_matrix(goal goal, double* datapoints, int N, int d)
{
    int status;

    switch (goal)
    {
        case sym_a:
            status = print_matrix_from_func(sym, datapoints, N, d);
            break;
        case ddg_a:
            status = print_matrix_from_func(ddg, datapoints, N, d);
            break;
        case norm_a:
            status = print_matrix_from_func(norm, datapoints, N, d);
            break;
        default:
            status = -1;
            break;
    }

    return status;
}

int main(int argc, char **argv) 
{
    int N, d;
    double *datapoints;
    char* path;
    goal goal;

    if(argc != 3) {
        printf("An Error Has Occurred");
        return -1;
    }

    goal = get_goal_from_arg(argv[1]);
    path = argv[2];

    if (get_datapoints_dimensions(path, &N, &d) != 0) {
        printf("An Error Has Occurred");
        return -1;
    }

    datapoints = get_datapoints(path, N, d);

    if (print_goal_matrix(goal, datapoints, N, d) != 0) {
        printf("An Error Has Occurred");
    }

    free(datapoints);
    return 0;
}
