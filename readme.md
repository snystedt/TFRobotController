# TFRobotController
TFRobotController is a simple program for Linux which reads a joystick and forwards the joystick state over to a serial device.

## How to build
First, there are some prerequisites.

- A Linux distro like Ubuntu or Linux mint
- The `joystick` package (for debian: `sudo apt-get install joystick`)
- GTK+ (for debian: `sudo apt-get install libgtk-3-dev`)

To build, simply navigate to the `src` folder and use the `make` command. An executable can then be found in `bin`.

### Build with debug
To enable some debug tools (ugly terminal debugging for the serial commands and the ability to set the serial file to (null) for testing without a device) compile with the macro `DEBUG`. To do this, add the flag `-DDEBUG` to the `CFLAGS` variable in the makefile.

## Usage
The program automatically looks for any joysticks and serial devices connected to the computer. Click the refresh button to look for these devices again while the program is running. Once a joystick and serial device have been chosen, click the connect button on the joystick and make sure that pressing its buttons and axis trigger the visual representation in the program. Click the connect button on the serial device and wait 5 seconds, once 5 seconds have passed the serial message being sent should be shown in the view below.

## Under the hood
The program sends 8 bytes of data from the computer to the serial device. The first two bytes contain information concerning which buttons are pressed. The buttons are encoded in bit-level. Seeing the two button bytes as 16 consecutive bits, the buttons are ordered as A = 0, B = 1, X = 2, Y = 3, R1 = 4, R3 = 5, L1 = 6, L3 = 7, START = 8, SELECT = 9, XBOX = 10, UP = 11, DOWN = 12, LEFT = 13, RIGHT = 14. The axis information are contained in the following 6 bytes. The first two axis bytes contain the L2 and R2 trigger in that order. These triggers have a value of 1 when released and increase to 255 as the trigger is pressed down. The remaining 4 bytes contain the stick information. The 4th and 5th byte (remember the byte numbering starts at 0) contain the left stick's Y and X axis. The axis has a value of ~128 in resting position and goes to 0 in one direction and to 255 in the other direction. The 6th and 7th byte work the same but for the right stick.

The program sends this data every 35ms. This can be configured in the `robotController.c` source file by changing the value of the macro `SERIAL_PERIOD`. Note that this value is given in microseconds.

This information is useful when retrieving the data on the serial device.
