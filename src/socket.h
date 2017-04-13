//
// Created by lucas on 24/03/17.
//

#ifndef GENE_ANALYSIS_SOCKET_H
#define GENE_ANALYSIS_SOCKET_H

#include <sys/types.h>
typedef struct {
    int _fd;
} socket_t;

// Creates/destroys a socket
int socket_create(socket_t* s);
int socket_destroy(socket_t* s);

/*
 * Wrapper for bind() and listen(), and accept() for the server
 */
int socket_bind_and_listen(socket_t *s, const unsigned int port);
int socket_accept(socket_t* s, socket_t* client_socket);

// Wrapper for connect() for the client
int socket_connect(socket_t *s, const char *host, const unsigned int port);

// Wrapper for shutdown()
int socket_shutdown(socket_t* s, const int mode);

/*
 * Sends 'len' bytes of 'msg'through the socket. This function guarantees
 * all bytes are sent successfully. Returns -1 in case of error.
 */
ssize_t socket_send(socket_t* s, const char* msg, size_t len);

/*
 * Receives up to 'len' bytes and stores them in 'buf'. If a shutdown occurs
 * from the other side, returns the amount of bytes read before it happened.
 * Returns -1 in case of error.
 */
ssize_t socket_receive(socket_t *s, char *buf, size_t len);

#endif //GENE_ANALYSIS_SOCKET_H
