// Gabriel Waltrip
// Rover.cpp : Defines the entry point for the console application.
// gcc -o rover.out -lwiringPi -lm -pthread rover.c Compass.h gps.h
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <wiringPi.h>
#include <stdio.h>
#include "csv.h"
#include "gps.h"
#include "Compass.h"

#define SAMPLING_TIME_MS 	300
#define MOTOR_STOP		100
#define ERROR_DISTANCE_M 	14
#define RADIUS_EARTH_M 		6371000
#define PI			3.1416
#define MIN_COMPASS_ERR		5
#define MAX_COMPASS_ERR		50

#define MOTOR_LEFT_BACKWARD	0
#define MOTOR_LEFT_FORWARD	1
#define MOTOR_RIGHT_BACKWARD	2
#define	MOTOR_RIGHT_FORWARD	3

#define Stop_Motors_For(ms)		digitalWrite(MOTOR_LEFT_BACKWARD,0);\
					digitalWrite(MOTOR_LEFT_FORWARD,0);\
					digitalWrite(MOTOR_RIGHT_BACKWARD,0);\
					digitalWrite(MOTOR_RIGHT_FORWARD,0);\
					delay(ms);
double bearing(struct Gps* current, struct TargetCords* target);
double distance(struct Gps* current, struct TargetCords* target);
inline int compassError(struct Gps* current, double _distance);

int main(int argc, char **argv){
	printf("Starting Rover!\n");
	/*Starts Inits*/
	struct Gps *current = malloc(sizeof(struct Gps));
	struct TargetCords* target = malloc(sizeof(struct TargetCords));
	target->Latitude = 0.0f;
	target->Longitude = 0.0f;
	target->Next = 0;
	target->Back = 0;
	target = GetTargetCords("targets.csv");

	int _heading;
	int _bearing;
	int _compassError;
	double _distance;
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
		_distance = distance(current,target);
		_compassError = compassError(current,_distance);
		int leftness = ((_heading-_compassError)%360)<_bearing;
		int rightness = ((_heading+_compassError)%360)>_bearing;
		if(_distance>=current->GetError()){
			//Forwards
			if(leftness&&rightness){
				Stop_Motors_For(MOTOR_STOP);
				digitalWrite(MOTOR_RIGHT_FORWARD,1);
				digitalWrite(MOTOR_LEFT_FORWARD,1);
			}
			//Left
			else if (leftness){
				Stop_Motors_For(MOTOR_STOP);
				while(leftness&&rightness){
					leftness = ((_heading-_compassError)%360)<_bearing;
					rightness = ((_heading+_compassError)%360)>_bearing;
					digitalWrite(MOTOR_RIGHT_FORWARD,0);
					delay(10);
					digitalWrite(MOTOR_RIGHT_FORWARD,1);
					delay(10);
				}
			}
			//Right
			else if (rightness){
				Stop_Motors_For(MOTOR_STOP);
				while(leftness&&rightness){
					leftness = ((_heading-_compassError)%360)<_bearing;
					rightness = ((_heading+_compassError)%360)>_bearing;
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
	while(target->Back){
		target = target->Back;
		free(target->Next);
	}
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

	return (2 * atan2(sqrt(alpha), sqrt(1 - alpha)) * RADIUS_EARTH_M);
}

inline int compassError(struct Gps* current, double _distance){
	int err = atan(current->GetError()/_distance);
	if(err < MIN_COMPASS_ERR)
		err = MIN_COMPASS_ERR;
	else if(err > MAX_COMPASS_ERR)
		err = MAX_COMPASS_ERR;
	return err;
}
