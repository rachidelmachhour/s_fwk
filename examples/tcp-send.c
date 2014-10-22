

#include "s_tcp.h"
#include <stdio.h>

int main()
{
	int socket;
	tcpcli_t * cli;
	char message1[255],message2[255];

	cli=new_tcp_client();

	socket=tcp_cli_connect(cli,"127.0.0.1",5000);

	if(socket == -1)
	{
		printf("error \n");
		return -1;
	}

	tcp_recv(socket,message1,255,0);

	printf(" the acknowledgment received is : %s \n",message1);

	tcp_send(socket,"Hello tcp Ayoub",50,0);

	tcp_recv(socket,message2,255,0);

	printf(" the message received is : %s \n",message2);

	tcp_send(socket,"close",50,0);

}