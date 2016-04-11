#ifndef TCP_C
#define TCP_C
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "tcp.h"

void tcpError(const char *msg) {
    perror(msg);
    exit(1);
}

void *tcpListener(void *arg){
	char buffer[5];
	int sockfd;
	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		tcpError("ERROR opening socket");
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		tcpError("ERROR on binding");
	listen(sockfd,5);
	clilen = sizeof(cli_addr);
	newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr,&clilen);
	if (newsockfd < 0)
		tcpError("ERROR on accept");
	while(mode){
		bzero(buffer,5);
		n = read(newsockfd,buffer,4);
		mode = buffer[0];
		if(mode)
			printf("Recieved 0x%x\n",buffer[0]);
	}
	close(newsockfd);
	close(sockfd);
	return NULL;
}
#endif
