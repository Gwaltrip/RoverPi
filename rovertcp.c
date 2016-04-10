// Gabriel Waltrip
// Rover.cpp : Defines the entry point for the console application.
// gcc -o rover.out -lwiringPi -lm -pthread rover.c Compass.h gps.h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wiringPi.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define MOTOR_RIGHT_A	0
#define MOTOR_RIGHT_B	2
#define MOTOR_LEFT_A	3
#define MOTOR_LEFT_B	4

#define Stop_All_Motors()	digitalWrite(MOTOR_RIGHT_A,0);\
				digitalWrite(MOTOR_RIGHT_B,0);\
				digitalWrite(MOTOR_LEFT_A,0);\
				digitalWrite(MOTOR_LEFT_B,0);

int n;
int newsockfd;
int portno = 8080;
char mode = 0xff;

void error(const char *msg);
void *tcpListener(void *arg);

int main(int argc, char **argv)
{
	char last = mode;
	pthread_t tcp;
	pthread_create(&tcp, NULL,tcpListener,"");
	/*Starts Main Loop*/
	printf("Starting Mainloop!\n");
	while (mode) {
		if(last != mode){
			Stop_All_Motors();
			last == mode;
		}
		//Fordward
		if(mode == 0x1){
			digitalWrite(MOTOR_RIGHT_A,1);
			digitalWrite(MOTOR_LEFT_A,1);
		}
		//Backwards
		else if(mode == 0x2){
			digitalWrite(MOTOR_RIGHT_B,1);
			digitalWrite(MOTOR_LEFT_B,1);
		}
		//Left
		else if (mode == 0x2){
			digitalWrite(MOTOR_RIGHT_A,1);
			digitalWrite(MOTOR_LEFT_B,1);
		}
		//Right
		else if (mode == 0x3){
			digitalWrite(MOTOR_RIGHT_B,1);
			digitalWrite(MOTOR_LEFT_A,1);
		}
		delay(50);
	}
	return 0;
}

void error(const char *msg) {
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
		error("ERROR opening socket");
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		error("ERROR on binding");
	listen(sockfd,5);
	clilen = sizeof(cli_addr);
	newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr,&clilen);
	if (newsockfd < 0)
		error("ERROR on accept");
	while(mode){
		bzero(buffer,5);
		n = read(newsockfd,buffer,4);
		mode = buffer[0];
		if(mode)
			printf("Recieved %c\n",buffer[0]);
	}
	close(newsockfd);
	close(sockfd);
	return NULL;
}
