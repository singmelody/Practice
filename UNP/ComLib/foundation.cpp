#include "StdAfx.h"
#include "foundation.h"


int Socket(int family, int type, int protocol)
{
	int n;
	if( ( n = socket( family, type, protocol)) < 0)
		err_sys("socket error");

	return n;
}

void Bind(int fd, SA* sa, int salen)
{
	if(bind( fd, sa, salen) < 0)
		err_sys("bind error");
}

void Listen(int fd, int backLog)
{
	char* ptr;

	if( ( ptr = getenv("LISTENQ")) != NULL)
		backLog = atoi(ptr);

	if(listen( fd, backLog) < 0)
		err_sys("listen error");

}

void Write(int fd, char* ptr, int nBytes)
{
#if WIN32
	if(recv( fd, ptr, nBytes, 0) != nBytes)
#else
	if(write( fd, ptr, nBytes) != nBytes)
#endif
		err_sys("write error");
}

void Close(int fd)
{
	if(closesocket(fd) == -1)
		err_sys("close error");
}

int Accept(int fd, SA* sa, socklen_t* salenptr)
{
	int n;

	while( ( n = accept( fd, sa, salenptr)) < 0)
	{
		if( errno == EPROTO || errno == ECONNABORTED || errno == ECONNABORTED)
			continue;

		err_sys("accept error");
	}

	printf("Accept Success\n");

	return n;
}

int snprintf(char * buff, size_t len, const char *fmt, ...)
{
	int n;
	va_list ap;
	
	va_start( ap, fmt);
	vsprintf( buff, fmt, ap);
	n = strlen(buff);
	va_end(ap);
	
	if( n >= len)
		err_quit("snprintf : %s overflowed array", fmt);

	return n;
}

void err_sys(const char * fmt, ...)
{
	// temp imp
	printf("%s\n", fmt);

#ifdef WIN32
	WSACleanup( );
#endif

	va_list ap;

	va_start( ap, fmt);
	err_doit( 1, LOG_ERR, fmt, ap);
	va_end(ap);

	exit(1);
}

/* Print message and return to caller
 * Caller specifies "errnoflag" and "level" */

static void 
	err_doit(int errnoflag, int level, const char* fmt, va_list ap)
{
	int errno_save;
	char buf[MAXLINE + 1];

	errno_save = errno;

	vsprintf( buf, fmt, ap);
	if(errnoflag)
		sprintf( buf + strlen(buf), ": %s", strerror(errno_save));

	strcat( buf, "\n");


	fflush( stdout);		/* in case stdout and stderr are the same */
	fputs( buf, stderr);
	fflush(stderr);
}

void err_quit(const char * fmt, ...)
{
	va_list	ap;

	va_start( ap, fmt);
	err_doit( 0, LOG_ERR, fmt, ap);
	va_end(ap);
	exit(1);
}

#ifdef WIN32
void InitWinSocket()
{
	WSADATA WSAData;

	//Init Windows Socket
	if ( WSAStartup(MAKEWORD(2,2), &WSAData) != 0 )
	{
		printf("Init Windows Socket Failed::%d", GetLastError());
		exit(1);
	}
}

#endif

void Inet_pton(int family, const char *strptr, void *addrptr)
{
	int n;

	if(( n = inet_pton( family, strptr, addrptr)) < 0)
		err_sys("inet_pton error for %s", strptr);
	else if( n == 0)
		err_quit("inet_pton error for %s", strptr);

	return;
}

void Connect(int fd,const SA* sa, socklen_t salenptr)
{
	if(connect( fd, sa, salenptr) < 0)
		err_sys("connect error");
}

void str_cli(FILE * fp, int sockfd)
{
	char sendLine[MAXLINE];
	char recvLine[MAXLINE];
	while( fgets( sendLine, MAXLINE, fp) != NULL)
	{
		Writen( sockfd, sendLine, strlen(sendLine));

		if(Readline(sockfd, recvLine, MAXLINE) == 0)
			err_quit("str_cli : server terminated prematurely");

		fputs( recvLine, stdout);
	}
}

void str_echo(int sockfd)
{
	size_t n;
	char buff[MAXLINE];

again:
	while( ( n = recv( sockfd, buff, MAXLINE, 0)) > 0)
	{
		Writen( sockfd, buff, n);
	}

	if( n < 0 && errno == EINTR)
		goto again;
	else if( n < 0 )
		err_sys("str_echo : read error");
}

char* Fgets(char *, int, FILE *)
{
	return NULL;
}

void Fputs(const char *, FILE *)
{

}


size_t readline(int fd, void* str, size_t len)
{
	size_t n, rc = 0;
	char c, *ptr;

	ptr = (char*)str;

	for(n = 1; n < len; n++)
	{
again:
		if( ( rc == recv( fd, &c, 1, 0)) == 1)
		{
			*ptr++ = c;
			if( c == '\n' )
				break;
		}
		else if( rc == 0 )
		{
			*ptr = 0;
			return (n-1);
		}
		else
		{
			if(errno == EINTR)
				goto again;

			return -1;
		}
	}

	*ptr = 0;

	return n;
}


size_t	Readline(int fd, void *vptr, size_t maxlen) 
{
	size_t n;
	if( ( n = readline( fd, vptr, maxlen)) < 0)
		err_sys("");

	return n;
}

size_t writen(int fd, const void* str, size_t len)
{
	size_t nleft;
	size_t nwritten;
	const char* ptr;

	ptr = (const char*)str;
	nleft = len;

	while(nleft > 0)
	{
		if( (nwritten = send( fd, ptr, nleft, 0)) <= 0)
		{
			if(nwritten < 0 && errno == EINTR)
				nwritten = 0;
			else
				return -1;
		}

		nleft -= nwritten;
		ptr += nwritten;
	}

	return len;
}

void Writen(int fd, void* vptr, size_t nbytes)
{
	if(writen( fd, vptr, nbytes) != nbytes)
		err_sys("writen error");
}

size_t Readn(int fd, void* buff, size_t nbyte, size_t nbytes)
{
	size_t nleft;
	size_t nread;
	char* ptr;

	ptr = (char*)buff;
	nleft = nbytes;

	while(nleft > 0)
	{
		if( (nread = recv( fd, ptr, nleft, 0)) < 0 )
		{
			if( errno == EINTR )
				nread = 0;
			else
				return -1;
		}
		else if( nread == 0)
			break;

		nleft -= nread;
		ptr   += nread;
	}

	return (nbytes - nleft);
}

int sockfd_to_family(int sockfd)
{
	struct sockaddr_storage ss;
	socklen_t len;
	len = sizeof(ss);
	if(getsockname( sockfd, (SA*)&ss, &len) < 0)
		return -1;

	return ss.ss_family;
}
