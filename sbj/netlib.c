#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <netdb.h>

#include <sys/types.h>
#include <unistd.h>

#include <fcntl.h>

extern int	errno;



/* ---------------------------------------------------------------------- */
/*            TCP/IP                                                      */
/* ---------------------------------------------------------------------- */
int
tcp_server_init(portno)
int		portno;
{
	int		sfd;
	struct sockaddr_in	sock_addr;

	int		optval = 1;	/* true */

	if ((sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		return ((-1)*errno);

	if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, (char *)&optval, sizeof(int)) < 0)
	{
		close(sfd);
		return ((-1)*errno);
	}

	bzero(&sock_addr, sizeof(sock_addr));

	sock_addr.sin_family      = AF_INET;
	sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	sock_addr.sin_port        = htons(portno);

	if (bind(sfd, (struct sockaddr *)&sock_addr, sizeof(sock_addr)) < 0)
	{
		close(sfd);
		return ((-1)*errno);
	}

	if (listen(sfd, 5) < 0)
	{
		close(sfd);
		return ((-1)*errno);
	}

	return (sfd);
}

int
tcp_server_init_reuseport(portno)
int		portno;
{
	int		sfd;
	struct sockaddr_in	sock_addr;

	int		optval = 1;	/* true */

	if ((sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		return ((-1)*errno);

	if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, (char *)&optval, sizeof(int)) < 0)
	{
		close(sfd);
		return ((-1)*errno);
	}

	/* Socket port ���� */
	if (setsockopt(sfd, SOL_SOCKET, SO_REUSEPORT, (char *)&optval, sizeof(int)) < 0)
	{
		close(sfd);
		return ((-1)*errno);
	}

	bzero(&sock_addr, sizeof(sock_addr));

	sock_addr.sin_family      = AF_INET;
	sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	sock_addr.sin_port        = htons(portno);

	if (bind(sfd, (struct sockaddr *)&sock_addr, sizeof(sock_addr)) < 0)
	{
		close(sfd);
		return ((-1)*errno);
	}

	if (listen(sfd, 3) < 0)
	{
		close(sfd);
		return ((-1)*errno);
	}

	return (sfd);
}

int
tcp_accept_client(listen_sfd)
int		listen_sfd;
{
	int		cli_sfd;
	socklen_t	clilen;

	struct sockaddr_in	cliaddr;

	clilen = sizeof(cliaddr);

	if ((cli_sfd = accept(listen_sfd, (struct sockaddr *)&cliaddr, &clilen)) < 0)
		return ((-1)*errno);

	return (cli_sfd);
}

int
get_client_ip_port(sfd, ipaddr, portno)
int		sfd;
char	*ipaddr;
int		*portno;
{
	struct sockaddr_in	cliaddr;
	socklen_t	clilen;
	int		dummy1, dummy2, dummy3, dummy4;

	clilen = sizeof(struct sockaddr_in);
	if (getpeername(sfd, (struct sockaddr *)&cliaddr, &clilen) < 0)
		return ((-1)*errno);

	sscanf(inet_ntoa(cliaddr.sin_addr), "%d.%d.%d.%d", &dummy1, &dummy2, &dummy3, &dummy4);
	sprintf(ipaddr, "%d.%d.%d.%d", dummy1, dummy2, dummy3, dummy4);

	*portno = ntohs(cliaddr.sin_port);

	return (0);
}

int
get_local_ipaddr(ipaddr)
char	*ipaddr;
{
	struct sockaddr_in	sock_addr;
	struct hostent		*hp;
	char	hostname[32];

	if (gethostname(hostname, sizeof(hostname)) < 0) return ((-1)*errno);
	if ((hp = gethostbyname(hostname)) == NULL) return ((-1)*errno);

	sock_addr.sin_addr.s_addr = ((struct in_addr *)(hp->h_addr))->s_addr;

	sprintf(ipaddr, "%s", inet_ntoa(sock_addr.sin_addr));

	return (0);
}

int
get_host2ip(host_name, ip_addr)
char	*host_name;
char	*ip_addr;
{
	struct sockaddr_in	sock_addr;
	struct hostent		*hp;

	if ((hp = gethostbyname(host_name)) == NULL) return (-1);

	sock_addr.sin_addr.s_addr = ((struct in_addr *)(hp->h_addr))->s_addr;
	sprintf(ip_addr, "%s", inet_ntoa(sock_addr.sin_addr));

	return (0);
}

