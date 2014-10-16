/**
 * \file ssocket.h
 * \brief Header for the s_socket functions
 * \author Ayoub AOUNE ( SpartSystems )
 * \version 0.1
 * \date 15 octobre 2014
 *
 * This is a header file for the s_socket functions, it contains all the prototypes of functions needed to develop an application using this socket in both Linux and Windows.
 *
 */


#ifdef __linux__ 
#include <unistd.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#elif _WIN32
#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#endif 


#ifndef SSOCKET_H
#define SSOCKET_H

#ifdef _WIN32 
struct iovec {                
              void  *iov_base;             
              size_t iov_len;            
};

struct msghdr {
              void         *msg_name;    
              int     msg_namelen;    
              struct iovec *msg_iov;       
              size_t        msg_iovlen;    
              void         *msg_control;   
              int     msg_controllen; 
              int           msg_flags; 
};
#endif 

/**
 * \fn int s_socket_create(int domain, int type, int protocol);
 * \brief function used to create a socket.
 *
 * \param domain : selects the protocol family which will be used for communication
 * \param type : specifies the communication semantics.
 * \param protocol : specifies a particular protocol to be used with the socket.
 * \return On success, a file descriptor. On error, -1 is returned.
 */
int s_socket_create(int domain, int type, int protocol);


/**
 * \fn int s_socket_addr_info(unsigned long ip, int port, void *sockaddr, int socklen);
 * \brief function used to set the address informations that will be used by the socket.
 *
 * \param ip : the IP
 * \param port : the port.
 * \param sockaddr : used by the socket when an application want to communicate following the internet style.
 * \param socklen : the length of the sockaddr.
 * \return 0 on success or -1 when error occurs.
 */
int s_socket_addr_info(unsigned long ip, int port, void *sockaddr, int socklen);

/**
 * \fn int s_socket_bind(int socket, const struct sockaddr *address,int address_len);
 * \brief function used to bind a name to a socket.
 *
 * \param socket : the file descriptor of the socket.
 * \param address : used by the socket when an application want to communicate following the internet style.
 * \param address_len : the length of the sockaddr.
 * \return 0 on success or -1 when error occurs.
 */
int s_socket_bind(int socket, const struct sockaddr *address,int address_len);

/**
 * \fn int s_socket_get_peer_name(int socket, const struct sockaddr *address,int address_len);
 * \brief function used to get the name of the connected peer socket .
 *
 * \param socket : the file descriptor of the socket.
 * \param address : used by the socket when an application want to communicate following the internet style.
 * \param address_len : the length of the sockaddr.
 * \return 0 on success or -1 when error occurs.
 */
int s_socket_get_peer_name(int socket, struct sockaddr *address,int *address_len);

/**
 * \fn int s_socket_get_sock_name(int socket, const struct sockaddr *address,int address_len);
 * \brief function used to get the socket name.
 *
 * \param socket : the file descriptor of the socket.
 * \param address : used by the socket when an application want to communicate following the internet style.
 * \param address_len : the length of the sockaddr.
 * \return 0 on success or -1 when error occurs.
 */
int s_socket_get_sock_name(int socket, struct sockaddr *address,int *address_len);

/**
 * \fn int s_socket_connect(int socket, const struct sockaddr *address,int address_len);
 * \brief function used to initiate a connection on a socket .
 *
 * \param socket : the file descriptor of the socket.
 * \param address : used by the socket when an application want to communicate following the internet style.
 * \param address_len : the length of the sockaddr.
 * \return 0 on success or -1 when error occurs.
 */
int s_socket_connect(int socket, const struct sockaddr *address,int address_len);

/**
 * \fn size_t s_socket_send(int socket, const void *message, size_t length, int flags);
 * \brief function used to send a message on a socket.
 *
 * \param socket : the file descriptor of the socket.
 * \param message : this is the message that will be sent.
 * \param length : the length of the message.
 * \param flags : the flags of the function send.
 * \return On success, these calls return the number of characters sent. On error, -1 is returned, and errno is set appropriately. 
 */
size_t s_socket_send(int socket, const void *message, size_t length, int flags);

/**
 * \fn size_t s_socket_sendto(int socket, const void *message, size_t length, int flags,const struct sockaddr *dest_addr, int dest_len);
 * \brief function used to send a message on a socket.
 *
 * \param socket : the file descriptor of the socket.
 * \param message : this is the message that will be sent.
 * \param length : the length of the message.
 * \param flags : the flags of the function send.
 * \param dest_addr : used by the socket when an application want to communicate following the internet style.
 * \param dest_len : the length of the sockaddr.
 * \return On success, these calls return the number of characters sent. On error, -1 is returned, and errno is set appropriately. 
 */
size_t s_socket_sendto(int socket, const void *message, size_t length, int flags,const struct sockaddr *dest_addr, int dest_len);

