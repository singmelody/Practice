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
