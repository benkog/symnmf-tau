#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "utils.h"

double* get_sym_matrix(double* datapoints, int N, int d)
{
    int i, j;
    double euc_distance, sym_value;
    double* A;

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

int main(int argc, char **argv) 
{
    
    char* goal;
    char* path;
    int N, d;
    double* datapoints, *A;

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
    printf("A matrix:\n");
    print_matrix(A, N, N);
    free(datapoints);
    free(A);
    return 0;
}
