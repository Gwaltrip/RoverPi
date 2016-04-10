HEADERS = Compass.c gps.c
LIBS = -lm -lwiringPi -pthread
CC = gcc
default: all

all: gps bluetooth tcp

gps: rovergps.c $(HEADERS)
	$(CC) -o RoverGps.out $(LIBS) $(HEADERS) rovergps.c

bluetooth: roverblue.c $(HEADERS)
	$(CC) -o RoverBluetooth.out $(LIBS) -lbluetooth  roverblue.c

tcp: rovertcp.c $(HEADERS)
	$(CC) -o RoverTCP.out $(LIBS) rovertcp.c

clean:
	-rm -f RoverGps.out
	-rm -f RoverBluetooth.out
	-rm -f RoverTCP.out
