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

int main(int argc, char *argv[]){
	int listenfd = 0, connfd = 0;
	struct sockaddr_in serv_addr;

	char recvBuff[1025];
	time_t ticks;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	memset(&serv_addr, '0', sizeof(serv_addr));
	memset(recvBuff, '0', sizeof(recvBuff));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(9090);
	
	bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

	listen(listenfd, 10);

	while(1){
		connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);

		ticks = time(NULL);
		
		//Send current time for example
		//snprintf(sendBuff, sizeof(sendBuff), "%.24s\r\n", ctime(&ticks));
		int n = 0;
		while((n = read(connfd, recvBuff, sizeof(recvBuff)-1))>0){
			recvBuff[n] = 0;
			if(fputs(recvBuff, stdout) == EOF){
				printf("\nError: fputs error\n");
			}
		}

		close(connfd);
		sleep(1);
	}

	return 0;
}
