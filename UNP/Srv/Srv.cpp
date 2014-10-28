// Srv.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "foundation.h"
#include <ctime>
int _tmain(int argc, _TCHAR* argv[])
{
	int		listenfd, connfd;
	socklen_t len;
	struct sockaddr_in servaddr, cliaddr;
	char buff[MAXLINE];
	time_t ticks;

#ifdef WIN32
	InitWinSocket();
#endif

	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	memset( &servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(13);

	Bind( listenfd, (SA*) &servaddr, sizeof(servaddr));

	Listen( listenfd, LISTENQ);

	for(;;)
	{
		len = sizeof(cliaddr);
		connfd = Accept( listenfd, (SA*)&cliaddr, &len);
		printf("connection from %s, port %d\n", inet_ntop( AF_INET, &cliaddr.sin_addr, buff, sizeof(buff)), ntohs(cliaddr.sin_port));

		ticks = time(NULL);
		printf( buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));

		Write( connfd, buff, strlen(buff));
		
		Close( connfd );
	}
	return 0;
}

