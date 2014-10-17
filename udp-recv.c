
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "port.h"
#include "ssocket-udp.h"

#define BUFSIZE 2048

int main(int argc, char **argv)
{	
	unsigned char message[2048];
    udpsrv_t * srv;
    struct udpcli_info *cli_info;
	
	srv=new_udp_server(4000);
	udp_server_recv(srv, message,sizeof(message),cli_info);

	printf("this is the message : %s \n \n", message);
	printf("the client info are : IP = %s \t Port = %d \n",cli_info->ip,cli_info->port);

	udp_server_responseto(srv,"well received",2048, cli_info);
}
