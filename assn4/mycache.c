#include <stdlib.h>
#include <getopt.h>
#include <stdio.h>
#include <math.h>

//Cache parameters
struct Parameters {
    int s; // Number of set index bits
    int E; // number of lines per set
    int b; // Number of block bits
    int hits;
    int misses;
    int evictions;
};

int main(int argc, char **argv) {
    struct Parameters param = {0, 0, 0, 0, 0, 0};
    char *traceFile;
    char option;

    while ((option = getopt(argc, argv, "s:E:b:t:")) != -1) {
        switch (option) {
            case 's':
                param.s = atoi(optarg);
                break;
            case 'E':
                param.E = atoi(optarg);
                break;
            case 'b':
                param.b = atoi(optarg);
                break;
            case 't':
                traceFile = optarg;
                break;
            default:
                printf("Invalid\n");
                exit(1);
        }
    }

    if (param.s == 0 || param.E == 0 || param.b == 0 || traceFile == NULL) {
        printf("Missing required arguments\n");
        exit(1);
    }
}