int
tcp_connect_server(hostname, portno)
char	*hostname;
int		portno;
{
	int		sfd;
	struct sockaddr_in	sock_addr;
	struct hostent		*hp;

	int		optval = 1;	/* true */

	if ((sfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		return ((-1)*errno);

	if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, (char *)&optval, sizeof(int)) < 0)
	{
		close(sfd);
		return ((-1)*errno);
	}

	if ((hp = gethostbyname(hostname)) == NULL)
	{
		close(sfd);
		return ((-1)*errno);
	}

	bzero(&sock_addr, sizeof(sock_addr));

	sock_addr.sin_family      = AF_INET;
	sock_addr.sin_port        = htons(portno);
	sock_addr.sin_addr.s_addr = ((struct in_addr *)hp->h_addr)->s_addr;

	if (connect(sfd, (struct sockaddr *)&sock_addr, sizeof(sock_addr)) < 0)
	{
		close(sfd);
		return ((-1)*errno);
	}

	return (sfd);
}

/* opt
 *  * 1 : KEEPALIVE
 *   * 2 : NODELAY
 *    * 3 : LINGER
 *     * 4 : NONBLOCK
 *      */
int
tcp_set_sockopt(opt, sfd)
int		opt;
int		sfd;
{
	int		rval, flags;
	int		optval;
	struct linger	linger_opt;

	if (opt == 1)
	{
		optval = 1;
		rval = setsockopt(sfd, SOL_SOCKET, SO_KEEPALIVE, (char *)&optval, sizeof(optval));
		if (rval < 0)
			return (-errno);
	}
	else if (opt == 2)
	{
		optval = 1;
		rval = setsockopt(sfd, IPPROTO_TCP, TCP_NODELAY, (char *)&optval, sizeof(optval));
		if (rval < 0)
			return (-errno);
	}
	else if (opt == 3)
	{
		linger_opt.l_onoff  = 1;
		linger_opt.l_linger = 0;

		if (setsockopt(sfd, SOL_SOCKET, SO_LINGER, &linger_opt, sizeof(linger_opt)) < 0)
			return (-errno);
	}
	else if (opt == 4)
	{
		if ((flags = fcntl(sfd, F_GETFL, 0)) < 0) return (-errno);
		flags |= O_NONBLOCK;
		if ((fcntl(sfd, F_SETFL, flags)) < 0) return (-errno);
	}

	return (0);
}

int
tcp_get_sock_buffsize(sfd, rsize, ssize)
int		sfd;
int		*rsize;
int		*ssize;
{
	int		rval, len;

	len = sizeof(len);
	if (getsockopt(sfd, SOL_SOCKET, SO_RCVBUF, rsize, &len) < 0) return (-errno);
	if (getsockopt(sfd, SOL_SOCKET, SO_SNDBUF, ssize, &len) < 0) return (-errno);

	return (0);
}

int
tcp_close(sfd)
int		sfd;
{
	shutdown(sfd, 2);
	close(sfd);
	return (0);
}


/* ---------------------------------------------------------------------- */
/*            UDP/IP                                                      */
/* ---------------------------------------------------------------------- */
int
udp_server_init(portno)
int		portno;
{
	int		sfd;
	struct sockaddr_in	sock_addr;

	int		optval = 1;	/* true */

	if ((sfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
		return ((-1)*errno);

	if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, (char *)&optval, sizeof(int)) < 0)
	{
		close(sfd);
		return ((-1)*errno);
	}

	bzero(&sock_addr, sizeof(sock_addr));

	sock_addr.sin_family      = AF_INET;
	sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	sock_addr.sin_port        = htons(portno);

	if (bind(sfd, (struct sockaddr *)&sock_addr, sizeof(sock_addr)) < 0)
	{
		close(sfd);
		return ((-1)*errno);
	}

	return (sfd);
}

