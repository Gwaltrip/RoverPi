HEADERSGPS = autoRover/Compass.c autoRover/gps.c autoRover/csv.c
HEADERSTCP = tcpRover/tcp.c
LIBSGPS = -lm -lgps
LIBSBLUE = -lbluetooth
LIBS = -lwiringPi -pthread
AUTODIR = autoRover
BLUEDIR = blueRover
TCPDIR = tcpRover
AUTOOBJ = auto.out
BLUEOBJ = blue.out
TCPOBJ = tcp.out
CC = gcc
NETC = dmcs
BIN = bin
default: all

all: mkbin gps bluetooth tcp monoconsole

mkbin:
	mkdir -p $(BIN)

gps: mkbin
	$(CC) -o $(BIN)/RoverGps.out $(LIBSGPS) $(LIBS) $(HEADERSGPS) autoRover/rovergps.c
	cp $(AUTODIR)/targets.csv $(BIN)/targets.csv
	cp $(AUTODIR)/gpsdStart.sh $(BIN)/gpsdStart.sh

bluetooth: mkbin
	$(CC) -o $(BIN)/RoverBluetooth.out $(LIBS) $(LIBSBLUE)  blueRover/roverblue.c

tcp: mkbin
	$(CC) -o $(BIN)/RoverTCP.out $(LIBS) $(HEADERSTCP)  tcpRover/rovertcp.c

monoconsole: mkbin
	$(NETC) ./PiSockets/Program.cs -out:./$(BIN)/client.mono

clean:
	-rm -f $(BIN)/$(AUTOOBJ) || true
	-rm -f $(BIN)/$(BLUEOBJ) || true
	-rm -f $(BIN)/$(TCPOBJ) || true
	-rm -f $(BIN)/client.mono || true
	-rm -f $(BIN)/targets.csv || true
	-rm -rf $(BIN) || true
	-rm -f $(AUTODIR)/$(AUTOOBJ) || true
	-rm -f $(BLUEDIR)/$(BLUEOBJ) || true
	-rm -f $(TCPDIR)/$(TCPOBJ) || true
