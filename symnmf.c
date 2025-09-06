#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "utils.h"

double* get_sym_matrix(double* datapoints, int N, int d)
{
    int i, j;
    double euc_distance, sym_value;
    double *A;

    A = malloc(N * N * sizeof(double));
    if (A == NULL) {
        printf("An Error Has Occurred");
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
    if (D == NULL) {
        printf("An Error Has Occurred");
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

    if (W == NULL || tmp_mat == NULL) {
        printf("An Error Has Occurred");
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

    if (H_next == NULL || H_T == NULL || HH_T == NULL || HH_TH == NULL || WH == NULL) {
        printf("An Error Has Occurred");
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

int main(int argc, char **argv) 
{
    
    char* goal;
    char* path;
    int N, d;
    double* datapoints, *A, *D, *W;

    if(argc != 3) {
        printf("An Error Has Occurred");
        return 0;
    }

    goal = argv[1];
    path = argv[2];

    printf("goal: %s\n", goal);
    /* printf("path: %s\n", path); */

    if (get_datapoints_dimensions(path, &N, &d) != 0)
    {
        return -1;
    }

    /* printf("N: %d\n", N); */
    /* printf("d: %d\n", d); */

    datapoints = get_datapoints(path, N, d);
    printf("datapoints matrix:\n");
    print_matrix(datapoints, N, d);
    A = get_sym_matrix(datapoints, N, d);
    printf("sym matrix:\n");
    print_matrix(A, N, N);
    D = get_ddg_matrix(A, N);
    printf("ddg matrix:\n");
    print_matrix(D, N, N);
    printf("norm matrix:\n");
    W = get_norm_matrix(A, D, N);
    print_matrix(W, N, N);
    free(datapoints);
    free(A);
    free(D);
    free(W);
    return 0;
}
