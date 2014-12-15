
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "s_udp.h"

#define BUFSIZE 2048

int main(int argc, char **argv)
{	
	char message[256];
    udpsrv_t * srv;
    int res=0;
    struct udpcli_info *cli_info;
	srv=new_udp_server(4520);
	
	cli_info=udpcli_info_init();

	while (1)
	{
		udp_server_recv(srv, message,256,cli_info);
		printf("the client info are : IP = %s \t Port = %d \n",cli_info->ip,cli_info->port);
		printf("%s\n", message);
		res=udp_server_responseto(srv,"well received\r\n",strlen("well received\r\n"), cli_info);
		bzero(message,256);
	}
	
}
