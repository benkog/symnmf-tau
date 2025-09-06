#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "util.h"

int get_datapoints_dimensions(char* path, int* N, int* d)
{
    FILE *file;
    int num_of_commas = 0, num_of_rows = 0, last_char;
    int curr_char;

    file = fopen(path, "r");

    if (!file) {
        printf("An Error Has Occurred");
        return -1;
    }

    while ((curr_char = fgetc(file)) != EOF) {
        if (curr_char == ',') {
            num_of_commas++;
        } else if (curr_char == '\n') {
            break;
        }
    }

    *d = num_of_commas + 1;

    fseek(file, 0, SEEK_SET);

    while ((curr_char = fgetc(file)) != EOF) {
        if (curr_char == '\n') {
            num_of_rows++;
        } 
        last_char = curr_char;
    }

    /* If the file does not end on a new line, then we forgot to count that row. */
    *N = (last_char == '\n') ? num_of_rows : (num_of_rows + 1);

    fclose(file);
    return 0;
}

double* get_datapoints(char* path, int N, int d)
{
    double *datapoints;
    FILE *file;
    int i, j;

    datapoints = malloc(N * d * sizeof(double));
    if (datapoints == NULL) {
        printf("An Error Has Occurred");
        return NULL;
    }

    file = fopen(path, "r");
    if (!file) {
        printf("An Error Has Occurred");
        free(datapoints);
        return NULL;
    }

    for (i = 0; i < N; i++) {
        for (j = 0; j < d; j++) {
            fscanf(file, "%lf", &datapoints[i * d + j]);
            fgetc(file);
        }
    }

    return datapoints;
}

void print_matrix(double* mat, int rows, int cols)
{
    int i, j;

    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            printf("%.4f", mat[i * cols + j]);
            if (j < cols - 1) {
                printf(",");
            }
        }
        printf("\n");
    }
}

double get_euc_distance(double* vec1, double* vec2, int d)
{
    int i;
    double distance = 0, diff;

    for (i = 0; i < d; i++) {
        diff = vec1[i] - vec2[i];
        distance += diff * diff;
    }

    return distance;
}

void diag_matrix_pow(double* mat, int N, double exp)
{
    int i;

    for (i = 0; i < N; i++) {
        mat[i * N + i] = pow(mat[i * N + i], exp);
    }
}

void mult_mats(double* mat1, double* mat2, double* target, int mat1_rows, int mat1_cols, int mat2_cols)
{
    int i, j, k;

    for (i = 0; i < mat1_rows; i++) {
        for (j = 0; j < mat2_cols; j++) {
            for (k = 0; k < mat1_cols; k++) {
                target[i * mat2_cols + j] += mat1[i * mat1_cols + k] * mat2[k * mat2_cols + j];
            }
        }
    }
}

void mult_sqr_mats(double* mat1, double* mat2, double* target, int N)
{
    mult_mats(mat1, mat2, target, N, N, N);
}
