#pragma once

#include <stdio.h>
#include <WinSock2.h>

#define MAXLINE 256
/* Following could be derived from SOMAXCONN in <sys/socket.h>, but many
   kernels still #define it as 5, while actually supporting many more */
#define	LISTENQ		1024
#define	SA	struct sockaddr

int Socket(int , int ,int );

int Bind(int , SA*, int);

int Listen( int, int);

int Write( int, char*, int );

int Close( int );

int Accept( int , SA*, void*);

int snprintf(char *, size_t, const char *, ...);
