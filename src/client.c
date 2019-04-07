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
	char sendBuff[1024];
	struct sockaddr_in serv_addr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	memset(&serv_addr, '0', sizeof(serv_addr));
	memset(sendBuff, '0', sizeof(sendBuff));
	memset(recvBuff, '0', sizeof(recvBuff));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(3490);
	serv_addr.sin_addr.s_addr = inet_addr(ip);

	if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
		puts("\nConnection failed");
		return 1;
	}
	else{
		puts("Connected!");
		while(1){
			printf(">> ");
			fgets(sendBuff, sizeof(sendBuff), stdin);	
			if(sendBuff[0] == '/'){
				if(strstr(sendBuff, "/exit")){
					break;
				}
			}
			if( send(sockfd, sendBuff, strlen(sendBuff), 0) < 0){
				puts("Send failed");
				continue;
			}
			
			//puts("Starting recv");
			int n;
			n = recv(sockfd, recvBuff, sizeof(recvBuff), 0);
			recvBuff[n] = 0;
			if( n < 0){
				puts("Recv failed");
				continue;
			}
			//puts("Stop recv");

			printf("> ");
			printf(recvBuff);
			memset(sendBuff, '0', sizeof(sendBuff));
			memset(recvBuff, '0', sizeof(recvBuff));
		}
		close(sockfd);
		puts("Connection closed!");
	}
	
	return 0;
}
