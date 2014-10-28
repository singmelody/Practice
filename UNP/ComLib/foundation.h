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
#define	SERV_PORT		 13

typedef int socklen_t;

// socket functions
int Socket(int , int ,int );

void Bind(int , SA*, int);

void Listen( int, int);

void Close( int );

int Accept( int , SA*, socklen_t*);

void Connect(int,const SA*, socklen_t);

size_t writen(int fd, const void* str, size_t len);

void Writen(int fd, void* ptr, size_t nbytes);	// write localbuffer 2 socketbuffer

void Write(int, void *, size_t); // write  localbuffer 2 socketbuffer

size_t Readn(int files, void* buff, size_t nbyte, size_t nbytes);

size_t my_read(int fd, char* ptr);

size_t readline(int fd, const void* str, size_t len);

size_t Readline(int, void *, size_t);

// transform functions
void Inet_pton(int, const char *, void *);

void str_cli(FILE *, int);

void str_echo(int sockfd);

int snprintf(char *, size_t, const char *, ...);

// error functions
void err_sys(const char *, ...);

void err_quit(const char *, ...);

void err_doit(int errnoflag, int level, const char* fmt, va_list ap);

void InitWinSocket();

// io functions
void	 Fclose(FILE *);
FILE	*Fdopen(int, const char *);
char	*Fgets(char *, int, FILE *);
FILE	*Fopen(const char *, const char *);
void	 Fputs(const char *, FILE *);



int sockfd_to_family(int sockfd);

enum ERRLEVEL
{
	LOG_ERR
};