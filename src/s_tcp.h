
#include "s_socket.h"
#include <stdio.h>
#include <stdlib.h>

#ifndef S_TCP_H
#define S_TCP_H

struct tcp_client
{
    ssocket_t *socket_d;
};


struct tcp_server
{
    ssocket_t *socket_d;
    int *clients;
    int max_client;
};

typedef struct tcp_client tcpcli_t;

typedef struct tcp_server tcpsrv_t;

// client

tcpcli_t * new_tcp_client();

ssocket_t * tcp_cli_get_socket_ptr(tcpcli_t *cli);

int tcp_cli_get_socket_fd(tcpcli_t *cli);

int tcp_cli_connect(tcpcli_t *cli, char *ip, int port);



// server

tcpsrv_t * new_tcp_server(int port);

ssocket_t * tcp_srv_get_socket_ptr(tcpsrv_t *srv);

int tcp_srv_get_socket_fd(tcpsrv_t *srv);

int tcp_wait_for_clients(tcpsrv_t *srv, int max_client);

int tcp_accept_clients(tcpsrv_t *srv,int *clients);
// Can be used by both client and server.

size_t tcp_send(int socket,unsigned char * buf,int len,int flag);

size_t tcp_recv(int socket, unsigned char *buf,int len,int flag);



#endif 
