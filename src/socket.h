//
// Created by lucas on 24/03/17.
//

#ifndef GENE_ANALYSIS_SOCKET_H
#define GENE_ANALYSIS_SOCKET_H

#include <sys/types.h>
typedef struct {
    int _fd;
} socket_t;

int socket_create(socket_t* s);
int socket_destroy(socket_t* s);

int socket_bind_and_listen(socket_t *s, const unsigned int port);
int socket_accept(socket_t* s, socket_t* client_socket);

int socket_connect(socket_t *s, const char *host, const unsigned int port);
int socket_shutdown(socket_t* s, const int mode);

ssize_t socket_send(socket_t* s, const char* msg, size_t len);
ssize_t socket_receive(socket_t *s, char *buf, size_t len);

#endif //GENE_ANALYSIS_SOCKET_H
