#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

int main(int argc, char **argv) {
    
    char* goal;
    char* path;
    int N, d;
    double* datapoints;

    if(argc != 3) {
        printf("An Error Has Occurred");
        return 0;
    }

    goal = argv[1];
    path = argv[2];

    printf("goal: %s\n", goal);
    printf("path: %s\n", path);

    if (get_datapoints_dimensions(path, &N, &d) != 0)
    {
        return -1;
    }

    printf("N: %d\n", N);
    printf("d: %d\n", d);

    datapoints = get_datapoints(path, N, d);
    print_matrix(datapoints, N, d);
    free(datapoints);
    return 0;
}