/**
 * \fn size_t s_socket_send_msg(int socket, const struct msghdr *msg, int flags);
 * \brief function used to send a message on a socket.
 *
 * \param socket : the file descriptor of the socket.
 * \param msg : this is the message that will be sent following the structure "msghdr".
 * \param flags : the flags of the function send.
 * \return On success, these calls return the number of characters sent. On error, -1 is returned, and errno is set appropriately. 
 */
size_t s_socket_send_msg(int socket, const struct msghdr *msg, int flags);

/**
 * \fn size_t s_socket_recv(int socket, void *buffer, size_t length, int flags);
 * \brief function used to receive a message from a socket.
 *
 * \param socket : the file descriptor of the socket.
 * \param buffer : this is the buffer where the message will be saved.
 * \param length : the length of the buffer.
 * \param flags : the flags of the function recv.
 * \return On success, these calls return the number of characters received. On error, -1 is returned, and errno is set appropriately. 
 */
size_t s_socket_recv(int socket, void *buffer, size_t length, int flags);

/**
 * \fn size_t s_socket_recvfrom(int socket, void *buffer, size_t length,int flags, struct sockaddr *address, int *address_len);
 * \brief function used to receive a message from a socket.
 *
 * \param socket : the file descriptor of the socket.
 * \param buffer : this is the buffer where the message will be saved.
 * \param length : the length of the buffer.
 * \param flags : the flags of the function recv.
 * \param dest_addr : used by the socket when an application want to communicate following the internet style.
 * \param dest_len : the length of the sockaddr.
 * \return On success, these calls return the number of characters received. On error, -1 is returned, and errno is set appropriately. 
 */
size_t s_socket_recvfrom(int socket, void *buffer, size_t length,int flags, struct sockaddr *address, int *address_len);

/**
 * \fn size_t s_socket_recv_msg(int socket, struct msghdr *msg, int flags);
 * \brief function used to receive a message on a socket.
 *
 * \param socket : the file descriptor of the socket.
 * \param msg : this is the message that will be received following the structure "msghdr".
 * \param flags : the flags of the function recv.
 * \return On success, these calls return the number of characters sent. On error, -1 is returned, and errno is set appropriately. 
 */
size_t s_socket_recv_msg(int socket, struct msghdr *msg, int flags);

/**
 * \fn int s_socket_listen(int socket, int backlog);
 * \brief function used to listen for connections on a socket.
 *
 * \param socket : the file descriptor of the socket.
 * \param backlog : defines the maximum length of the  queue  of pending  connections.
 * \return 0 on success or -1 when error occurs.
 */
int s_socket_listen(int socket, int backlog);

/**
 * \fn int s_socket_accept(int socket, struct sockaddr *address,int *address_len);
 * \brief function used to accept a connection on a socket.
 *
 * \param socket : the file descriptor of the socket.
 * \param address : used by the socket when an application want to communicate following the internet style.
 * \param address_len : the length of the sockaddr.
 * \return Returns the file descriptor of the socket which is accepted.
 */
int s_socket_accept(int socket, struct sockaddr *address,int *address_len);

/**
 * \fn int s_socket_close(int socket);
 * \brief function used to close a file descriptor.
 *
 * \param socket : the file descriptor of the socket.
 * \return 0 on success or -1 when error occurs.
 */
int s_socket_close(int socket);

/**
 * \fn int s_socket_shutdown(int socket, int how);
 * \brief function used to shut down part of a full-duplex connection.
 *
 * \param socket : the file descriptor of the socket.
 * \param how : how to shutdown this connection.
 * \return 0 on success or -1 when error occurs.
 */
int s_socket_shutdown(int socket, int how);

/**
 * \fn int s_socket_get_sock_opt(int socket, int level, int option_name,void *option_value, int *option_len);
 * \brief function used to get options on sockets.
 *
 * \param socket : the file descriptor of the socket.
 * \param level : this is the level where the option exists.
 * \param option_name : the option name.
 * \param option_value : the value of the option.
 * \param option_len : the length of the option choosed.
 * \return 0 on success or -1 when error occurs. 
 */
int s_socket_get_sock_opt(int socket, int level, int option_name,void *option_value, int *option_len);

/**
 * \fn int s_socket_set_sock_opt(int socket, int level, int option_name,const void *option_value, int *option_len);
 * \brief function used to set options on sockets.
 *
 * \param socket : the file descriptor of the socket.
 * \param level : this is the level where the option exists.
 * \param option_name : the option name.
 * \param option_value : the value of the option.
 * \param option_len : the length of the option choosed.
 * \return 0 on success or -1 when error occurs.
 */
int s_socket_set_sock_opt(int socket, int level, int option_name,const void *option_value, int option_len);

#endif

 