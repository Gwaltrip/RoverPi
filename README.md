# RoverPi
This is a 3 part controller for a tank tred rover.


The first controller is using tcp/ip. This reads a byte in and then determines how to react.
To Compile this alone use "make tcp"

The secound controller is using Bluetooth. This reads a byte in and then determines how to react.
To Compile this alone use "make blue"

The last controller is using a GPS and a compass to determines how to react.
To Compile this alone use "make gps"

To make all of theses at the sametime just use "make"
