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

	return n;
}

int snprintf(char *, size_t, const char *, ...)
{
	return 0;
}

void err_sys(const char * fmt, ...)
{
	WSACleanup( );

	va_list ap;

	va_start( ap, fmt);
	err_doit( 1, LOG_ERR, fmt, ap);
	va_end(ap);


	exit(1);
}

/* Print message and return to caller
 * Caller specifies "errnoflag" and "level" */

static void err_doit(int erronoflag, int level, const char* fmt, va_list ap)
{

}

void err_quit(const char *, ...)
{

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
	while( Fgets( sendLine, MAXLINE, fp) != NULL)
	{
		Writen( sockfd, sendLine, strlen(sendLine));

		if(Readline(sockfd, recvLine, MAXLINE) == 0)
			err_quit("str_cli : server terminated prematurely");

		Fputs( recvLine, stdout);
	}
}

void Writen(int fd, void* ptr, size_t nbytes)
{
	if(write( fd, ptr, nbytes) != nbytes)
		err_sys("writeen error");
}

char* Fgets(char *, int, FILE *)
{

}

void Fputs(const char *, FILE *)
{

}

size_t	Readline(int, void *, size_t)
{

}

size_t Writen(int fd, const void* vptr, size_t nbytes)
{
	size_t nleft;
	size_t nwritten;
	const char* ptr;

	ptr = (const char*)vptr;
	nleft = nbytes;

	while(nleft > 0)
	{
		if( (nwritten = write( fd, ptr, nleft)) <= 0)
		{
			if(nwritten < 0 && errno == EINTR)
				nwritten = 0;
			else
				return -1;
		}

		nleft -= nwritten;
		ptr += nwritten;
	}
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
		if( (nread = read( fd, ptr, nleft)) < 0 )
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
