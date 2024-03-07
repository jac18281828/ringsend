#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "network.h"

#define CONNECTIONS 5
#define READ_BUFFER_SZ 1024
#define SOCKET_RCVBUF_SIZE 1024 * 1024

int create_ring_socket(int port, struct ring_socket *rs) {
    if(rs == NULL) {
        fprintf(stderr, "Ring socket is NULL\n");
        return -1;
    }

    int opt = 1;

    rs->port = port;
    // Create a socket file descriptor
    if ((rs->fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        return -1;
    }

    if (setsockopt(rs->fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        return -1;
    }

    int read_buffer_size = SOCKET_RCVBUF_SIZE;
    if (setsockopt(rs->fd, SOL_SOCKET, SO_RCVBUF, &read_buffer_size, sizeof(read_buffer_size))) {
        perror("setsockopt");
        return -1;
    }

    rs->address.sin_family = AF_INET;
    rs->address.sin_addr.s_addr = INADDR_ANY;
    rs->address.sin_port = htons(port);

    // Bind the socket to the address and port
    if (bind(rs->fd, (struct sockaddr *)&rs->address, sizeof(rs->address)) < 0) {
        perror("bind failed");
        return -1;
    }

    // Listen for incoming connections
    if (listen(rs->fd, CONNECTIONS) < 0) {
        perror("listen");
        return -1;
    }

    return 0;
}

int ring_socket_send(int peer_port, const char *message, int message_len) {
    int client_fd;
    struct hostent *server;
    struct sockaddr_in address;
    int n_write;
    int total_written = 0;

    // Create a socket file descriptor
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        return -1;
    }

    server = gethostbyname("localhost"); // Assuming server on the same machine
    if (server == NULL) {
        close(client_fd);
        fprintf(stderr,"Error, no such host\n");
        return -1;
    }

    memset((char *) &address, 0, sizeof(address));
    address.sin_family = AF_INET;
    memcpy((char *)server->h_addr, (char *)&address.sin_addr.s_addr, server->h_length);
    address.sin_port = htons(peer_port);
    
    // Connect to the server
    if (connect(client_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        close(client_fd);
        perror("connection failed");
        return -1;
    }

    printf("Sending ring file to peer on port %d\n", peer_port);

    while(total_written < message_len) {
        n_write = write(client_fd, message + total_written, message_len - total_written);
        if (n_write < 0) {
            perror("write");
            return -1;
        }
        total_written += n_write;
    }

    close(client_fd);
    return total_written;
}

int ring_socket_receive(struct ring_socket *rs, char *message, int message_len) {
    int addrlen = sizeof(rs->address);
    int client_fd;
    int total_read = 0;
    int n_read;
    char buffer[READ_BUFFER_SZ];
    // Accept a connection
    if ((client_fd = accept(rs->fd, (struct sockaddr *)&rs->address, (socklen_t*)&addrlen)) < 0) {
        perror("accept");
        return -1;
    }

    // Read the data from the client
    while (total_read < message_len) {
        n_read = read(client_fd, buffer, sizeof(buffer));
        if (n_read < 0) {
            perror("read");
            return -1;
        }
        if (total_read + n_read > message_len) {
            fprintf(stderr, "Buffer overflow\n");
            return -1;
        }
        memcpy(message + total_read, buffer, n_read);
        total_read += n_read;
    }

    printf("Received ring file from peer\n");

    // Close the sockets
    close(client_fd);
    return total_read;
}

int destroy_ring_socket(struct ring_socket *rs) {
    close(rs->fd);
    return 0;
}