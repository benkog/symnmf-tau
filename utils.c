#include <stdio.h>
#include <stdlib.h>
#include "util.h"

int get_datapoints_dimensions(char* path, int* N, int* d)
{
    FILE* file;
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
    double* datapoints;
    FILE* file;
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

void print_matrix(double* mat, int N, int d)
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < d; j++) {
            printf("%.4f", mat[i * d + j]);
            if (j < d - 1) {
                printf(",");
            }
        }
        printf("\n");
    }
}
