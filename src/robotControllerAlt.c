#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <termios.h>
#include <linux/joystick.h>
#include "serial_lib.h"

#define DEBUG

void readState(unsigned char state[]) {
	fprintf(stdout, "Enter 8 unsigned integers: ");
	fscanf(stdin, "%hhu %hhu %hhu %hhu %hhu %hhu %hhu %hhu", &state[0], &state[1], &state[2], 
		&state[3], &state[4], &state[5], &state[6], &state[7]);
}

int main(int argc, char* argv[]) {
	char *portname;

	//Set up the COM port controller
	if (argc > 1) {
		portname = argv[1];
	} else {
		portname = "/dev/ttyACM0";
	}

	int fd = serialport_init(portname, 9600);
	if (fd < 0) {
	        fprintf(stderr, "error %d opening %s: %s", errno, portname, strerror(errno));
	        exit(EXIT_FAILURE);
	}

	unsigned char state[8];
	for (int i = 0; i < 8; i++) {
		state[i] = 0;
	}

	state[0] = 4;

	while(1) {
		fprintf(stderr, "Sent:\t");
		for (int i = 0; i < 8; i++) {
			fprintf(stderr, "%u ", state[i]);
		}
		fprintf(stderr, "\n");
		write(fd, state, 8);
		usleep(50000);
		fprintf(stderr, "\n----------------------------------------------------------------\n");
	}
}