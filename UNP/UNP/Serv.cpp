#include <iostream>
using namespace std;

int main(int argc, char **argv)
{
	int listenfd, connfd;

	struct sockaddr_in servaddr;
	char	buff[MAXLINE];
	time_t	ticks;

	listenfd = Socket();
}