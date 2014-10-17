
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "port.h"
#include "ssocket-udp.h"

int main(void)
{
	udpcli_t * cli;
	int fd;
	unsigned char buf[2048];
	cli=new_udp_client();

	fd=udp_cli_get_socket_fd(cli);

	printf("My socket fd is :%d \n",fd);

	udp_client_send(cli, "127.0.0.1", 4000, "hello udp ayoub",20);

	udp_cli_recv(cli, buf,2048);

	printf(" Message received is : %s \n",buf);
	return 0;
}