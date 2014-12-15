

#include "s_tcp.h"
#include "s_loop.h"

#define MAX_CLIENTS 3

int sock=0;
tcpsrv_t * srv;
char message[255];
sloop_t * sloop_d;
int clients[MAX_CLIENTS];

void myfunc(int fd, void *user_data)
{
	s_socket_set_non_block(fd);

	printf("client appellé !! \n \n");

	tcp_recv(fd,message,255,0);

	tcp_recv(fd,message,255,0);

	printf(" the message received is : %s \n",message);

    tcp_send(fd,"Tcp communication is allright \n",50,0);

    tcp_recv(fd,message,255,0);

    if(strcmp(message,"close")==0)
    	sloop_remove_fd(sloop_d,SLOOP_RD,fd);

}


void myserver(int fd, void *user_data)
{
	int i;

	printf("server appelé !! \n \n");

	sock =tcp_accept_clients(srv,clients);

	if(sock!=0)
	{
	tcp_send(sock,"Tcp communication OK \n",50,0);

    sloop_add_fd(sloop_d,SLOOP_RD,sock,myfunc,NULL);
	}
}


int main()
{
	int i;

	bzero(clients,sizeof(clients));

	sloop_d = sloop_new();

	sloop_set_timeout(sloop_d,500);

	srv=new_tcp_server(5000);

	sock=tcp_wait_for_clients(srv,MAX_CLIENTS);

    sloop_add_fd(sloop_d,SLOOP_RD,sock,myserver,NULL);

    sloop_run(sloop_d);
    
}