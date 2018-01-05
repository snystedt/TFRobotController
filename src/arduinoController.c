#include "arduinoController.h"

void write_state(const unsigned char *msg, int msg_length, int delay) {

#ifdef _MY_CONTROLLER_H_
	msg = get_msg(msg, msg_length);
#endif

	if (fd && msg) {
		write(fd, msg, msg_length);
	}
	usleep(delay);
}

void set_fd(int serial_fd) {
	fd = serial_fd;
}
