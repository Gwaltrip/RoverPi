#ifndef COMPASS_H
#define COMPASS_H
#include <pthread.h>
inline short makeWord(unsigned char HB, unsigned char LB);
float heading();
void setScale();
void (*compassThreadStart());
void compassThread();
struct Compass{
	int ThreadID;
	double heading;
	pthread_t Thread;
}compass;
#endif
