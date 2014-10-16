/**
 * \file ssocket-udp.h
 * \brief Header for the udp s_socket functions
 * \author Ayoub AOUNE ( SpartSystems )
 * \version 0.1
 * \date 17 octobre 2014
 *
 * This is a header file for the udp s_socket functions, it contains all the prototypes of functions needed to develop an application using this socket in both Linux and Windows.
 *
 */

#include "ssocket.h"


#ifndef SSOCKET_UDP_H
#define SSOCKET_UDP_H

struct ssocket_data
{
	ssocket_t socket;
	struct sockaddr *address;
	int address_len;
	int num_buf_sent;
	int nb_byte_sent;
	int nb_error;
};

typedef struct ssocket_data ssocket_d;

ssocket_d * new_udp_client();

ssocket_d * new_udp_server(int port);

int udp_client_set_port(int port);

ssocket_t udp_client_get_fd(ssocket_d * socket_s);

int udp_client_get_port(ssocket_d * socket_s);

int udp_client_send(ssocket_d *socket_s, char *ip, int port, char *message);

int udp_server_recv(ssocket_d *socket_s, char *message, ssocket_d *socket_client);

int udp_server_responseto(ssocket_d *socket_s, char *message, ssocket_d *socket_client);

int udp_server_broadcast(ssocket_d *socket_s, char *message);


#endif 
