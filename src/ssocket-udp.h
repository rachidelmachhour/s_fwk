
#include "ssocket.h"


#ifndef SSOCKET_UDP_H
#define SSOCKET_UDP_H

struct udpcli_info
{
	char *ip;
	int port;
};

struct udp_client
{
    ssocket_t *socket_d;
};

struct udp_server
{
    ssocket_t *socket_d;
};

typedef struct udp_client udpcli_t;

typedef struct udp_server udpsrv_t;


//client

udpcli_t * new_udp_client();

ssocket_t * udp_cli_get_socket_ptr(udpcli_t *cli);

int udp_cli_get_socket_fd(udpcli_t *cli);

int udp_cli_recv(udpcli_t *cli, unsigned char *buf,int len);

int udp_client_send(udpcli_t *cli, char *ip, int port, unsigned char * buf,int len);


//Server

udpsrv_t * new_udp_server(int port);

ssocket_t * udp_srv_get_socket_ptr(udpsrv_t *srv);

int udp_srv_get_socket_fd(udpsrv_t *srv);

int udp_server_recv(udpsrv_t *srv, unsigned char *buf,int len, struct udpcli_info *cli_info);

int udp_server_responseto(udpsrv_t *srv,unsigned char *buf,int len, struct udpcli_info *cli_info);


#endif
