#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/// argument struct
struct args {
    int thread_count;
    unsigned short initial_port;
};

/**
 * Parse the command line arguments
 * @param argc The number of arguments
 * @param argv The arguments
 * @param args The parsed arguments
 */
int parse_args(int argc, char* argv[], struct args* args);

#ifdef __cplusplus
}
#endif
