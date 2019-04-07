#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

// Server
int main(int argc, char *argv[]){
	int listenfd = 0, connfd = 0, n;
	struct sockaddr_in serv_addr;
	char recvBuff[1025];
	
	memset(&serv_addr, '0', sizeof(serv_addr));
	memset(recvBuff, '0', sizeof(recvBuff));

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if(listenfd == -1){
		perror("Could not create socket");
		return 1;
	}
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(3490);
	
	if(bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0){
		perror("bind failed.");
		return 1;
	}
	listen(listenfd, 10);

	puts("Wating for connections...");
	int c = sizeof(struct sockaddr_in);
	while(1){
		
		connfd = accept(listenfd, (struct sockaddr *)&serv_addr, (socklen_t *)&c);
		if(connfd < 0){
			perror("accept failed");
			return 1;
		}
		puts("Connection!");
		while((n =  recv(connfd, recvBuff, sizeof(recvBuff), 0)) > 0){
			recvBuff[n] = 0;
			printf(recvBuff);
			//printf("%d\n", strlen(recvBuff));
			if(send(connfd, recvBuff, strlen(recvBuff), 0) < 0){
				puts("send failed");
			}
			memset(recvBuff, '0', sizeof(recvBuff));
		}
		
		if (n == 0){
			puts("Client disconnected");
			fflush(stdout);
		}
		else if (n == -1){
			perror("recv failed");
		}
	}	

	return 0;
}
