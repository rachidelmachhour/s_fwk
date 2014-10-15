
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

int s_socket_create(int domain, int type, int protocol);

int s_socket_addr_info(unsigned long ip, int port, void *sockaddr, int socklen);

int s_socket_bind(int socket, const struct sockaddr *address,int address_len);

int s_socket_get_peer_name(int socket, struct sockaddr *address,int *address_len);

int s_socket_get_sock_name(int socket, struct sockaddr *address,int *address_len);

int s_socket_connect(int socket, const struct sockaddr *address,int address_len);

size_t s_socket_send(int socket, const void *message, size_t length, int flags);

size_t s_socket_sendto(int socket, const void *message, size_t length, int flags,const struct sockaddr *dest_addr, int dest_len);

size_t s_socket_send_msg(int socket, const struct msghdr *msg, int flags);

size_t s_socket_recv(int socket, void *buffer, size_t length, int flags);

size_t s_socket_recvfrom(int socket, void *buffer, size_t length,int flags, struct sockaddr *address, int *address_len);

size_t s_socket_recv_msg(int socket, struct msghdr *msg, int flags);

int s_socket_listen(int socket, int backlog);

int s_socket_accept(int socket, struct sockaddr *address,int *address_len);

int s_socket_close(int socket);

int s_socket_shutdown(int socket, int how);

int s_socket_get_sock_opt(int socket, int level, int option_name,void *option_value, int *option_len);

int s_socket_set_sock_opt(int socket, int level, int option_name,const void *option_value, int option_len);

//Socket UDP ready to use

int s_socket_udp_send_msg(char *ip, int port,char *message);

unsigned char * s_socket_udp_recv_msg(int port, int time);

//Socket TCP ready to use

int s_socket_tcp_send_msg(char *ip, int port,char *message);

unsigned char * s_socket_tcp_recv_msg(int port, int time);

#endif 