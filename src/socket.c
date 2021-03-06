//
// Created by lucas on 24/03/17.
//
#include <sys/socket.h>
#include <netdb.h>

#include <arpa/inet.h>
#include "socket.h"
#include <string.h>
#include <unistd.h>



int socket_create(socket_t* s) {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0)
        return 1;
    s->_fd = fd;
    return 0;
}

int socket_destroy(socket_t* s) {
    close(s->_fd);
    return 0;
}

int socket_bind_and_listen(socket_t *s, const unsigned int port) {
    struct sockaddr_in srv;
    memset(&srv, 0, sizeof(srv));
    srv.sin_family = AF_INET;
    srv.sin_addr.s_addr = htonl(INADDR_ANY);
    srv.sin_port = htons((uint16_t) port);

    if (bind(s->_fd, (const struct sockaddr *) &srv, sizeof(srv)) < 0) {
        return 1;
    }

    listen(s->_fd, 1);
    return 0;
}

int socket_accept(socket_t* s, socket_t* client_socket) {
    struct sockaddr_in client;
    socklen_t clilen = (socklen_t) sizeof(struct sockaddr_in);

    int fd = accept(s->_fd, (struct sockaddr*) &client, &clilen);
    if (fd < 0) {
        return 1;
    }
    client_socket->_fd = fd;
    return 0;
}

int socket_connect(socket_t *s, const char *host, const unsigned int port) {
    struct sockaddr_in srv;
    srv.sin_family = AF_INET;
    srv.sin_port = htons((uint16_t)port);
    srv.sin_addr.s_addr = inet_addr(host);

    socklen_t len = (socklen_t)sizeof(struct sockaddr);
    int error = connect(s->_fd, (struct sockaddr *) &srv, len);
    if (error){
        return 1;
    }
    return 0;
}

int socket_shutdown(socket_t* s, const int mode) {
    int error = shutdown(s->_fd, mode);
    if (error) {
        return 1;
    }
    return 0;
}

ssize_t socket_send(socket_t* s, const char* msg, size_t len) {
    ssize_t total_bytes = 0;
    ssize_t sent = 1;

    while (total_bytes < len && sent) {
        sent = send(s->_fd, msg + total_bytes, len - total_bytes, MSG_NOSIGNAL);
        if (sent < 0) {
            return -1;
        }
        total_bytes += sent;
    }
    return total_bytes;
}

ssize_t socket_receive(socket_t *s, char *buf, size_t len) {
    ssize_t total_recv = 0;
    ssize_t received = 1;
    while (total_recv < len && received) {
        received = recv(s->_fd, buf + total_recv,
                        len - total_recv, MSG_NOSIGNAL);
        if (received < 0) {
            return -1;
        }
        total_recv += received;
    }
    return total_recv;
}
