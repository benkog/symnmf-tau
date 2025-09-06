#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "utils.h"

typedef enum { sym_a, ddg_a, norm_a, invalid_a } goal;

goal get_goal_from_arg(char* arg)
{
    if (strcmp(arg, "sym") == 0) return sym_a;
    if (strcmp(arg, "ddg") == 0) return ddg_a;
    if (strcmp(arg, "norm") == 0) return norm_a;
    return invalid_a;
}

double* get_sym_matrix(double* datapoints, int N, int d)
{
    int i, j;
    double euc_distance, sym_value;
    double *A;

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
                euc_distance = get_euc_distance((datapoints + i * d), (datapoints + j * d), d);
                sym_value = exp(-0.5 * euc_distance);
                A[i * N + j] = sym_value;
                A[j * N + i] = sym_value;
            }   
        }
    }

    return A;
}

double* get_ddg_matrix(double* sym_matrix, int N)
{
    int i, j;
    double edge_sum = 0;
    double *D;

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
    double *W, *tmp_mat;

    W = malloc(N * N * sizeof(double));
    tmp_mat = malloc(N * N * sizeof(double));

    if (W == NULL || tmp_mat == NULL || sym_matrix == NULL || ddg_matrix == NULL) {
        free(W); free(tmp_mat);
        return NULL;
    }

    diag_matrix_pow(ddg_matrix, N, -0.5); /* D^{-0.5} */
    mult_sqr_mats(ddg_matrix, sym_matrix, tmp_mat, N); /* D^{-0.5} * A */
    mult_sqr_mats(tmp_mat, ddg_matrix, W, N); /* D^{-0.5} * A * D^{-0.5} */

    free(tmp_mat);
    return W;
}

double* get_symnmf_matrix(double* initial_H_matrix, double* norm_matrix, int N, int k)
{   
    /*
    * TODO: add comments explainig the variables.
    */
    double *H, *H_next, *H_T, *HH_T, *HH_TH, *WH, *tmp, *W = norm_matrix, beta = 0.5, eps = 1e-4;
    int i, j, index, val, max_iter = 300, curr_iter = 0;

    H = malloc(N * k * sizeof(double));
    H_next = malloc(N * k * sizeof(double));
    H_T = malloc(N * k * sizeof(double));
    HH_T = malloc(N * N * sizeof(double));
    HH_TH = malloc(N * k * sizeof(double));
    WH = malloc(N * k * sizeof(double));

    if (H_next == NULL || H_T == NULL || HH_T == NULL || HH_TH == NULL || WH == NULL || initial_H_matrix == NULL || norm_matrix == NULL) {
        free(H_next); free(H_T); free(HH_T); free(HH_TH); free(WH);
        return NULL;
    }

    memcpy(H, initial_H_matrix, N * k * sizeof(double));

    do {
        transpose_mat(H, H_T, N, k);
        mult_mats(H, H_T, HH_T, N, k, N);
        mult_mats(HH_T, H, HH_TH, N, N, k);
        mult_mats(W, H, WH, N, N, k);

        for (i = 0; i < N; i++) {
            for (j = 0; j < k; j++) {
                index = i * k + j;
                val = H[index] * (1 - beta + beta * (WH[index] / ((HH_TH == 0) ? 1e-6 : HH_TH[index])));
                H_next[index] = val;
            }
        }

        /* Swap the pointers so H points to H_next for next iteration */
        tmp = H;
        H = H_next;
        H_next = tmp;

    } while (curr_iter < max_iter && frobenius_norm_sqr_diff(H, H_next, N, k) >= eps);
    
    free(H_next); free(H_T); free(HH_T); free(HH_TH); free(WH);
    return H;
}

int print_goal(char* path, goal goal, double* datapoints, double* sym_matrix, double* ddg_matrix, double* norm_matrix)
{
    int N, d;

    if (get_datapoints_dimensions(path, &N, &d) != 0) {
        return -1;
    }

    datapoints = get_datapoints(path, N, d);
    sym_matrix = get_sym_matrix(datapoints, N, d);
    if (goal == sym_a) return print_matrix(sym_matrix, N, N);
    
    ddg_matrix = get_ddg_matrix(sym_matrix, N);
    if (goal == ddg_a) return print_matrix(ddg_matrix, N, N);

    norm_matrix = get_norm_matrix(sym_matrix, ddg_matrix, N);
    if (goal == norm_a) return print_matrix(norm_matrix, N, N);

    return -1;
}

double* sym(double* datapoints, int N, int d)
{
    double *sym_matrix;

    sym_matrix = get_sym_matrix(datapoints, N, d);

    return sym_matrix;
}

double* ddg(double* datapoints, int N, int d)
{
    double *sym_matrix, *ddg_matrix;

    sym_matrix = get_sym_matrix(datapoints, N, d);
    ddg_matrix = get_ddg_matrix(sym_matrix, N);

    free(sym_matrix);
    return ddg_matrix;
}

double* norm(double* datapoints, int N, int d)
{
    double *sym_matrix, *ddg_matrix, *norm_matrix;

    sym_matrix = get_sym_matrix(datapoints, N, d);
    ddg_matrix = get_ddg_matrix(sym_matrix, N);
    norm_matrix = get_norm_matrix(sym_matrix, ddg_matrix, N);

    free(sym_matrix); free(ddg_matrix);
    return norm_matrix;
}

double* symnmf(double* datapoints, double* H, int N, int d, int k)
{
    double *sym_matrix, *ddg_matrix, *norm_matrix, *symnmf_matrix;

    sym_matrix = get_sym_matrix(datapoints, N, d);
    ddg_matrix = get_ddg_matrix(sym_matrix, N);
    norm_matrix = get_norm_matrix(sym_matrix, ddg_matrix, N);
    symnmf_matrix = get_symnmf_matrix(H, norm_matrix, N, k);

    free(sym_matrix); free(ddg_matrix); free(norm_matrix);
    return symnmf_matrix;
}

int print_matrix_from_func(double* (*func)(char*, int*, int*), double* datapoints, int N, int d)
{
    int status;
    double* matrix;

    matrix = func(datapoints, N, d);
    status = print_matrix(matrix, N, N);
    free(matrix);

    return status;
}

int print_goal_matrix(goal goal, double* datapoints, int N, int d)
{
    int status;

    status = -1;

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
