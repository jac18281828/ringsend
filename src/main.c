#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#include "application.h"
#include "ringsender.h"
#include "network.h"

#define SEED_DATA_FILE "README.md"

static int seed_data_size(const char *filename);
static int seed_ring(int port, const char *filename, int data_buffer_size);

int main(int argc, char* argv[]) {
    struct args args;
    int exit_code = parse_args(argc, argv, &args);

    if (exit_code != EXIT_SUCCESS) {
        exit(exit_code);
    }

    int data_buffer_size = seed_data_size(SEED_DATA_FILE);
    if(data_buffer_size < 0) {
        fprintf(stderr, "Failed to get seed data size\n");
        exit(EXIT_FAILURE);
    }

    struct ring_sender *sender = calloc(args.thread_count, sizeof(struct ring_sender));
    for(int i=0; i<args.thread_count; i++) {
        int sender_port = args.initial_port + i;
        int peer_port = args.initial_port + (i+1) % args.thread_count;
        if(create_ring_sender(sender_port, peer_port, data_buffer_size, &sender[i]) != 0) {
            fprintf(stderr, "Failed to create ring sender on %d\n", sender_port);
            exit_code = EXIT_FAILURE;
        }
        fprintf(stdout, "Created ring sender with port %d and peer port %d\n", sender_port, peer_port);
    }

    if(exit_code == EXIT_SUCCESS && seed_ring(sender[0].port, SEED_DATA_FILE, data_buffer_size) != 0) {
        fprintf(stderr, "Failed to seed ring\n");
        exit_code = EXIT_FAILURE;
    }

    for(int i=0; i<args.thread_count; i++) {
        if (exit_code != EXIT_SUCCESS) {
            cancel_ring_sender(&sender[i]);
        }
        if(destroy_ring_sender(&sender[i]) != 0) {
            fprintf(stderr, "Failed to destroy ring sender\n");
            exit_code = EXIT_FAILURE;
        }
    }

    free(sender);

    if (exit_code != 0) {
        exit(exit_code);
    }
    exit(EXIT_SUCCESS);
}

static int seed_data_size(const char *filename) {
    struct stat st;
    if(stat(filename, &st) != 0) {
        fprintf(stderr, "Failed to stat seed file: %s\n", filename);
        return -1;
    }
    return st.st_size;
}

static int seed_ring(int port, const char *filename, int data_buffer_size) {
    char *data_buffer = malloc(data_buffer_size);
    if(data_buffer == NULL) {
        fprintf(stderr, "Failed to allocate data buffer\n");
        return -1;
    }
    FILE *datafile = fopen(filename, "rb");
    if(datafile == NULL) {
        free(data_buffer);
        fprintf(stderr, "Failed to open seed file: %s\n", filename);
        return -1;
    }
    int n_read;
    int total_read = 0;
    while((n_read = fread(data_buffer + total_read, sizeof(char), data_buffer_size - total_read, datafile)) > 0) {
        total_read += n_read;
    }
    if(ferror(datafile)) {
        fclose(datafile);
        free(data_buffer);
        fprintf(stderr, "Failed to read valid seed file\n");
        return -1;
    }
    if(total_read != data_buffer_size) {
        fclose(datafile);
        free(data_buffer);
        fprintf(stderr, "Failed to read valid seed file\n");
        return -1;
    }
    if(ring_socket_send(port, data_buffer, data_buffer_size) != data_buffer_size) {
        fclose(datafile);
        free(data_buffer);
        fprintf(stderr, "Failed to send ring data\n");
        return -1;
    }
    fclose(datafile);
    free(data_buffer);
    return 0;
}
