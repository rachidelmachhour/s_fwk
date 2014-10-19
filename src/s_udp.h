
#include "s_socket.h"

/**
 * \file s_udp.h
 * \brief Header for the udp socket functions
 * \author Ayoub AOUNE ( SpartSystems )
 * \version 0.1
 * \date 15 octobre 2014
 *
 * This is a header file for the udp socket functions, it contains all the prototypes of functions needed to develop an application using this udp socket in both Linux and Windows and Android.
 *
 */

#ifndef S_UDP_H
#define S_UDP_H

/**
 * \struct udpcli_info
 * \brief define the info of the udp client.
 * 
 * - ip : the IP of the client.
 * - port : the port of the client. 
 *
 */
struct udpcli_info
{
	char *ip;
	int port;
};

/**
 * \struct udp_client
 * \brief define the udp client.
 * 
 * - socket_d : the socket informations of the client. 
 *
 */
struct udp_client
{
    ssocket_t *socket_d;
};

/**
 * \struct udp_server
 * \brief define the udp server.
 * 
 * - socket_d : the socket informations of the server. 
 *
 */
struct udp_server
{
    ssocket_t *socket_d;
};

typedef struct udp_client udpcli_t;

typedef struct udp_server udpsrv_t;

/**
 * \fn struct udpcli_info * udpcli_info_init();
 * \brief function used to initilize the udp client info.
 *
 * \param NONE
 * \return the new udpcli_info initilized.
 */
struct udpcli_info * udpcli_info_init();

//client

/**
 * \fn udpcli_t * new_udp_client();
 * \brief function used to initilize the udp client.
 *
 * \param NONE
 * \return the new udp client initilized.
 */
udpcli_t * new_udp_client();

/**
 * \fn ssocket_t * udp_cli_get_socket_ptr(udpcli_t *cli);
 * \brief function used to get the socket pointer of the client.
 *
 * \param cli : the client.
 * \return the socket pointer of the client.
 */
ssocket_t * udp_cli_get_socket_ptr(udpcli_t *cli);

/**
 * \fn int udp_cli_get_socket_fd(udpcli_t *cli);
 * \brief function used to get the socket file descriptor of the client.
 *
 * \param cli : the client.
 * \return the file descriptor of the udp client.
 */
int udp_cli_get_socket_fd(udpcli_t *cli);

/**
 * \fn int udp_cli_recv(udpcli_t *cli, unsigned char *buf,int len);
 * \brief function used by the client to receive the response message.
 *
 * \param cli : the client.
 * \param buf : the message.
 * \param len : the length of the message.
 * \return 0 if success ,else -1.
 */
int udp_cli_recv(udpcli_t *cli, unsigned char *buf,int len);

/**
 * \fn int udp_client_send(udpcli_t *cli, char *ip, int port, unsigned char * buf,int len);
 * \brief function used by the client to send message.
 *
 * \param cli : the client.
 * \param ip : the ip of the server.
 * \param port : the port of the server.
 * \param buf : the message.
 * \param len : the length of the message.
 * \return 0 if success ,else -1.
 */
int udp_client_send(udpcli_t *cli, char *ip, int port, unsigned char * buf,int len);


//Server

/**
 * \fn udpcli_t * new_udp_server();
 * \brief function used to initilize the udp server.
 *
 * \param port : the Port of reception.
 * \return the new udp server initilized.
 */
udpsrv_t * new_udp_server(int port);

/**
 * \fn ssocket_t * udp_srv_get_socket_ptr(udpcli_t *cli);
 * \brief function used to get the socket pointer of the server.
 *
 * \param srv : the server.
 * \return the socket pointer of the server.
 */
ssocket_t * udp_srv_get_socket_ptr(udpsrv_t *srv);

/**
 * \fn int udp_srv_get_socket_fd(udpcli_t *cli);
 * \brief function used to get the socket file descriptor of the server.
 *
 * \param srv : the server.
 * \return the file descriptor of the udp server.
 */
int udp_srv_get_socket_fd(udpsrv_t *srv);

/**
 * \fn int udp_server_recv(udpsrv_t *srv, unsigned char *buf,int len, struct udpcli_info *cli_info);
 * \brief function used by the server to receive message.
 *
 * \param srv : the server.
 * \param buf : the message.
 * \param len : the length of the message.
 * \param cli_info : the client informations.
 * \return 0 if success ,else -1.
 */
int udp_server_recv(udpsrv_t *srv, unsigned char *buf,int len, struct udpcli_info *cli_info);

/**
 * \fn int udp_server_responseto(udpsrv_t *srv,unsigned char *buf,int len, struct udpcli_info *cli_info);
 * \brief function used by the server to response to client by a message.
 *
 * \param srv : the server.
 * \param buf : the message.
 * \param len : the length of the message.
 * \param cli_info : the client informations.
 * \return 0 if success ,else -1.
 */
int udp_server_responseto(udpsrv_t *srv,unsigned char *buf,int len, struct udpcli_info *cli_info);

#endif 
