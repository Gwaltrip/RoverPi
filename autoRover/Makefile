HEADERS = Compass.c gps.c
LIBS = -lm -lwiringPi -pthread
CC = gcc
default: auto

auto: rovergps.c $(HEADERS)
	$(CC) -o auto.out $(LIBS) $(HEADERS) rovergps.c

clean:
	-rm -f auto.out
