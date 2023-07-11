#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>

int computeMinimumAttempts(int numEggs, int numFloors, int **dp) {
    if (numFloors == 0 || numFloors == 1 || numEggs == 1) {
        return numFloors;
    }
    if (dp[numEggs][numFloors] != -1) {
        return dp[numEggs][numFloors];
    }
    int result = INT_MAX;
    for (int i = 1; i <= numFloors; i++) {
        int temp = 1 + fmax(computeMinimumAttempts(numEggs-1, i-1, dp), computeMinimumAttempts(numEggs, numFloors-i, dp));
        result = fmin(result, temp);
    }
    dp[numEggs][numFloors] = result;
    return result;
}

int eggDrop(int numEggs, int numFloors, bool verbose, FILE *outFile) {
    int **dp = (int **)malloc((numEggs+1) * sizeof(int *));
    for (int i = 0; i <= numEggs; i++) {
        dp[i] = (int *)malloc((numFloors+1) * sizeof(int));
        memset(dp[i], -1, (numFloors+1) * sizeof(int));
    }

    int result = computeMinimumAttempts(numEggs, numFloors, dp);

    if (verbose) {
        printf("Memoization Table:\n");
        for (int i = 1; i <= numEggs; i++) {
            printf("NumEggs=%d: ", i);
            for (int j = 1; j <= numFloors; j++) {
                printf("%2d ", dp[i][j]);
            }
            printf("\n");
        }
    }

    if (outFile != NULL) {
        fprintf(outFile, "Memoization Table:\n");
        for (int i = 1; i <= numEggs; i++) {
            fprintf(outFile, "NumEggs=%d: ", i);
            for (int j = 1; j <= numFloors; j++) {
                fprintf(outFile, "%2d ", dp[i][j]);
            }
            fprintf(outFile, "\n");
        }
    }

    for (int i = 0; i <= numEggs; i++) {
        free(dp[i]);
    }
    free(dp);

    return result;
}

int main(int argc, char *argv[]) {
    int numEggs, numFloors;
    bool verbose = false;
    FILE *outFile = NULL;

    if (argc < 3) {
        printf("Usage: %s <numEggs> <numFloors> [-V] [-O outputFile]\n", argv[0]);
        return 1;
    }

    numEggs = atoi(argv[1]);
    numFloors = atoi(argv[2]);

    if (argc >= 4) {
        for (int i = 3; i < argc; i++) {
            if (strcmp(argv[i], "-V") == 0) {
                verbose = true;
            } else if (strcmp(argv[i], "-O") == 0 && i+1 < argc) {
                outFile = fopen(argv[i+1], "w");
                if (outFile == NULL) {
                    printf("Error: Failed to open output file %s\n", argv[i+1]);
                    return 1;
                }
                i++;
            } else {
                printf("Error: Unknown option %s\n", argv[i]);
                return 1;
            }
        }
    }

    if (numEggs <= 0 || numFloors <= 0) {
        printf("Error: Number of eggs and floors must be positive integers\n");
        return 1;
    }

    clock_t startTime = clock();
    int result = eggDrop(numEggs, numFloors, verbose, outFile);
    clock_t endTime = clock();

    double timeTaken = ((double)(endTime - startTime)) / CLOCKS_PER_SEC;

    printf("Minimum number of attempts required: %d\n", result);
    printf("Time taken by the program: %.6f seconds\n", timeTaken);
    printf("Number of clock cycles taken by the program: %ld\n", endTime - startTime);

    if (outFile != NULL) {
        fclose(outFile);
    }

    return 0;
}
