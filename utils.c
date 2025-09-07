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
        return -1;
    }

    while ((curr_char = fgetc(file)) != EOF) { /* Count number of commas in a row */
        if (curr_char == ',') {
            num_of_commas++;
        } else if (curr_char == '\n') {
            break;
        }
    }

    /* The number of elements in each rows is the number of commas in a row + 1 */
    *d = num_of_commas + 1; 
                                            
    fseek(file, 0, SEEK_SET);

    while ((curr_char = fgetc(file)) != EOF) { /* count number of new lines */
        if (curr_char == '\n') {
            num_of_rows++;
        } 
        last_char = curr_char;
    }

    /* If the file does not end on a new line, then we did not count the last row. */
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
        return NULL;
    }

    file = fopen(path, "r");
    if (!file) {
        free(datapoints);
        return NULL;
    }

    for (i = 0; i < N; i++) {
        for (j = 0; j < d; j++) {
            fscanf(file, "%lf", &datapoints[i * d + j]); 
            fgetc(file);    /* Move one char forward, could be a comma or a new line */
        }
    }

    return datapoints;
}

int print_matrix(double* mat, int rows, int cols)
{
    int i, j;

    if (mat == NULL) {
        return -1;
    }

    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            printf("%.4f", mat[i * cols + j]);
            if (j < cols - 1) {
                printf(",");
            }
        }
        printf("\n");
    }

    return 0;
}

double get_sqrd_euc_distance(double* vec1, double* vec2, int d)
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
            target[i * mat2_cols + j] = 0.0; /* Reset element i,j to zero, in case there was something there before or its uninitialized */
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

double frobenius_norm_sqr_diff(double* mat1, double* mat2, int rows, int cols)
{
    int i;
    double sum = 0.0, diff;

    for (i = 0; i < rows * cols; i++) {
        diff = mat1[i] - mat2[i];
        sum += diff * diff;
    }

    return sum;
}

void transpose_mat(double* mat, double* target, int rows, int cols)
{
    int i, j;

    for (i = 0; i < rows; ++i) {
        for (j = 0; j < cols; ++j) {
            target[j * rows + i] = mat[i * cols + j];
        }
    }
}
