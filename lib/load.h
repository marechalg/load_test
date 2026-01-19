#ifndef LOAD_H
#define LOAD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static inline void laod(char*, int, int, int);

static inline void load(char* target, int threads, int connections, int duration) {
    char wrk_command[256];
    snprintf(wrk_command, sizeof(wrk_command), "wrk -t%d -c%d -d%ds %s", threads, connections, duration, target);
    
    char command[1024];
    snprintf(command, sizeof(command), "mkdir -p ./output/ && %s | tee ./output/unsaved", wrk_command);
    
    int result = system(command);

    if (result == 0) {
        printf("\nDone.\n");
    } else {
        printf("Failed with exit code %d\n", result);
    }
}

#endif