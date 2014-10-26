#pragma once

#include <WinSock2.h>
#include <errno.h>
#include <cstdlib>
#include <stdio.h>

#define MAXLINE 256
/* Following could be derived from SOMAXCONN in <sys/socket.h>, but many
   kernels still #define it as 5, while actually supporting many more */
#define	LISTENQ		1024
#define	SA	struct sockaddr

typedef int socklen_t;

int Socket(int , int ,int );

void Bind(int , SA*, int);

void Listen( int, int);

void Write( int, char*, int );

void Close( int );

int Accept( int , SA*, socklen_t*);

int snprintf(char *, size_t, const char *, ...);

void err_sys(const char *, ...);

void err_doit(int ,int , const char* , va_list);

void InitWinSocket();

enum ERRLEVEL
{
	LOG_ERR
};