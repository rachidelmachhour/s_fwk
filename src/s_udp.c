

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
	info_t * info_s;
	ssocket_t *socket_s;
	socket_s=cli->socket_d;
    info_s=info_init();
    int recvlen;
    char message[2048];

	recvlen = s_socket_recvfrom(socket_s, message, len, 0,info_s);
	if (recvlen > 0) {
		message[recvlen] = 0;
		strcpy(buf,message);
	}
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
			perror("sendto");
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
	s_socket_addr_info(socket_s,INADDR_ANY,port);
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
	info_t * info_s;
	ssocket_t *socket_s;
	int recvlen;
	char message[2048];
	printf("0 \n");
	socket_s=srv->socket_d;
	printf("1 \n");
    info_s=info_init();
    printf("2 \n");
	recvlen = s_socket_recvfrom(socket_s, message, len, 0, info_s);
	printf("3 \n recvlen : %d \n",recvlen);
	if (recvlen > 0) {
		message[recvlen] = 0;
		cli_info->ip=inet_ntoa(info_s->address.sin_addr);
		printf("4 \n");
		cli_info->port=ntohs(info_s->address.sin_port);
		printf("5 \n");
		strcpy(buf,message);
		printf("6 \n");
	}
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
	info_s->address.sin_family=socket_srv->address.sin_family;
	info_s->address.sin_addr.s_addr=socket_srv->address.sin_addr.s_addr;
	info_s->address.sin_port=socket_srv->address.sin_port;
	if (s_socket_sendto(socket_s, buf, len, 0, info_s)==-1)
			perror("sendto");
}

