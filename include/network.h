#pragma once

#include <netinet/in.h>

#ifdef __cplusplus
extern "C"
{
#endif

/// a struct to hold the ring socket
struct ring_socket
{
    int fd;
    int port;
    struct sockaddr_in address;
};

/**
 * create the ring socket
 * @param port the port to bind to
 * @param ring_socket the ring socket to create
 * @return 0 on success, -1 on error
 */
int create_ring_socket(int port, struct ring_socket *rs);

/**
 * destroy the ring socket
 * @param ring_socket the ring socket to destroy
 * @return 0 on success, -1 on error
 */
int destroy_ring_socket(struct ring_socket *rs);

/**
 * send a message to the peer
 * @param peer_port the port of the peer
 * @param message the message to send
 * @param message_len the length of the message
 * @return message_len on success, -1 on error
 */
int ring_socket_send(int peer_port, const char *message, int message_len);

/**
 * receive a message from the peer
 * @param ring_socket the ring socket to receive from
 * @param message the message to receive
 * @param message_len the length of the message
 * @return message_len on success, -1 on error
 */
int ring_socket_receive(struct ring_socket *rs, char *message, int message_len);

#ifdef __cplusplus
}
#endif
