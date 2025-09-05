#include <stdio.h>
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
