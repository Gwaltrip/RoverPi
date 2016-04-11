// Gabriel Waltrip
// Rover.cpp : Defines the entry point for the console application.
// gcc -o rover.out -lwiringPi -lm -pthread rover.c Compass.h gps.h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <wiringPi.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

#define MOTOR_RIGHT_A	0
#define MOTOR_RIGHT_B	2
#define MOTOR_LEFT_A	3
#define MOTOR_LEFT_B	4

#define Stop_All_Motors()	digitalWrite(MOTOR_RIGHT_A,0);\
				digitalWrite(MOTOR_RIGHT_B,0);\
				digitalWrite(MOTOR_LEFT_A,0);\
				digitalWrite(MOTOR_LEFT_B,0);
//typedef uint32_t socklen_t

char mode = 0xff;

void error(const char *msg);
void *blueListener(void *arg);

int main(int argc, char **argv)
{
	char last = mode;
	pthread_t blue;
	pthread_create(&blue,NULL,blueListener,"");

        wiringPiSetup();
        pinMode(MOTOR_LEFT_A,OUTPUT);
        pinMode(MOTOR_LEFT_B,OUTPUT);
        pinMode(MOTOR_RIGHT_A,OUTPUT);
        pinMode(MOTOR_RIGHT_B,OUTPUT);
	/*Starts Main Loop*/
	printf("Starting Mainloop!\n");
	while (mode) {
		if(last != mode){
			Stop_All_Motors();
			last = mode;
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
	Stop_All_Motors();
	return 0;
}

void error(const char *msg) {
    perror(msg);
    exit(1);
}

void *blueListener(void *arg){
	char buffer[5];
	int s, client, bytes_read;
	struct sockaddr_rc loc_addr = {0},
			   rem_addr = {0};
	socklen_t opt = sizeof(rem_addr);

	s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

	loc_addr.rc_family = AF_BLUETOOTH;
	loc_addr.rc_bdaddr = *BDADDR_ANY;
	loc_addr.rc_channel = (uint8_t) 1;
	bind(s, (struct sockaddr *)&loc_addr, sizeof(loc_addr));

	listen(s,1);

	client = accept(s, (struct sockaddr *)&rem_addr, &opt);
    	while(1){
		bytes_read = read(client, buffer, sizeof(buffer));
    		if( bytes_read > 0 ) {
    			mode = buffer[0];
		}
	}
	// close connection
	close(client);
	close(s);

	return NULL;
}
