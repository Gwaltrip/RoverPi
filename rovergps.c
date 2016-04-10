// Gabriel Waltrip
// Rover.cpp : Defines the entry point for the console application.
// gcc -o rover.out -lwiringPi -lm -pthread rover.c Compass.h gps.h
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <wiringPi.h>
#include <stdio.h>
#include "gps.h"
#include "Compass.h"

#define SAMPLING_TIME_MS 	300
#define MOTOR_STOP		100
#define ERROR_DISTANCE_M 	14
#define RADIUS_EARTH_M 		6371000
#define PI			3.1416
#define COMPASS_ERR		30

#define MOTOR_LEFT_BACKWARD	0
#define MOTOR_LEFT_FORWARD	1
#define MOTOR_RIGHT_BACKWARD	2
#define	MOTOR_RIGHT_FORWARD	3

#define Stop_Motors_For(ms)		digitalWrite(MOTOR_LEFT_BACKWARD,0);\
					digitalWrite(MOTOR_LEFT_FORWARD,0);\
					digitalWrite(MOTOR_RIGHT_BACKWARD,0);\
					digitalWrite(MOTOR_RIGHT_FORWARD,0);\
					delay(ms);

struct TargetCords {
	double Longitude;
	double Latitude;
	struct TargetCords* Next;
	struct TargetCords* Back;
};

double bearing(struct Gps* current, struct TargetCords* target);
double distance(struct Gps* current, struct TargetCords* target);


int main(int argc, char **argv)
{
	printf("Starting Rover!\n");
	/*Starts Inits*/
	struct Gps *current = malloc(sizeof(struct Gps));
	struct TargetCords* target = malloc(sizeof(struct TargetCords));
	target->Latitude = 0.0f;
	target->Longitude = 0.0f;
	target->Next = 0;
	target->Back = 0;
	int _heading;
	int _bearing;

	setScale();
	current->GpsInit = GpsInit;
	current->GpsInit(current);
	current->UpdateThreadStart();

	pinMode(MOTOR_LEFT_BACKWARD,OUTPUT);
	pinMode(MOTOR_LEFT_FORWARD,OUTPUT);
	pinMode(MOTOR_RIGHT_BACKWARD,OUTPUT);
	pinMode(MOTOR_RIGHT_FORWARD,OUTPUT);

	/*Ends Inits*/
	/*Starts Main Loop*/
	printf("Starting Mainloop!\n");
	while (target->Next) {
		_heading = (int)heading();
		_bearing = (int)bearing(current,target);
		if(distance(current,target)>0){
			//Forwards
			if((_heading-_bearing) % 360 < (_heading + COMPASS_ERR) % 360
			&& (_heading-_bearing) % 360 > (_heading + COMPASS_ERR) % 360){
				Stop_Motors_For(MOTOR_STOP);
			}
			//Backwards
			else if((_heading-_bearing-180) % 360 < (_heading + COMPASS_ERR) % 360
                             && (_heading-_bearing-180) % 360 > (_heading + COMPASS_ERR) % 360){
				Stop_Motors_For(MOTOR_STOP);
			}
			//Left
			else if ((_bearing - _heading) % 360 > (180-COMPASS_ERR)){
				Stop_Motors_For(MOTOR_STOP);
				while((_heading-_bearing) % 360 < (_heading + COMPASS_ERR) % 360
                        	   && (_heading-_bearing) % 360 > (_heading + COMPASS_ERR) % 360){
					digitalWrite(MOTOR_RIGHT_FORWARD,0);
					delay(10);
					digitalWrite(MOTOR_RIGHT_FORWARD,1);
					delay(10);
				}
			}
			//Right
			else if ((_bearing - _heading) % 360 < (COMPASS_ERR)){
				Stop_Motors_For(MOTOR_STOP);
                                while((_heading-_bearing) % 360 < (_heading + COMPASS_ERR) % 360
                                   && (_heading-_bearing) % 360 > (_heading + COMPASS_ERR) % 360){
                                        digitalWrite(MOTOR_LEFT_FORWARD,0);
                                        delay(10);
                                        digitalWrite(MOTOR_LEFT_FORWARD,1);
                                        delay(10);
                                }
			}
		}
	}
	delay(100);
	current->UpdateThreadKill();
	printf("Stopping Rover!\n");
	free(current);
	return 0;
}

double bearing(struct Gps* current, struct TargetCords* target) {
	double theda = atan2(cos(current->GetLatitude())
				*sin(target->Latitude)
				-sin(current->GetLatitude())
				*cos(target->Latitude)
				*cos(target->Longitude
				-current->GetLongitude()),
				sin(target->Longitude-current->GetLongitude())
				*cos(target->Latitude)
				);
	return theda;
}

double distance(struct Gps* current,struct TargetCords* target) {
	double alpha = sin(((target->Latitude - current->GetLatitude()) * PI / 180.0f) / 2);
	double beta = sin(((target->Longitude - current->GetLongitude()) * PI / 180.f) / 2);
	alpha = alpha*alpha
		  + cos(current->GetLatitude() * PI / 180.0f)
	      * cos(target->Latitude * PI / 180.0f)
		  * beta * beta;

	alpha = 2 * atan2(sqrt(alpha), sqrt(1 - alpha)) * RADIUS_EARTH_M;

	return (alpha < ERROR_DISTANCE_M)?0.0:alpha;
}
