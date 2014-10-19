/**
 * \file s_socket.c
 * \brief Definition of the socket functions
 * \author Ayoub AOUNE ( SpartSystems )
 * \version 0.1
 * \date 29 septembre 2014
 *
 * This is file is used to code the functions needed to use sockets.
 *
 */

#include "s_socket.h"

#define BUFSIZE 2048

info_t * info_init()
{
	info_t * info_s;
	info_s= (info_t *)malloc(sizeof(info_t));
	info_s->address_len=sizeof(info_s->address);
	return info_s;
}

ssocket_t * ssocket_init()
{
	ssocket_t *socket_s;
	socket_s= (ssocket_t *)malloc(sizeof(ssocket_t));
	socket_s->address_len=sizeof(socket_s->address);
	socket_s->nb_error_sent=0;
	socket_s->nb_byte_sent=0;
	socket_s->nb_buf_sent=0;
	socket_s->nb_error_recv=0;
	socket_s->nb_byte_recv=0;
	socket_s->nb_buf_recv=0;
	return socket_s;
}

int s_socket_create(ssocket_t *socket_s,int domain, int type, int protocol)
{
	int sock;
#ifdef _WIN32
	WSADATA WSAData;
	WSAStartup(MAKEWORD(2,0), &WSAData);
#endif
	if ((sock=socket(domain, type, protocol))==-1)
		return -1;
	socket_s->socket=sock;
	return 0;
}

int s_socket_addr_info(ssocket_t *socket_s, unsigned long ip, int port)
{
	memset(&socket_s->address, 0, socket_s->address_len);

	socket_s->address.sin_family = AF_INET;

	if(ip=="INADDR_ANY"){
	socket_s->address.sin_addr.s_addr = htonl(ip);
	}
	else
	socket_s->address.sin_addr.s_addr = ip;

	socket_s->address.sin_port = htons(port);

	return 0;
}

int s_socket_bind(ssocket_t *socket_s)
{
	if (bind(socket_s->socket, (struct sockaddr *)&socket_s->address, socket_s->address_len) < 0) 
		return -1;

	return 0;
}

int s_socket_get_peer_name(ssocket_t *socket_s)
{
	int ret;
	if (ret=getpeername(socket_s->socket, (struct sockaddr *)&socket_s->address, (int *)&socket_s->address_len) < 0) 
		return -1;

	return ret;
}

int s_socket_get_sock_name(ssocket_t *socket_s)
{
	int ret;
	if (ret=getsockname(socket_s->socket, (struct sockaddr *)&socket_s->address, (int *)&socket_s->address_len) < 0) 
		return -1;

	return ret;
}

int s_socket_connect(ssocket_t *socket_s)
{
	if (connect(socket_s->socket, (const struct sockaddr *)&socket_s->address, socket_s->address_len) < 0)
		return -1;

	return 0;
}

size_t s_socket_send(ssocket_t *socket_s, const void *message, size_t length, int flags)
{
	size_t len;
	len = send(socket_s->socket, message, length, flags);
	if(len==-1)
	{
		perror("send error");
		socket_s->nb_error_sent++;
		return -1;
	}
	socket_s->nb_buf_sent++;
	socket_s->nb_byte_sent+=len;
	return len;
}

size_t s_socket_sendto(ssocket_t *socket_s, const void *message, size_t length, int flags,info_t * info_s)
{
	size_t len;
	len = sendto(socket_s->socket,message, length, flags,&info_s->address, info_s->address_len);
	if(len==-1)
	{
		perror("sendto error");
		socket_s->nb_error_sent++;
		return -1;
	}
	socket_s->nb_buf_sent++;
	socket_s->nb_byte_sent+=len;
	return len;
}

