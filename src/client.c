#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

// Client
int main(int argc, char *argv[]) {
	int sockfd = 0, connfd = 0;
	char *ip = "127.0.0.1";
	char recvBuff[1024];
	struct sockaddr_in serv_addr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	memset(&serv_addr, '0', sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(3490);
	serv_addr.sin_addr.s_addr = inet_addr(ip);

	if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
		printf("\nConnection failed\n");
		return 1;
	}
	else{
		printf("Connected!\n");
	}
	
	return 0;
}
