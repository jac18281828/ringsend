#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <pthread.h>

#include "network.h"

/**
 * A struct representing a sender in the ring
 */
struct ring_sender
{
    int port;
    int peer_port;
    char *data_buffer;
    int data_buffer_size;
    pthread_t thread;
    struct ring_socket ring_socket;
};

/**
 * Create a ring sender wih the given port and peer port
 * @param port The port to bind the sender to
 * @param peer_port The port of the next sender in the ring
 * @param data_buffer_size The size of the data buffer
 * @param sender The sender to create
 * @return 0 on success, 1 on failure
 */
int create_ring_sender(int port, int peer_port, int data_buffer_size, struct ring_sender *sender);

/**
 * Cancel a ring sender
 * @param sender The sender to cancel
 * @return 0 on success, 1 on failure
 */
int cancel_ring_sender(struct ring_sender *sender);

/**
 * Destroy a ring sender
 * @param sender The sender to destroy
 * @return 0 on success, 1 on failure
 */
int destroy_ring_sender(struct ring_sender *sender);

#ifdef __cplusplus
}
#endif
