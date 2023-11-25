#include "../header/error_hand.h"
#include "../header/book.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

const char *errorMessages[][2] = {
    { "Memory allocation failed", "Error code: 1" },
    { "Failed to open file", "Error code: 2" },
    { "Failed Writing to FILE", "Error code: 3" },
    { "Invalid File Name: must contain .book as suffix", "Error code: 4" },
    { "Failed to get value", "Error code: 5"},
    { "--Invalid Input--", "" },
};

void print_error(uint32_t errorCode, char *varName, bool IFEXIT) {
    
    char *errorCodeStr = malloc(sizeof(errorCode));

    sprintf(errorCodeStr, "%d", errorCode);

    uint32_t numErrors = sizeof(errorMessages) / sizeof(errorMessages[0]);

    for (uint32_t i = 0; i < numErrors; ++i) {
        if (strstr(errorMessages[i][1], errorCodeStr) || (!strcmp(errorMessages[i][1], ""))) {
            fprintf(stderr, "Error: %s for %s\n%s\n", errorMessages[i][0], varName, errorMessages[i][1]);
            if(IFEXIT == true) {
                free(errorCodeStr);
                exit(errorCode);
            }
        }
    }
    if(IFEXIT) {
        fprintf(stderr, "Unknown error code: %d\n", errorCode);
        exit(errorCode);
        free(errorCodeStr);
    }
}
