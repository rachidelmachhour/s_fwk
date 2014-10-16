
#include "ssocket.h"

#define BUFSIZE 2048

ssocket_t s_socket_create(int domain, int type, int protocol)
{
	ssocket_t sock;
#ifdef _WIN32
	WSADATA WSAData;
	WSAStartup(MAKEWORD(2,0), &WSAData);
#endif
	if ((sock=socket(domain, type, protocol))==-1)
		return -1;

	return sock;
}

int s_socket_addr_info(unsigned long ip, int port, void *sockaddr, int socklen)
{
	struct sockaddr_in *sockaddrin = (struct sockaddr_in *)sockaddr;

	memset(sockaddrin, 0, socklen);
	sockaddrin->sin_family = AF_INET;
	if(ip=="INADDR_ANY"){
	sockaddrin->sin_addr.s_addr = htonl(ip);
	}
	else
	sockaddrin->sin_addr.s_addr = ip;

	sockaddrin->sin_port = htons(port);

	return 0;
}

int s_socket_bind(ssocket_t socket, const struct sockaddr *address,int address_len)
{
	if (bind(socket, (struct sockaddr *)address, address_len) < 0) 
		return -1;

	return 0;
}

int s_socket_get_peer_name(ssocket_t socket, struct sockaddr *address,int *address_len)
{
	int ret;
	if (ret=getpeername(socket,(struct sockaddr *)address,(int *)address_len) < 0) 
		return -1;

	return ret;
}

int s_socket_get_sock_name(ssocket_t socket, struct sockaddr *address,int *address_len)
{
	int ret;
	if (ret=getsockname(socket,(struct sockaddr *)address,(int *)address_len) < 0) 
		return -1;

	return ret;
}

int s_socket_connect(ssocket_t socket, const struct sockaddr *address,int address_len)
{
	if (connect(socket,(const struct sockaddr *)address,address_len) < 0)
		return -1;

	return 0;
}

size_t s_socket_send(ssocket_t socket, const void *message, size_t length, int flags)
{
	size_t len;
	len = send(socket, message, length, flags);
	return len;
}

size_t s_socket_sendto(ssocket_t socket, const void *message, size_t length, int flags,const struct sockaddr *dest_addr, int dest_len)
{
	size_t len;
	len = sendto(socket,message, length, flags,dest_addr, dest_len);
	return len;
}

size_t s_socket_send_msg(ssocket_t socket, const struct msghdr *msg, int flags)
{
#ifdef __linux__ 
	size_t len;
	len=sendmsg(socket, msg, flags);
	return len;
#endif
	int i, msg_len = 0;
	ssize_t ret;
	char *buf;

	if (msg->msg_iovlen == 1) {
		return sendto(socket, msg->msg_iov[0].iov_base, msg->msg_iov[0].iov_len,
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

	ret = sendto(socket, buf, msg_len, flags, msg->msg_name, msg->msg_namelen);

	free(buf);

	return ret;
}

size_t s_socket_recv(ssocket_t socket, void *buffer, size_t length, int flags)
{
	size_t len;
	len = recv(socket,buffer, length, flags);
	return len;
}

size_t s_socket_recvfrom(ssocket_t socket, void *buffer, size_t length,int flags, struct sockaddr *address, int *address_len)
{
	size_t len;
	len = recvfrom(socket,buffer, length,flags, address, address_len);
	return len;
}

size_t s_socket_recv_msg(ssocket_t socket, struct msghdr *msg, int flags)
{
#ifdef __linux__ 
	size_t len;
	len=recvmsg(socket, msg, flags);
	return len;
#endif
	int i, msg_len = 0;
	ssize_t ret;

	if (msg->msg_iovlen == 1) {
		return recvfrom(socket, msg->msg_iov[0].iov_base, msg->msg_iov[0].iov_len,
						flags, msg->msg_name, msg->msg_namelen);
	}

	msg_len = 0;

	for (i=0; i < (int)msg->msg_iovlen; i++)
	{
		ret=recvfrom(socket, msg->msg_iov[i].iov_base, msg_len, flags, msg->msg_name, msg->msg_namelen);
		msg_len += msg->msg_iov[i].iov_len;
	}

	return ret;
}

int s_socket_listen(ssocket_t socket, int backlog)
{
	if (listen(socket, backlog) < 0) 
		return -1;

	return 0;
}

ssocket_t s_socket_accept(ssocket_t socket, struct sockaddr *address,int *address_len)
{
	int ret;
	if ((ret=accept(socket, (struct sockaddr *)address, (int *)address_len)) < 0) 
		return -1;

	return ret;
}

int s_socket_close(ssocket_t socket)
{
#ifdef _WIN32
	closesocket(socket);
    WSACleanup();
#elif __linux__ 
    close(socket);
#endif
    return 0;
}

int s_socket_shutdown(ssocket_t socket, int how)
{
	if (shutdown(socket,how) < 0) 
		return -1;

	return 0;
}

int s_socket_set_sock_opt(ssocket_t socket, int level, int option_name,const void *option_value, int option_len)
{
	int ret;
	if (ret=setsockopt(socket, level, option_name,option_value, option_len) < 0) 
		return -1;

	return ret;
}

int s_socket_get_sock_opt(ssocket_t socket, int level, int option_name,void *option_value, int *option_len)
{
	int ret;
	if (ret=getsockopt(socket, level, option_name,option_value, option_len) < 0) 
		return -1;

	return ret;
}

