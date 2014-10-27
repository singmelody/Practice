#pragma once

#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <errno.h>
#include <cstdlib>
#include <stdio.h>

#define MAXLINE 256
/* Following could be derived from SOMAXCONN in <sys/socket.h>, but many
   kernels still #define it as 5, while actually supporting many more */
#define	LISTENQ		1024
#define	SA	struct sockaddr
#define	SERV_PORT		 9877

typedef int socklen_t;

int Socket(int , int ,int );

void Bind(int , SA*, int);

void Listen( int, int);

void Write( int, char*, int );

void Writen( int, void*, size_t);

void Close( int );

int Accept( int , SA*, socklen_t*);

void Connect(int,const SA*, socklen_t);

void Inet_pton(int, const char *, void *);

void str_cli(FILE *, int);

void str_echo(int sockfd);

int snprintf(char *, size_t, const char *, ...);

void err_sys(const char *, ...);

void err_quit(const char *, ...);

void err_doit(int ,int , const char* , va_list);

void InitWinSocket();

char*	Fgets(char *, int, FILE *);

void	Fputs(const char *, FILE *);

size_t Readn(int files, void* buff, size_t nbyte, size_t nbytes);

size_t Writen(int files, const void* buff, size_t nbytes);

size_t	Readline(int, void *, size_t);

int sockfd_to_family(int sockfd);

enum ERRLEVEL
{
	LOG_ERR
};