int
udp_server_init_ip(hostname, portno)
char	*hostname;
int		portno;
{
	int		sfd;
	struct sockaddr_in	sock_addr;
	struct hostent		*hp;

	int		optval = 1;	/* true */

	if ((sfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
		return ((-1)*errno);

	if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, (char *)&optval, sizeof(int)) < 0)
	{
		close(sfd);
		return ((-1)*errno);
	}
	if ((hp = gethostbyname(hostname)) == NULL)
	{
		close(sfd);
		return ((-1)*errno);
	}

	bzero(&sock_addr, sizeof(sock_addr));

	sock_addr.sin_family      = AF_INET;
	sock_addr.sin_addr.s_addr = ((struct in_addr *)hp->h_addr)->s_addr;
	sock_addr.sin_port        = htons(portno);

	if (bind(sfd, (struct sockaddr *)&sock_addr, sizeof(sock_addr)) < 0)
	{
		close(sfd);
		return ((-1)*errno);
	}

	return (sfd);
}

int
udp_server_mcast_init(mcast_ip, portno)
char	*mcast_ip;
int		portno;
{
	int		sfd, optval, len, oplen;
	struct sockaddr_in	sock_addr;
	struct ip_mreq	mcast;

	char	recv[10*1024];

	if ((sfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
		return ((-1)*errno);

	bzero(&mcast, sizeof(mcast));
	mcast.imr_multiaddr.s_addr = inet_addr(mcast_ip);
	mcast.imr_interface.s_addr = htonl(INADDR_ANY);

	if (setsockopt(sfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mcast, sizeof(mcast)) < 0)
	{
		close(sfd);
		return ((-1)*errno);
	}

	optval = 1;	/* true */
	if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, (char *)&optval, sizeof(int)) < 0)
	{
		close(sfd);
		return ((-1)*errno);
	}

	len = sizeof(recv);
	oplen = sizeof(len);

	if (setsockopt(sfd, SOL_SOCKET, SO_RCVBUF, (void *)&len, oplen) < 0)
	{
		close(sfd);
		return ((-1)*errno);
	}

	bzero(&sock_addr, sizeof(sock_addr));
	sock_addr.sin_family      = AF_INET;
	sock_addr.sin_addr.s_addr = inet_addr(mcast_ip);
	sock_addr.sin_port        = htons(portno);

	if (bind(sfd, (struct sockaddr *)&sock_addr, sizeof(sock_addr)) < 0)
	{
		close(sfd);
		return ((-1)*errno);
	}

	return (sfd);
}

int
udp_client_init()
{
	int		sfd;
	int		optval = 1;	/* true */

	if ((sfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
		return ((-1)*errno);

	if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, (char *)&optval, sizeof(int)) < 0)
	{
		close(sfd);
		return ((-1)*errno);
	}

	return (sfd);
}

int
udp_client_init_bc()
{
	int		sfd, rval;

	sfd = udp_client_init();
	if (sfd < 0) return (sfd);

	if ((rval = udp_broadcast_set(sfd)) < 0) return (rval);

	return (sfd);
}

int
udp_client_init_mc(ttl)
int		ttl;
{
	int		sfd, rval;

	sfd = udp_client_init();
	if (sfd < 0) return (sfd);

	if ((rval = udp_multicast_ttl_set(sfd, ttl)) < 0) return (rval);

	return (sfd);
}

int
udp_broadcast_set(sfd)
int		sfd;
{
	int		optval = 1;	/* true */

	if (setsockopt(sfd, SOL_SOCKET, SO_BROADCAST, (char *)&optval, sizeof(int)) < 0)
		return ((-1)*errno);

	return (0);
}

int
udp_multicast_ttl_set(sfd, _ttl)
int		sfd;
int		_ttl;
{
	unsigned char	ttl;

	ttl = (unsigned char)_ttl;

	if (setsockopt(sfd, IPPROTO_IP, IP_MULTICAST_TTL, (char *)&ttl, sizeof(unsigned char)) < 0)
		return ((-1)*errno);

	return (0);
}


/* �۽������� UDP Multicast ó�� �� �ý��׿� Network Device�� ������ ������ �Ǿ� ������
 *  * Ư�� Device�� ���� �� �� �ֵ��� �����Ѵ�.
 *   */
int
udp_multicast_if_set(sfd, ip)
int		sfd;
char	*ip;	/* ���������� �ϴ� NIC IP �ּ� */
{
	struct in_addr	addr;

	memset(&addr, 0x00, sizeof(addr));

	addr.s_addr = inet_addr(ip);

	if (setsockopt(sfd, IPPROTO_IP, IP_MULTICAST_IF, (char *)&addr, sizeof(addr)) < 0)
		return ((-1)*errno);

	return (0);
}

