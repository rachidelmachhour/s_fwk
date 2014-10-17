

#include "ssocket-udp.h"


udpcli_t * new_udp_client()
{
	udpcli_t * cli;
	ssocket_t * socket_s;
	cli= (udpcli_t *)malloc(sizeof(udpcli_t));
	socket_s=ssocket_init();
	s_socket_create(socket_s,AF_INET, SOCK_DGRAM, 0);

	if(socket_s->socket==-1)
		return -1;

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
	ssocket_t *socket_cli;
	ssocket_t *socket_s;
	socket_s=cli->socket_d;
    socket_cli=ssocket_init();
    int recvlen;
    char message[2048];

	recvlen = s_socket_recvfrom(socket_s, message, len, 0, socket_cli);
	if (recvlen > 0) {
		message[recvlen] = 0;
		strcpy(buf,message);
	}
	s_socket_close(socket_cli);
}

int udp_client_send(udpcli_t *cli, char *ip, int port, unsigned char * buf,int len)
{
	ssocket_t *socket_cli;
	ssocket_t *socket_s;
	socket_s=cli->socket_d;
    socket_cli=ssocket_init();

	s_socket_addr_info(inet_addr(ip),port,socket_cli);
	if (s_socket_sendto(socket_s, buf, len, 0, socket_cli)==-1)
			perror("sendto");
	s_socket_close(socket_cli);
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
		perror("cannot create socket\n");
		return 0;
	}
	s_socket_set_sock_opt(socket_s, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on));
	s_socket_addr_info(INADDR_ANY,port,socket_s);
	if (s_socket_bind(socket_s) < 0)
	{
		perror("bind failed");
		return 0;
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
	ssocket_t *socket_srv;
	ssocket_t *socket_s;
	int recvlen;
	char message[2048];
	socket_s=srv->socket_d;
    socket_srv=ssocket_init();

	recvlen = s_socket_recvfrom(socket_s, message, len, 0, socket_srv);
	if (recvlen > 0) {
		message[recvlen] = 0;
		cli_info->ip=inet_ntoa(socket_srv->address.sin_addr);
		cli_info->port=ntohs(socket_srv->address.sin_port);
		strcpy(buf,message);
	}
	s_socket_close(socket_srv);
}

int udp_server_responseto(udpsrv_t *srv,unsigned char *buf,int len, struct udpcli_info *cli_info)
{
	ssocket_t *socket_srv;
	ssocket_t *socket_s;
    socket_srv=ssocket_init();
    socket_s=srv->socket_d;
	s_socket_addr_info(inet_addr(cli_info->ip),cli_info->port,socket_srv);
	if (s_socket_sendto(socket_s, buf, len, 0, socket_srv)==-1)
			perror("sendto");
	s_socket_close(socket_srv);
}

