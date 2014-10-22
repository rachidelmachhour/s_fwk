

#include "s_tcp.h"


tcpcli_t * new_tcp_client()
{
	tcpcli_t * cli = malloc(sizeof(tcpcli_t));;
	ssocket_t * socket_s;

	socket_s=ssocket_init();
	if ((s_socket_create(socket_s, AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("cannot create socket\n");
		return NULL;
	}
	if(socket_s->socket==-1)
		return NULL;

	cli->socket_d=socket_s;
	return cli;
}

ssocket_t * tcp_cli_get_socket_ptr(tcpcli_t *cli)
{
	return cli->socket_d;
}

int tcp_cli_get_socket_fd(tcpcli_t *cli)
{
	ssocket_t * socket_s=(ssocket_t *)cli->socket_d;
	return socket_s->socket;
}

int tcp_cli_connect(tcpcli_t *cli, char *ip, int port)
{
	int res=0;
	ssocket_t * socket_s=(ssocket_t *)cli->socket_d;
	s_socket_addr_info(socket_s,inet_addr(ip),port);
	res=s_socket_connect(socket_s);
	if(res < 0)
	{
		s_socket_close(socket_s);
		return -1;
	}
	return socket_s->socket;
}


tcpsrv_t * new_tcp_server(int port)
{
	tcpsrv_t * srv;
	ssocket_t * socket_s;
	int on=1;
	srv= (tcpsrv_t *)malloc(sizeof(tcpsrv_t));
	socket_s=ssocket_init();
	if ((s_socket_create(socket_s, AF_INET, SOCK_STREAM, 0)) < 0)
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
	srv->max_client=0;
	return srv;
}

ssocket_t * tcp_srv_get_socket_ptr(tcpsrv_t *srv)
{
	return srv->socket_d;
}

int tcp_srv_get_socket_fd(tcpsrv_t *srv)
{
	ssocket_t * socket_s=(ssocket_t *)srv->socket_d;
	return socket_s->socket;
}


int tcp_wait_for_clients(tcpsrv_t *srv, int max_client)
{
	int fd_top;
	ssocket_t * socket_s=(ssocket_t *)srv->socket_d;

	srv->max_client=max_client;
	
	if (s_socket_listen(socket_s, max_client) == -1) {
      	perror("listen");
   	}

   	fd_top = socket_s->socket;
   	return fd_top;
}

int tcp_accept_clients(tcpsrv_t *srv, int *clients)
{
	int new_fd,i;
	ssocket_t * socket_s=(ssocket_t *)srv->socket_d;
	info_t * info_s;

	info_s = info_init();

	if ((new_fd = s_socket_accept(socket_s,info_s)) == -1) 
	{
        perror("accept");
    }
    for(i=0;i<srv->max_client;i++) 
    {
        if(clients[i]==0)
        {
    	    clients[i] = new_fd ;
            return new_fd;
        }
    }
    return 0;
}



size_t tcp_send(int socket,unsigned char * buf,int len,int flag)
{
	return send(socket, buf, len, flag);
}

size_t tcp_recv(int socket, unsigned char *buf,int len,int flag)
{
	return recv(socket, buf, len, flag);
}



