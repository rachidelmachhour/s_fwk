
#include "ssocket.h"

#define BUFSIZE 2048

int s_socket_create(int domain, int type, int protocol)
{
	int sock;
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

int s_socket_bind(int socket, const struct sockaddr *address,int address_len)
{
	if (bind(socket, (struct sockaddr *)address, address_len) < 0) 
		return -1;

	return 0;
}

int s_socket_get_peer_name(int socket, struct sockaddr *address,int *address_len)
{
	int ret;
	if (ret=getpeername(socket,(struct sockaddr *)address,(int *)address_len) < 0) 
		return -1;

	return ret;
}

int s_socket_get_sock_name(int socket, struct sockaddr *address,int *address_len)
{
	int ret;
	if (ret=getsockname(socket,(struct sockaddr *)address,(int *)address_len) < 0) 
		return -1;

	return ret;
}

int s_socket_connect(int socket, const struct sockaddr *address,int address_len)
{
	if (connect(socket,(const struct sockaddr *)address,address_len) < 0)
		return -1;

	return 0;
}

size_t s_socket_send(int socket, const void *message, size_t length, int flags)
{
	size_t len;
	len = send(socket, message, length, flags);
	return len;
}

size_t s_socket_sendto(int socket, const void *message, size_t length, int flags,const struct sockaddr *dest_addr, int dest_len)
{
	size_t len;
	len = sendto(socket,message, length, flags,dest_addr, dest_len);
	return len;
}

size_t s_socket_send_msg(int socket, const struct msghdr *msg, int flags)
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

size_t s_socket_recv(int socket, void *buffer, size_t length, int flags)
{
	size_t len;
	len = recv(socket,buffer, length, flags);
	return len;
}

size_t s_socket_recvfrom(int socket, void *buffer, size_t length,int flags, struct sockaddr *address, int *address_len)
{
	size_t len;
	len = recvfrom(socket,buffer, length,flags, address, address_len);
	return len;
}

size_t s_socket_recv_msg(int socket, struct msghdr *msg, int flags)
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

int s_socket_listen(int socket, int backlog)
{
	if (listen(socket, backlog) < 0) 
		return -1;

	return 0;
}

int s_socket_accept(int socket, struct sockaddr *address,int *address_len)
{
	int ret;
	if ((ret=accept(socket, (struct sockaddr *)address, (int *)address_len)) < 0) 
		return -1;

	return ret;
}

int s_socket_close(int socket)
{
#ifdef _WIN32
	closesocket(socket);
    WSACleanup();
#elif __linux__ 
    close(socket);
#endif
    return 0;
}

int s_socket_shutdown(int socket, int how)
{
	if (shutdown(socket,how) < 0) 
		return -1;

	return 0;
}

int s_socket_set_sock_opt(int socket, int level, int option_name,const void *option_value, int option_len)
{
	int ret;
	if (ret=setsockopt(socket, level, option_name,option_value, option_len) < 0) 
		return -1;

	return ret;
}

int s_socket_get_sock_opt(int socket, int level, int option_name,void *option_value, int *option_len)
{
	int ret;
	if (ret=getsockopt(socket, level, option_name,option_value, option_len) < 0) 
		return -1;

	return ret;
}


int s_socket_udp_send_msg(char *ip, int port,char *message)
{
	struct sockaddr_in myaddr, remaddr;
	int slen;
	int socket;

	socket=s_socket_create(AF_INET, SOCK_DGRAM, 0);
	if(socket==-1)
		return -1;

	slen=sizeof(remaddr);

	s_socket_addr_info(INADDR_ANY,0,(struct sockaddr *)&myaddr,sizeof(myaddr));

	if (s_socket_bind(socket, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) {
		perror("bind failed");
		return 0;
	}       

	s_socket_addr_info(inet_addr(ip),port,(struct sockaddr *)&remaddr,sizeof(remaddr));

		printf("Sending packet to %s port %d\n", ip, port);
		if (s_socket_sendto(socket, message, strlen(message), 0, (struct sockaddr *)&remaddr, slen)==-1)
			perror("sendto");

	s_socket_close(socket);
}


unsigned char * s_socket_udp_recv_msg(int port, int time_sec)
{
	struct sockaddr_in myaddr;
	struct sockaddr_in remaddr;
	int addrlen = sizeof(remaddr);
	int recvlen, on = 1;					
	unsigned char buf[BUFSIZE];	
	int socket;

	if ((socket = s_socket_create(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("cannot create socket\n");
		return 0;
	}
	s_socket_set_sock_opt(socket, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on));
	s_socket_addr_info(INADDR_ANY,port,&myaddr,sizeof(myaddr));

	if (s_socket_bind(socket, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) {
		perror("bind failed");
		return 0;
	}

	if(time_sec==0)
	{
		printf("waiting on port %d\n", port);
		recvlen = s_socket_recvfrom(socket, buf, BUFSIZE, 0, (struct sockaddr *)&remaddr, &addrlen);
		printf("received %d bytes\n", recvlen);
		if (recvlen > 0) {
			buf[recvlen] = 0;
			printf("received message: \"%s\" From IP : %s \n", buf,inet_ntoa(remaddr.sin_addr)));
		}
	}
	else if(time_sec==-1)
	{
		for(;;)
		{
			printf("waiting on port %d\n", port);
			recvlen = s_socket_recvfrom(socket, buf, BUFSIZE, 0, (struct sockaddr *)&remaddr, &addrlen);
			printf("received %d bytes\n", recvlen);
			if (recvlen > 0) 
			{
				buf[recvlen] = 0;
			printf("received message: \"%s\" From IP : %s \n", buf,inet_ntoa(remaddr.sin_addr)));
			}
		}
	}
	else
	{	
	    fd_set read_fd;
	    int rc,ret;
		struct timeval timeout;
	    timeout.tv_sec = time_sec;
	    timeout.tv_usec = 0;
		FD_ZERO(&read_fd);
    	FD_SET(socket, &read_fd);
    	ret=0;
    	printf("waiting on port %d\n", port);
    	while(1)
    	{
    	rc = select(socket+1, &read_fd, NULL, NULL, &timeout);
    	if(rc<0)
    		return -1;
    	if(rc==0)
    	{
    		printf("timeout finished !! \n");
			s_socket_close(socket);
			return buf;
    	}
    	else
    	{
    		recvlen = s_socket_recvfrom(socket, buf+ret, BUFSIZE, 0, (struct sockaddr *)&remaddr, &addrlen);
			printf("received %d bytes\n", recvlen);
			buf[ret+recvlen] = ';';
			ret+=recvlen+1;
    	}
    	}
    	if (recvlen > 0) 
			{
				buf[ret] = 0;
			printf("received message: \"%s\" From IP : %s \n", buf,inet_ntoa(remaddr.sin_addr)));
			}
	}
	s_socket_close(socket);
	return buf;
}

int s_socket_tcp_send_msg(char *ip, int port,char *message)
{
	int socket, rc, length = sizeof(int);
    struct sockaddr_in serveraddr;
    char buffer[255];
    char server[255];
    char temp;
    int totalcnt = 0;
    struct hostent *hostp;

    socket = s_socket_create(AF_INET, SOCK_STREAM, 0);
    s_socket_addr_info(inet_addr(ip),port,&serveraddr,sizeof(struct sockaddr_in));
    hostp = gethostbyname(ip);
    if(hostp == (struct hostent *)NULL)
    {
    printf("HOST NOT FOUND --> ");
    s_socket_close(socket);
    return -1;
    }
    if((rc = s_socket_connect(socket, (struct sockaddr *)&serveraddr, sizeof(serveraddr))) < 0)
    {
    s_socket_close(socket);
    return -1;
    }
    else
	if (s_socket_send(socket, message, strlen(message), 0)==-1)
			printf("error sending \n");
	printf(" message : %s \n",message);
    s_socket_close(socket);

    return 0;
}

unsigned char * s_socket_tcp_recv_msg(int port, int time_sec)
{
	int socket, socket2, rc, length = sizeof(int);
    int totalcnt = 0, on = 1,recvlen=0;
    char temp;
	char buffer[255] = {0};
    struct sockaddr_in serveraddr;
    struct sockaddr_in their_addr;
    int addrlen = sizeof(their_addr);
    fd_set read_fd;
    struct timeval timeout;
    timeout.tv_sec = time_sec;
    timeout.tv_usec = 0;

    if((socket = s_socket_create(AF_INET, SOCK_STREAM, 0)) < 0)
    {
     return -1;
    }
    else
    printf("Server-socket() is OK\n");

    if((rc = s_socket_set_sock_opt(socket, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on))) < 0)
    {
    s_socket_close(socket);
    return -1;
    }
    else
    printf("Server-setsockopt() is OK\n");
  
    s_socket_addr_info(INADDR_ANY,port,&serveraddr,sizeof(struct sockaddr_in));
    if((rc = s_socket_bind(socket, (struct sockaddr *)&serveraddr, sizeof(serveraddr))) < 0)
    {
    s_socket_close(socket);
    return -1;
    }
    else
    printf("Server-bind() is OK\n");

    if((rc = s_socket_listen(socket, 10)) < 0)
    {
        s_socket_close(socket);
		return -1;
    }
    else
    printf("Server-Ready for client connection...\n");

    int sin_size = sizeof(struct sockaddr_in);

    if((socket2 = s_socket_accept(socket, (struct sockaddr *)&their_addr, &sin_size)) < 0)
    {
        s_socket_close(socket);
		return -1;
    }
    else
    printf(" socket : %d \n ",socket);
	printf(" socket2 : %d \n ",socket2);
    printf("Server-accept() is OK\n");

    printf("Server-new socket, socket2 is OK...\n");

    printf("Got connection from the client: %s\n", inet_ntoa(their_addr.sin_addr));

    FD_ZERO(&read_fd);
    FD_SET(socket2, &read_fd);
    rc = select(socket2+1, &read_fd, NULL, NULL, &timeout);
    totalcnt = 0;
    if (rc < 0)
    {
		s_socket_close(socket);  
        s_socket_close(socket2);
		return -1;
    }
    else if(rc ==0)
    {
    	printf(" timeout finished !!! \n");
    	s_socket_close(socket);  
        s_socket_close(socket2);
		return -1;
    }
    else
    {
    recvlen = s_socket_recv(socket2, buffer, sizeof(buffer), NULL);
    if(recvlen == -1)
    	printf("errno : %s  \n",strerror(errno));
	printf("received %d bytes \n", recvlen);
    }
    if(rc < 0)
    {
        s_socket_close(socket);  
        s_socket_close(socket2);
		return -1;
    }
    else if (rc == 0)
    {
        s_socket_close(socket);  
        s_socket_close(socket2);
		return -1;
    }
    else
    {
        printf("Server-read() is OK\n");
    }
    

    printf("Received data from the client: %s\n", (char *)buffer);
    printf("Server-Echoing back to client...\n");
    s_socket_close(socket2);
    s_socket_close(socket);
    return buffer;
}

