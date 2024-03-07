#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "application.h"

// maximum thread count is defined to be 10
#define MAX_THREAD_COUNT 10
#define MINIMUM_PORT 1024
#define MAXIMUM_PORT USHRT_MAX

int parse_args(int argc, char* argv[], struct args* args) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s threads initial_port\n", argv[0]);
        return 1;
    }
    
    char* endptr;
    long thread_count = strtol(argv[1], &endptr, 10);
    if (*endptr != '\0' || thread_count < 1 || thread_count > MAX_THREAD_COUNT) {
        fprintf(stderr, "Invalid thread count: %ld\n", thread_count);
        return 1;
    }

    long initial_port = strtol(argv[2], &endptr, 10);
    if (*endptr != '\0' || initial_port < MINIMUM_PORT || initial_port > MAXIMUM_PORT) {
        fprintf(stderr, "Invalid initial port: %ld\n", initial_port);
        return 1;
    }
    args->thread_count = (int)thread_count;
    args->initial_port = (unsigned short)initial_port;

    return 0;
}
