HEADERS = tcp.c
LIBS = -lwiringPi -pthread
CC = gcc
default: tcp

tcp: rovertcp.c $(HEADERS)
	$(CC) -o tcp.out $(LIBS) $(HEADERS) rovertcp.c

clean:
	-rm -f tcp.out
