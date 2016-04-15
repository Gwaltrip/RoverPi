
#include <stdlib.h>
#include "gps.h"
#include <stdio.h>
#include <wiringPi.h>

int main(){
	struct Gps *current = malloc(sizeof(struct Gps));

        current->GpsInit = GpsInit;
        current->GpsInit(current);
        current->UpdateThreadStart();
	int i = 0;
	while((i++)<1000){

		delay(100);
	}
	return 0;
}
