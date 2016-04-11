LIBS =  -lwiringPi -pthread
CC = gcc
default: blue

blue: roverblue.c $(HEADERS)
	$(CC) -o blue.out $(LIBS) -lbluetooth  roverblue.c

clean:
	-rm -f blue.out
