/**
 * \file s_udp.c
 * \brief Definition of the udp socket functions
 * \author Ayoub AOUNE ( SpartSystems )
 * \version 0.1
 * \date 29 septembre 2014
 *
 * This is file is used to code the functions needed to use udp socket.
 *
 */
#include <stdlib.h>
#include "s_udp.h"


struct udpcli_info * udpcli_info_init()
{
	struct udpcli_info *cli_info;
	cli_info=(struct udpcli_info *)malloc(sizeof(struct udpcli_info ));
	return cli_info;
}

udpcli_t * new_udp_client()
{
	udpcli_t * cli;
	ssocket_t * socket_s;
	cli= (udpcli_t *)malloc(sizeof(udpcli_t));
	socket_s=ssocket_init();
	s_socket_create(socket_s,AF_INET, SOCK_DGRAM, 0);

	if(socket_s->socket==-1)
		return NULL;

	cli->socket_d=socket_s;
	return cli;
}

ssocket_t * udp_cli_get_socket_ptr(udpcli_t *cli)
{
    return cli->socket_d;
}

int udp_cli_get_socket_fd(udpcli_t *cli)
{
	ssocket_t * socket_s=(ssocket_t *)cli->socket_d;
	return socket_s->socket;
}

int udp_cli_recv(udpcli_t *cli, unsigned char *buf,int len)
{
	info_t * info_s;
	ssocket_t *socket_s;
	socket_s=cli->socket_d;
	int ret;
    info_s=info_init();

	ret = s_socket_recvfrom(socket_s, buf, len, 0,info_s);

	return ret;
}

int udp_client_send(udpcli_t *cli, char *ip, int port, unsigned char * buf,int len)
{
	info_t * info_s;
	ssocket_t *socket_cli;
	ssocket_t *socket_s;
	socket_s=cli->socket_d;
    socket_cli=ssocket_init();
    info_s=info_init();

	s_socket_addr_info(socket_cli,inet_addr(ip),port);
	info_s->address.sin_family=socket_cli->address.sin_family;
	info_s->address.sin_addr.s_addr=socket_cli->address.sin_addr.s_addr;
	info_s->address.sin_port=socket_cli->address.sin_port;
	if (s_socket_sendto(socket_s, buf, len, 0, info_s)==-1)
		return -1;

	return 0;
}



udpsrv_t * new_udp_server(int port)
{
	udpsrv_t * srv;
	ssocket_t * socket_s;
	int on=1;
	srv= (udpsrv_t *)malloc(sizeof(udpsrv_t));
	socket_s=ssocket_init();
	if ((s_socket_create(socket_s, AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		return NULL;
	}
	s_socket_set_sock_opt(socket_s, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on));
	s_socket_addr_info(socket_s,INADDR_ANY,port);
	if (s_socket_bind(socket_s) < 0)
	{
		return NULL;
	}
	srv->socket_d=socket_s;
	return srv;
}

ssocket_t * udp_srv_get_socket_ptr(udpsrv_t *srv)
{
	return srv->socket_d;
}

int udp_srv_get_socket_fd(udpsrv_t *srv)
{
	ssocket_t * socket_s=(ssocket_t *)srv->socket_d;
	return socket_s->socket;
}

int udp_server_recv(udpsrv_t *srv, unsigned char *buf,int len, struct udpcli_info *cli_info)
{
	info_t * info_s;
	int ret;
	ssocket_t *socket_s;
	socket_s=srv->socket_d;
    info_s=info_init();
	ret = s_socket_recvfrom(socket_s, buf, len, 0, info_s);
	if(ret == -1)
		return -1;
	cli_info->ip = inet_ntoa(info_s->address.sin_addr);
	cli_info->port = ntohs(info_s->address.sin_port);
	return 0;
}

int udp_server_responseto(udpsrv_t *srv,unsigned char *buf,int len, struct udpcli_info *cli_info)
{
	info_t * info_s;
	ssocket_t *socket_srv;
	ssocket_t *socket_s;
    socket_srv=ssocket_init();
    info_s=info_init();
    socket_s=srv->socket_d;
	s_socket_addr_info(socket_srv,inet_addr(cli_info->ip),cli_info->port);
	info_s->address = socket_srv->address;
	info_s->address_len = socket_srv->address_len;

	return s_socket_sendto(socket_s, buf, len, 0, info_s);

}

