/**
 * @author: Anupam Khargharia
 * Assn 4: CPSC 275
 * C Program that simulates the behaviour of a cache memory (Verbose also included)
 * Version: 1.0
*/

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>

// Global clock for LRU implementation
int globalClock = 0;

//Cache parameters
struct Parameters {
    int v; //verbose or not
    int s; // set index bits
    int E; // lines per set
    int b; //block bits
    int hits;
    int misses;
    int evictions;
};

// Struct for a line in a cache set
struct Line {
    int valid;
    unsigned long long int tag;
    unsigned long long lastUsed; // Timestamp for LRU
};

// Struct for a cache set
struct Set {
    struct Line *lines;
};

// Struct for the cache
struct Cache {
    struct Set *sets;
};

// Function to initialize the cache
struct Cache initializeCache(int S, int E);

// Function to simulate cache access
void accessCache(struct Cache *cache, struct Parameters *param, unsigned long long int address);

// Initialize the cache using the input param from command line. Then access all elements using the accessCache function and print.
int main(int argc, char **argv) {
    struct Parameters param = {0, 0, 0, 0, 0, 0, 0};
    char *traceFile;
    char option;

    while ((option = getopt(argc, argv, "s:E:b:t:v")) != -1) {
        switch (option) {
            case 's': //set index bits
                param.s = atoi(optarg);
                break;
            case 'E': // lines per set
                param.E = atoi(optarg);
                break;
            case 'b': //block bits
                param.b = atoi(optarg);
                break;
            case 'v':
                param.v = 1; //verbose or not
                break;
            case 't':
                traceFile = optarg;
                break;
            default:
                printf("Invalid\n");
                exit(1);
        }
    }

    //invalid Cache
    if (param.s == 0 || param.E == 0 || param.b == 0 || traceFile == NULL) {
        printf("Missing required arguments\n");
        exit(1);
    }

    int S = (1 << param.s);

    struct Cache cache = initializeCache(S, param.E);

    FILE *fp = fopen(traceFile, "r");
    if (fp == NULL) {
        printf("Could not open trace file\n");
        exit(1);
    }

    char operation;
    int size;
    unsigned long long int address;
    while (fscanf(fp, " %c %llx,%d", &operation, &address, &size) == 3) {
            if (param.v==1){
                printf("\n %c, %llx, %d ", operation, address, size);
            }
            accessCache(&cache, &param, address);
            if (operation == 'M') {
                accessCache(&cache, &param, address); // M operation so do it twice
            }
    }

    fclose(fp);
    if (param.v==1){
        printf("\n");
    }
    printf("hits:%d misses:%d evictions:%d\n", param.hits, param.misses, param.evictions);

    return 0;
}

// Function to initialize the cache
struct Cache initializeCache(int S, int E) {
    struct Cache cache;
    cache.sets = (struct Set *) malloc(S * sizeof(struct Set));
    for (int i = 0; i < S; i++) {
        cache.sets[i].lines = (struct Line *) malloc(E * sizeof(struct Line));
        for (int j = 0; j < E; j++) {
            cache.sets[i].lines[j].valid = 0;
            cache.sets[i].lines[j].tag = 0;
        }
    }
    return cache;
}

// Function to simulate cache access
void accessCache(struct Cache *cache, struct Parameters *param, unsigned long long int address) {
    int tagSize = 64 - (param->s + param->b);
    unsigned long long int tag = address >> (param->s + param->b);
    int setIndex = (address << tagSize) >> (tagSize + param->b);
    struct Set set = cache->sets[setIndex];

    int hit = 0;
    int leastRecentlyUsedIndex = 0;
    unsigned long long oldestTimestamp = ~0ULL; // max possible value

    for (int i = 0; i < param->E; i++) {
        if (set.lines[i].valid) {
            if (set.lines[i].tag == tag) {
                hit = 1;
                param->hits++;
                set.lines[i].lastUsed = globalClock; // Update timestamp on hit
                if (param->v==1){
                    printf("hit ");
                }
                break;
            }
            if (set.lines[i].lastUsed < oldestTimestamp) {
                oldestTimestamp = set.lines[i].lastUsed;
                leastRecentlyUsedIndex = i;
            }
        } else if (oldestTimestamp == ~0ULL) {
            leastRecentlyUsedIndex = i; // If the line is invalid, it's a candidate for eviction
        }
    }

    if (!hit) {
        param->misses++;
        if (param->v==1){
            printf("miss ");
        }
        // Replace the least recently used line
        set.lines[leastRecentlyUsedIndex].valid = 1;
        set.lines[leastRecentlyUsedIndex].tag = tag;
        set.lines[leastRecentlyUsedIndex].lastUsed = globalClock;
        // If an eviction happens
        if (oldestTimestamp != ~0ULL) {
            param->evictions++;
            if (param->v==1){
                printf("eviction ");
            }
        }
    }
    globalClock++; // Increment global clock after each memory access
}

