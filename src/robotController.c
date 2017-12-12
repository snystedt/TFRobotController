#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <termios.h>
#include <linux/joystick.h>
#include <pthread.h>
#include "robotGui.h"

#define SERIAL_PERIOD 35000 // The delay in microseconds between messages

int main(int argc, char* argv[]) {
	//initiateAngleMatrix();
	int status = gui_main(argc, argv);

	return status;
}
