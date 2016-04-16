HEADERS = Compass.c gps.c csv.c
LIBS = -lm -lwiringPi -lgps -pthread
OBJ = auto.out
CC = gcc
default: auto

auto: rovergps.c $(HEADERS)
	$(CC) -o $(OBJ) $(LIBS) $(HEADERS) rovergps.c

clean:
	-rm -f $(OBJ)
