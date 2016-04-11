HEADERSGPS = autoRover/Compass.c autoRover/gps.c
HEADERSTCP = tcpRover/tcp.c
LIBSGPS = -lm
LIBSBLUE = -lbluetooth
LIBS = -lwiringPi -pthread
CC = gcc
default: all

all: gps bluetooth tcp

gps: rovergps.c $(HEADERS)
	$(CC) -o RoverGps.out $(LIBSGPS) $(LIBS) $(HEADERSGPS) autoRover/rovergps.c

bluetooth: roverblue.c $(HEADERS)
	$(CC) -o RoverBluetooth.out $(LIBS) $(LIBSBLUE)  autoRover/roverblue.c

tcp: rovertcp.c $(HEADERS)
	$(CC) -o RoverTCP.out $(LIBS) $(HEADERSTCP)  tcpRover/rovertcp.c

clean:
	-rm -f RoverGps.out
	-rm -f RoverBluetooth.out
	-rm -f RoverTCP.out
