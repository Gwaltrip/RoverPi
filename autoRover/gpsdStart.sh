#!/bin/bash
echo "Killing last gpsd"
killall gpsd
echo "Starting gpsd on USB0"
gpsd /dev/ttyUSB0 -F /var/run/gpsd.sock
