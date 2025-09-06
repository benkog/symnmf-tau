#include <stdio.h>
#include <stdlib.h>
#include <math.h>
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

    /* Initialize the D matrix with zeros */
    D = calloc(N * N, sizeof(double));
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

    W = calloc(N * N, sizeof(double));
    if (W == NULL) {
        printf("An Error Has Occurred");
        return NULL;
    }

    tmp_mat = calloc(N * N, sizeof(double));
    if (tmp_mat == NULL) {
        printf("An Error Has Occurred");
        free(W);
        return NULL;
    }

    diag_matrix_pow(ddg_matrix, N, -0.5);
    mult_sqr_mats(ddg_matrix, sym_matrix, tmp_mat, N); /* Compute D^{-0.5}_A */
    mult_sqr_mats(tmp_mat, ddg_matrix, W, N); /* Compute D^{-0.5}_A_D^{-0.5} */

    free(tmp_mat);
    return W;
}

double* get_H_matrix(double* initial_H_matrix, double* norm_matrix, int N, int k)
{
    
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