int
udp_close(sfd)
int		sfd;
{
	close(sfd);
	return (0);
}


/* ---------------------------------------------------------------------- */
/*            Unix Domain Socket                                          */
/* ---------------------------------------------------------------------- */
int
UDP_server_init(path)
char	*path;
{
	int		sfd;
	int		optval = 1;
	struct sockaddr_un	sock_addr;

	if ((sfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
		return (-1*errno);

	if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, (char *)&optval, sizeof(int)) < 0)
	{
		close(sfd);
		return (-1*errno);
	}

	unlink(path);

	bzero(&sock_addr, sizeof(sock_addr));
	sock_addr.sun_family = AF_UNIX;
	strcpy(sock_addr.sun_path, path);

	if (bind(sfd, (struct sockaddr *)&sock_addr, sizeof(sock_addr)) < 0)
	{
		close(sfd);
		return (-1*errno);
	}

	listen(sfd, 3);

	return (sfd);
}

int
UDP_accept_client(sfd)
int		sfd;
{
	int		newsfd;
	int		sock_len;
	struct sockaddr_un	sock_addr;

	sock_len = sizeof(sock_addr);

	if ((newsfd = accept(sfd, (struct sockaddr *)&sock_addr, &sock_len)) < 0)
		return (-1*errno);

	return (newsfd);
}

