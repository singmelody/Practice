// Srv.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "foundation.h"
#include <ctime>
int _tmain(int argc, _TCHAR* argv[])
{
	int		listenfd, connfd;
	struct sockaddr_in servaddr;
	char buff[MAXLINE];
	time_t ticks;

	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	memset( &servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(13);

	Bind( listenfd, (SA*) &servaddr, sizeof(servaddr));

	Listen( listenfd, LISTENQ);

	for(;;)
	{
		connfd = Accept( listenfd, (SA*)NULL, NULL);

		ticks = time(NULL);
		snprintf( buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));

		Write( connfd, buff, strlen(buff));
		
		Close( connfd );
	}
	return 0;
}

