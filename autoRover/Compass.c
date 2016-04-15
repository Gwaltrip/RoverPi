// Gabriel Waltrip																
// gcc spiCompass.c -o spiCompass.out -lwiringPi -lm
#include <wiringPiI2C.h>
#include <math.h>
#include "Compass.h"

#define ADDRESS         0x1e
#define CONFIG_REG_A    0x00
#define CONFIG_REG_B    0x01
#define MODE_REG        0x02
#define X_MSB           0x03
#define X_LSB           0x04
#define Y_MSB           0x07
#define Y_LSB           0x08

#define GAUSS           1.3f
#define PI              3.1416f
#define SAMPLE_RATE_MS  100
#define COMP_ERR	33

#define I2CRead(X)      wiringPiI2CReadReg8(compassfd,X)
#define I2CWrite(X,Y)   wiringPiI2CWriteReg8(compassfd,X,Y)

int compassfd;
double DigitalResolution;

void compassInit() {
	compassfd = wiringPiI2CSetup(ADDRESS);
	I2CWrite(CONFIG_REG_A, 0x70);
	I2CWrite(MODE_REG, 0x00);
	DigitalResolution = 1.3;
	setScale();
	delay(16);
}

float heading() {
	float x_axis = makeWord(I2CRead(X_MSB), I2CRead(X_LSB)) * DigitalResolution;
	float y_axis = makeWord(I2CRead(Y_MSB), I2CRead(Y_LSB)) * DigitalResolution;
	float heading = atan2(y_axis, x_axis);
	heading = (heading > 0 ? heading : (2 * PI + heading)) * 180 / PI;
        heading += COMP_ERR;
        heading = ( heading > 360.0f ? heading - 360.0f : heading );
	return heading;
}

inline short makeWord(unsigned char HB, unsigned char LB) {
	return (HB << 8) | LB;
}

void setScale() {
	unsigned char value = 0;
	if (GAUSS == 0.88) {
		value = 0x00;
		DigitalResolution = 0.73;
	}
	else if (GAUSS == 1.3) {
		value = 0x01;
		DigitalResolution = 0.92;
	}
	else if (GAUSS == 1.9) {
		value = 0x02;
		DigitalResolution = 1.22;
	}
	else if (GAUSS == 2.5) {
		value = 0x03;
		DigitalResolution = 1.52;
	}
	else if (GAUSS == 4.0) {
		value = 0x04;
		DigitalResolution = 2.27;
	}
	else if (GAUSS == 4.7) {
		value = 0x05;
		DigitalResolution = 2.56;
	}
	else if (GAUSS == 5.6) {
		value = 0x06;
		DigitalResolution = 3.03;
	}
	else if (GAUSS == 8.1) {
		value = 0x07;
		DigitalResolution = 4.35;
	}
	value <<= 5;
	I2CWrite(CONFIG_REG_B, value);
}