size_t s_socket_send_msg(ssocket_t *socket_s, const struct msghdr *msg, int flags)
{
#ifdef __linux__ 
	size_t len;
	len=sendmsg(socket_s->socket, msg, flags);
	if(len==-1)
	{
		perror("send_msg error");
		socket_s->nb_error_sent++;
		return -1;
	}
	socket_s->nb_buf_sent++;
	socket_s->nb_byte_sent+=len;
	return len;
#endif
	int i, msg_len = 0;
	ssize_t ret;
	char *buf;

	if (msg->msg_iovlen == 1) {
		return sendto(socket_s->socket, msg->msg_iov[0].iov_base, msg->msg_iov[0].iov_len,
						flags, msg->msg_name, msg->msg_namelen);
	}

	for (i=0; i < (int)msg->msg_iovlen; i++)
		msg_len += msg->msg_iov[i].iov_len;

	buf = malloc(msg_len);
	if (!buf)
		return -1;

	msg_len = 0;
	for (i=0; i < (int)msg->msg_iovlen; i++)
	{
		memcpy(buf+msg_len, msg->msg_iov[i].iov_base, msg->msg_iov[i].iov_len);
		msg_len += msg->msg_iov[i].iov_len;
	}

	ret = sendto(socket_s->socket, buf, msg_len, flags, msg->msg_name, msg->msg_namelen);
	if(ret==-1)
	{
		perror("send_msg error");
		socket_s->nb_error_sent++;
		return -1;
	}
	socket_s->nb_buf_sent++;
	socket_s->nb_byte_sent+=ret;
	free(buf);

	return ret;
}

size_t s_socket_recv(ssocket_t *socket_s, void *buffer, size_t length, int flags)
{
	size_t len;
	len = recv(socket_s->socket,buffer, length, flags);
	if(len==-1)
	{
		perror("recv error");
		socket_s->nb_error_recv++;
		return -1;
	}
	socket_s->nb_buf_recv++;
	socket_s->nb_byte_recv+=len;
	return len;
}

size_t s_socket_recvfrom(ssocket_t *socket_s, void *buffer, size_t length,int flags,info_t * info_s)
{
	size_t len;
	len = recvfrom(socket_s->socket,buffer, length,flags, &info_s->address, &info_s->address_len);
	if(len==-1)
	{
		perror("recvfrom error");
		socket_s->nb_error_recv++;
		return -1;
	}
	socket_s->nb_buf_recv++;
	socket_s->nb_byte_recv+=len;
	return len;
}

size_t s_socket_recv_msg(ssocket_t *socket_s, struct msghdr *msg, int flags)
{
#ifdef __linux__ 
	size_t len;
	len=recvmsg(socket_s->socket, msg, flags);
	if(len==-1)
	{
		perror("recvmsg error");
		socket_s->nb_error_recv++;
		return -1;
	}
	socket_s->nb_buf_recv++;
	socket_s->nb_byte_recv+=len;
	return len;
#endif
	int i, msg_len = 0;
	ssize_t ret;

	if (msg->msg_iovlen == 1) {
		return recvfrom(socket_s->socket, msg->msg_iov[0].iov_base, msg->msg_iov[0].iov_len,
						flags, msg->msg_name, msg->msg_namelen);
	}

	msg_len = 0;

	for (i=0; i < (int)msg->msg_iovlen; i++)
	{
		ret=recvfrom(socket_s->socket, msg->msg_iov[i].iov_base, msg_len, flags, msg->msg_name, msg->msg_namelen);
		if(ret==-1)
		{
			perror("recvmsg error");
			socket_s->nb_error_recv++;
			return -1;
		}
		socket_s->nb_buf_recv++;
		socket_s->nb_byte_recv+=ret;
		msg_len += msg->msg_iov[i].iov_len;
	}

	return ret;
}

int s_socket_listen(ssocket_t *socket_s, int backlog)
{
	if (listen(socket_s->socket, backlog) < 0) 
		return -1;

	return 0;
}

int s_socket_accept(ssocket_t *socket_s, info_t * info_s)
{
	int ret;
	if ((ret=accept(socket_s->socket, (struct sockaddr *)&info_s->address, (int *)&info_s->address_len)) < 0) 
		return -1;

	return ret;
}

int s_socket_close(ssocket_t *socket_s)
{
#ifdef _WIN32
	closesocket(socket_s->socket);
    WSACleanup();
#elif __linux__ 
    close(socket_s->socket);
#endif
    return 0;
}

int s_socket_shutdown(ssocket_t *socket_s, int how)
{
	if (shutdown(socket_s->socket,how) < 0) 
		return -1;

	return 0;
}

int s_socket_set_sock_opt(ssocket_t *socket_s, int level, int option_name,const void *option_value, int option_len)
{
	int ret;
	if (ret=setsockopt(socket_s->socket, level, option_name,option_value, option_len) < 0) 
		return -1;

	return ret;
}

int s_socket_get_sock_opt(ssocket_t *socket_s, int level, int option_name,void *option_value, int *option_len)
{
	int ret;
	if (ret=getsockopt(socket_s->socket, level, option_name,option_value, option_len) < 0) 
		return -1;

	return ret;
}

