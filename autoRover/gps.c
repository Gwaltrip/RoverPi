/*Gabriel Waltrip*/
//This Gps libary is written using OO techniques.
//This class uses a singleton to make sure there
//is only one instance of a singleton
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <gps.h>
#include "gps.h"
struct __GpsPrivate__ {
	float error;
	float longitude;
	float latitude;
	int ThreadID;
	pthread_t UpdateThread;
	void *(*UpdateGps)(void* args);
}*__GpsPrivate__Section;

float GetError__GpsPrivate__Section(void){
	return __GpsPrivate__Section->error;
}

float GetLongitude__GpsPrivate__Section(void) {
	return __GpsPrivate__Section->longitude;
}

float GetLatitude__GpsPrivate__Section(void) {
	return __GpsPrivate__Section->latitude;
}

void UpdateThreadStart__GpsPrivate__Section(void) {
	printf("Starting Update GPS Thread: %x\n",__GpsPrivate__Section->UpdateThread);
	if (__GpsPrivate__Section->ThreadID == -1){
		//Starts a new Pthread
		__GpsPrivate__Section->ThreadID
			= pthread_create(
				&(__GpsPrivate__Section->UpdateThread),
				NULL,
				__GpsPrivate__Section->UpdateGps,
				"");
		printf("New Thread ID: %d\n", __GpsPrivate__Section->ThreadID);
	}
	else{
		printf("ERROR: Tried to start a thread which is already running!\n");
	}
}

void UpdateThreadKill__GpsPrivate__Section(void){
	printf("Killing Update Thread\n");
	if(__GpsPrivate__Section->ThreadID == -1){
		pthread_cancel(__GpsPrivate__Section->ThreadID);
		printf("Thread %d was killed!\n",__GpsPrivate__Section->ThreadID);
		__GpsPrivate__Section->ThreadID = -1;
	}
	else{
		printf("Thread was never alive!\n");
	}
}

void *UpdateGps__GpsPrivate__Section(void* args){
	int rc;
	struct gps_data_t gps;
	if ((rc = gps_open("localhost", "2947", &gps)) == -1) {
	    printf("code: %d, reason: %s\n", rc, gps_errstr(rc));
	}
	else{
		printf("Update Thread Running!\n");
		gps_stream(&gps, WATCH_ENABLE | WATCH_JSON, NULL);
		while(1){
			if (gps_waiting (&gps,2000000)){
				if(gps_read(&gps)==-1){
					printf("ERROR: gps not read correctly");
				}
				else{
					if ((gps.status == STATUS_FIX) &&
                			(gps.fix.mode == MODE_2D || gps.fix.mode == MODE_3D) &&
                			!isnan(gps.fix.latitude) &&
                			!isnan(gps.fix.longitude)) {
						printf("EPX: %f EPY: %f\n",gps.fix.epx,gps.fix.epy);
						__GpsPrivate__Section->longitude = gps.fix.longitude;
						__GpsPrivate__Section->latitude = gps.fix.latitude;
						__GpsPrivate__Section->error = (gps.fix.epx >= gps.fix.epy)?gps.fix.epx:gps.fix.epy;
					}
				}
			}
		}
	}
}

void GpsInit(struct Gps *current){
	printf("Initializing Gps Object!\n");
	__GpsPrivate__Section = malloc(sizeof(struct __GpsPrivate__));
	__GpsPrivate__Section->longitude = 0.0f;
	__GpsPrivate__Section->latitude = 0.0f;
	__GpsPrivate__Section->ThreadID = -1;
	__GpsPrivate__Section->UpdateGps = UpdateGps__GpsPrivate__Section;

	//Addresses the class to target the currect implemenation of the functions
        current->UpdateThreadKill = UpdateThreadKill__GpsPrivate__Section;
        current->UpdateThreadStart = UpdateThreadStart__GpsPrivate__Section;
        current->GetLongitude = GetLongitude__GpsPrivate__Section;
        current->GetLatitude = GetLatitude__GpsPrivate__Section;
	current->GetError = GetError__GpsPrivate__Section;
}
#define __GpsPrivate__Section
#define __GpsPrivate__
