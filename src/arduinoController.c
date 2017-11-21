#include "arduinoController.h"

void write_state(const unsigned char *msg, int msg_length, int delay) {
	if (fd) {
		write(fd, msg, msg_length);
	}
	usleep(delay);
}

void set_fd(int serial_fd) {
	fd = serial_fd;
}