int
UDP_connect_server(path)
char	*path;
{
	int		sfd;
	struct sockaddr_un	sock_addr;

	if ((sfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
		return (-1*errno);

	bzero(&sock_addr, sizeof(sock_addr));
	sock_addr.sun_family = AF_UNIX;
	strcpy(sock_addr.sun_path, path);

	if (connect(sfd, (struct sockaddr *)&sock_addr, sizeof(sock_addr)) < 0)
	{
		close(sfd);
		return (-1*errno);
	}

	return (sfd);
}


/* ---------------------------------------------------------------------- */
/*            I/O Interface                                               */
/* ---------------------------------------------------------------------- */
int
tcp_send(sfd, buff, size)
int		sfd;
char	*buff;
int		size;
{
	int		nleft, pos;
	int		nwritten, nread;

	nleft = size;
	pos   = 0;

	while (nleft > 0)
	{
		if ((nwritten = send(sfd, (char *)buff+pos, nleft, 0)) < 0)
			return ((-1)*errno);

		nleft -= nwritten;
		pos   += nwritten;
	}

	return (pos);
}

int
tcp_select_send(sfd, buff, ssize)
int		sfd;
char	*buff;
int		ssize;
{
	fd_set	fdset;
	struct timeval	tv;
	int		retval;

	tv.tv_sec  = 3;
	tv.tv_usec = 0;

	FD_ZERO(&fdset);
	FD_SET(sfd, &fdset);

	retval = select(sfd+1, 0, &fdset, 0, &tv);
	if (retval == 0)
	{
		return (-9990);		/* select time out */
	}
	else if (retval < 0)
	{
		return (-1*errno);
	}

	if (FD_ISSET(sfd, &fdset))
	{
		retval = send(sfd, buff, ssize, 0);
		if (retval < 0)
		{
			return (-1*errno);
		}
		else if (retval == 0)
		{
			return (-9999);
		}

		/* data send ok!!! */
		return (retval);
	}

	return (-1);
}

int
tcp_send_size(sfd, buff, ssize)
int		sfd;
char	*buff;
int		ssize;
{
	int		nleft;
	int		nwritten;
	int		pos;

	pos = 0;
	nleft = ssize;

	while (nleft > 0)
	{
		nwritten = tcp_select_send(sfd, buff+pos, nleft);
		if (nwritten < 0) return (nwritten);

		nleft -= nwritten;
		pos += nwritten;
	}

	return (pos);
}

int
tcp_send_size_no_select(sfd, buff, ssize)
int		sfd;
char	*buff;
int		ssize;
{
	int		nleft;
	int		nwritten;

	nleft = ssize;

	while (nleft > 0)
	{
		nwritten = send(sfd, buff, nleft, 0);
		if (nwritten < 0)
		{
			return (-1*errno);
		}
		else if (nwritten == 0)
		{
			return (-9999);
		}

		nleft -= nwritten;
		buff += nwritten;
	}

	return (ssize);
}

int
tcp_recv(sfd, buff, max_size)
int		sfd;
char	*buff;
int		max_size;
{
	int		datalen;

	datalen = recv(sfd, buff, max_size, 0);
	if (datalen < 0)
		return ((-1)*errno);

	buff[datalen] = 0x00;

	return (datalen);
}

int
tcp_select_recv(sfd, buff, rsize)
int		sfd;
char	*buff;
int		rsize;
{
	fd_set	fdset;
	struct timeval	tv;
	int		retval;

	tv.tv_sec  = 3;
	tv.tv_usec = 0;

	FD_ZERO(&fdset);
	FD_SET(sfd, &fdset);

	retval = select(sfd+1, &fdset, 0, 0, &tv);
	if (retval == 0)
	{
		return (-9990);		/* select time out */
	}
	else if (retval < 0)
	{
		return (-1*errno);
	}

	if (FD_ISSET(sfd, &fdset))
	{
		retval = recv(sfd, buff, rsize, 0);
		if (retval < 0)
		{
			/* reset by peer. used linger option */
			if (errno == 104) return (-9999);

			return (-1*errno);
		}
		else if (retval == 0)
		{
			return (-9999);
		}

		/* data recv ok!!! */
		return (retval);
	}

	return (-1);
}

int
tcp_recv_size(sfd, buff, rsize)
int		sfd;
char	*buff;
int		rsize;
{
	int		rlen;
	int		pos, nleft, nread;

	rlen = tcp_select_recv(sfd, buff, rsize);
	if (rlen < 0) return (rlen);

	pos = rlen;

	if (rsize > rlen)
	{
		nleft = rsize - rlen;

		while (nleft > 0)
		{
			nread = tcp_select_recv(sfd, buff+pos, nleft);
			if (nread < 0) return (nread);

			nleft -= nread;
			pos += nread;
		}
	}

	return (rsize);
}

int
tcp_recv_size_no_select(sfd, buff, rsize)
int		sfd;
char	*buff;
int		rsize;
{
	int		nread, totsize = 0;

	while (totsize != rsize)
	{
		nread = recv(sfd, buff, rsize-totsize, 0);
		if (nread < 0)
		{
			/* reset by peer. used linger option */
			if (errno == 104) return (-9999);

			return (-1*errno);
		}
		else if (nread == 0)
		{
			return (-9999);
		}

		totsize += nread;
		buff += nread;
	}

	return (totsize);
}

int
tcp_recv_size_tm(sfd, buff, rsize, wait_time)
int		sfd;
char	*buff;
int		rsize;
int		wait_time;
{
	int		rlen;
	int		pos, nleft, nread;

	rlen = tcp_select_recv_tm(sfd, buff, rsize, wait_time);
	if (rlen < 0) return (rlen);

	pos = rlen;

	if (rsize > rlen)
	{
		nleft = rsize - rlen;

		while (nleft > 0)
		{
			nread = tcp_select_recv(sfd, buff+pos, nleft);
			if (nread < 0) return (nread);

			nleft -= nread;
			pos += nread;
		}
	}

	return (rsize);
}

int
tcp_select_recv_tm(sfd, buff, rsize, wait_time)
int		sfd;
char	*buff;
int		rsize;
int		wait_time;
{
	fd_set	fdset;
	struct timeval	tv;
	int		retval;

	tv.tv_sec  = wait_time;
	tv.tv_usec = 0;

	FD_ZERO(&fdset);
	FD_SET(sfd, &fdset);

	if (wait_time == 0)
		retval = select(sfd+1, &fdset, 0, 0, 0);
	else
		retval = select(sfd+1, &fdset, 0, 0, &tv);

	if (retval == 0)
	{
		return (-9990);		/* select time out */
	}
	else if (retval < 0)
	{
		return (-1*errno);
	}

	if (FD_ISSET(sfd, &fdset))
	{
		retval = recv(sfd, buff, rsize, 0);
		if (retval < 0)
		{
			/* reset by peer. used linger option */
			if (errno == 104) return (-9999);

			return (-1*errno);
		}
		else if (retval == 0)
		{
			return (-9999);
		}

		/* data recv ok!!! */
		return (retval);
	}

	return (-1);
}

int
tcp_recv_sock_status(sfd)
int		sfd;
{
	fd_set	fdset;
	struct timeval	tv;
	int		retval;

	tv.tv_sec  = 0;
	tv.tv_usec = 0;

	FD_ZERO(&fdset);
	FD_SET(sfd, &fdset);

	retval = select(sfd+1, &fdset, 0, 0, &tv);
	if (retval == 0)
	{
		return (0);		/* select time out */
	}
	else if (retval < 0)
	{
		return (-1*errno);
	}

	if (FD_ISSET(sfd, &fdset)) return (1);

	return (0);
}

int
tcp_socket_clear(sfd)
int     sfd;
{
	char    rbuff[20000];

	tcp_select_recv(sfd, rbuff, sizeof(rbuff));
	return (0);
}

int
udp_recv(sfd, buff, max_length, ipaddr)
int		sfd;
char	*buff;
int		max_length;
char	*ipaddr;
{
	struct sockaddr_in	cliaddr;
	socklen_t	clilen;
	int		datalen;

	int		dummy1, dummy2, dummy3, dummy4;

	clilen = sizeof(cliaddr);
	datalen = recvfrom(sfd, buff, max_length, 0, (struct sockaddr *)&cliaddr, &clilen);
	if (datalen < 0)
		return ((-1)*errno);

	buff[datalen] = 0x00;

	sprintf(ipaddr, "%s", inet_ntoa(cliaddr.sin_addr));

	return (datalen);
}

int
udp_send(sfd, buff, size, ipaddr, portno)
int		sfd;
char	*buff;
int		size;
char	*ipaddr;
int		portno;
{
	struct sockaddr_in	cliaddr;
	socklen_t	clilen;
	struct hostent		*hp;
	int		retval;

	if ((hp = gethostbyname(ipaddr)) == NULL)
		return ((-1)*errno);

	bzero(&cliaddr, sizeof(cliaddr));

	cliaddr.sin_family      = AF_INET;
	cliaddr.sin_port        = htons(portno);
	cliaddr.sin_addr.s_addr = ((struct in_addr *)hp->h_addr)->s_addr;

	clilen = sizeof(cliaddr);

	retval = sendto(sfd, buff, size, 0, (struct sockaddr *)&cliaddr, clilen);
	if (retval < 0)
		return ((-1)*errno);

	return (retval);
}

int
wait_event(sfd)
int		sfd;
{
	fd_set	fdset;
	struct timeval	tv;
	int		retval;

	tv.tv_sec  = 5;
	tv.tv_usec = 0;

	FD_ZERO(&fdset);
	FD_SET(sfd, &fdset);

	retval = select(sfd+1, &fdset, 0, 0, &tv);
	if (retval <= 0)
		return (-1);

	if (FD_ISSET(sfd, &fdset))
		return (0);
	else
		return (-1);
}

int
wait_event2(sfd, timeout)
int		sfd;
int		timeout;
{
	fd_set	fdset;
	struct timeval	tv;
	int		retval;

	tv.tv_sec  = timeout;
	tv.tv_usec = 0;

	FD_ZERO(&fdset);
	FD_SET(sfd, &fdset);

	retval = select(sfd+1, &fdset, 0, 0, &tv);
	if (retval <= 0)
		return (-1);

	if (FD_ISSET(sfd, &fdset))
		return (0);
	else
		return (-1);
}

int
wait_event3(sfd, timeout)
int		sfd;
int		timeout;
{
	fd_set	fdset;
	struct timeval	tv;
	int		retval;

	tv.tv_sec  = timeout;
	tv.tv_usec = 0;

	FD_ZERO(&fdset);
	FD_SET(sfd, &fdset);

	retval = select(sfd+1, &fdset, 0, 0, &tv);
	if (retval < 0)
		return (-1);
	else if (retval == 0)
		return (9999);

	if (FD_ISSET(sfd, &fdset))
		return (0);
	else
		return (-1);
}

int
wait_event_usec(sfd, timeout)
int		sfd;
int		timeout;
{
	fd_set	fdset;
	struct timeval	tv;
	int		retval;

	tv.tv_sec  = 0;
	tv.tv_usec = timeout;

	FD_ZERO(&fdset);
	FD_SET(sfd, &fdset);

	retval = select(sfd+1, &fdset, 0, 0, &tv);
	if (retval < 0)
		return (-1);
	else if (retval == 0)
		return (9999);

	if (FD_ISSET(sfd, &fdset))
		return (0);
	else
		return (-1);
}

