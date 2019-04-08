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
#include <pthread.h>

void *send_thread(void *);
void *recv_thread(void *);

int buffer_size = 1024;

char *local_name, **remote_names;

// Client
int main(int argc, char *argv[]) {
	int sockfd = 0, connfd = 0;
	char *ip = "127.0.0.1";
	struct sockaddr_in serv_addr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	memset(&serv_addr, '0', sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(3490);
	serv_addr.sin_addr.s_addr = inet_addr(ip);

	if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
		puts("\nConnection failed");
		return 1;
	}
	else{
		puts("Connected!");

		//GETTING NAMES
		local_name = "pwn3r1";
		int clients_num;

		remote_names = malloc(sizeof(char) * 100 * clients_num);
		memset(remote_names, '0', sizeof(char) * 100 * clients_num);

		// getting num and list of clients from server

		remote_names[0] = local_name;
		remote_names[1] = "pwn3r2";	
		//-------------

		//socket preparation
		int *connfd;
		connfd = malloc(1);
		*connfd = sockfd;

		//THREADING
		pthread_t thread_for_send;
		pthread_t thread_for_recv;
		if(pthread_create(&thread_for_send, NULL, send_thread, (void *)connfd) < 0){
			perror("could not create thread for send");
		}
		if(pthread_create(&thread_for_recv, NULL, send_thread, (void *)connfd) < 0){
			perror("could not create thread for receive");
		}
		pthread_join(thread_for_send, NULL);
		pthread_join(thread_for_recv, NULL);

		close(sockfd);
		puts("Connection closed!");
	}
	
	return 0;
}

void *send_thread(void *connfd){
	char sendBuff[buffer_size];
	int socket = *(int*)connfd;
	memset(sendBuff, '0', sizeof(sendBuff));

	while(1){
		printf("<%s>>> ", local_name);
		fgets(sendBuff, sizeof(sendBuff), stdin);	

		if(sendBuff[0] == '/'){
			if(strstr(sendBuff, "/exit")){
				break;
			}
		}
		if( send(socket, sendBuff, strlen(sendBuff), 0) < 0){
			puts("Send failed");
		}

		memset(sendBuff, '0', sizeof(sendBuff));
		sleep(1);
	}
	return 0;
}

void *recv_thread(void *connfd){
	char recvBuff[buffer_size];
	int socket = *(int*)connfd;
	memset(recvBuff, '0', sizeof(recvBuff));

	while(1){
		int n;
		n = recv(socket, recvBuff, sizeof(recvBuff), 0);
		recvBuff[n] = 0;
		if( n < 0){
			puts("Receive failed");
		}
		else{
			//printf("\n<%s> ", remote_names[1]);
			printf(recvBuff);
			//printf("\n<%s>>> ", local_name);
		}

		memset(recvBuff, '0', sizeof(recvBuff));
	}

	return 0;
}
