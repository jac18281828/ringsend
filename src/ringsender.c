#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#include "ringsender.h"
#include "network.h"

static void *ring_sender(void *args);

int create_ring_sender(int port, int peer_port, int data_buffer_size, struct ring_sender *sender) {
    if (sender == NULL) {
        fprintf(stderr, "Sender is NULL\n");
        return -1;
    }

    sender->port = port;
    sender->peer_port = peer_port;
    sender->data_buffer_size = data_buffer_size;
    sender->data_buffer = malloc(data_buffer_size);
    if(sender->data_buffer == NULL) {
        free(sender->data_buffer);
        fprintf(stderr, "Failed to allocate data buffer\n");
        return -1;
    }
    if(create_ring_socket(port, &sender->ring_socket) != 0) {
        free(sender->data_buffer);
        fprintf(stderr, "Failed to create ring socket\n");
        return -1;
    }
    if(pthread_create(&sender->thread, NULL, ring_sender, sender) != 0) {
        free(sender->data_buffer);        
        destroy_ring_socket(&sender->ring_socket);
        fprintf(stderr, "Failed to create ring thread\n");
        return -1;
    }
    return 0;

}

int cancel_ring_sender(struct ring_sender *sender) {
    if (sender == NULL) {
        fprintf(stderr, "Sender is NULL\n");
        return -1;
    }
    pthread_cancel(sender->thread);
    return 0;
}

int destroy_ring_sender(struct ring_sender *sender) {
    if (sender == NULL) {
        fprintf(stderr, "Sender is NULL\n");
        return -1;
    }
    pthread_join(sender->thread, NULL);
    destroy_ring_socket(&sender->ring_socket);
    free(sender->data_buffer);
    return 0;
}

static void *ring_sender(void *args) {
    struct ring_sender *sender = (struct ring_sender *) args;

    for(;;) {
        int bytes_read = ring_socket_receive(&sender->ring_socket, sender->data_buffer, sender->data_buffer_size);
        if(bytes_read < sender->data_buffer_size) {
            fprintf(stderr, "Failed to receive ring\n");
            break;
        }
        sleep(1);
        int bytes_sent = ring_socket_send(sender->peer_port, sender->data_buffer, sender->data_buffer_size);
        if(bytes_sent < sender->data_buffer_size) {
            fprintf(stderr, "Failed to send data\n");
            break;
        }
    }
    
    return NULL;
}
