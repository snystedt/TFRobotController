#include "threads.h"

// Status struct for debug
status_struct status_data = {controller_state, serial_state};

int create_controller_thread(char *joy_file)
{
	if ((joy_fd = get_joystick(joy_file)) == -1) {
		fprintf(stderr, "Error while opening joystick file\n");
		return -1;
	}

	controller_data = malloc(sizeof(controller_struct));

	controller_data->button_state = 0;
	/* Zero out the button and axis-states */
	for (int i = 0; i < AXIS_NO; i++) {
		controller_data->axis_state[i] = 0;
	}

	// Start thread that updates controller state
	if (pthread_create(&controller_thread,
			   NULL,
			   &controller_thread_start,
			   (void*)controller_data) != 0) {
		perror("Error while creating controller thread\n");
		free(controller_data);
		return -1;
	}

	return 0;
}

int remove_controller_thread()
{
	int errorcode;

	if (errorcode = (pthread_cancel(controller_thread)) != 0) {
		fprintf(stderr, "Controller thread not succesfully exited...\n");
		return errorcode;
	}

	fprintf(stderr, "Controller thread exited succesfully\n");
	close(joy_fd);

	return errorcode;
}

void *controller_thread_start(void *arg) {
	/* Parse the config-file and initialize the button and
	axis structs */
	parse_config();
	fprintf(stdout, "Read config file successfully\n");

	while (true) {
		update_state(joy_fd, controller_data);
		collect_state(controller_data, controller_state);
	}
	fprintf(stderr, "Controller thread exiting...\n");
}

int create_serial_thread(char* filename)
{
	if (!strcmp(filename, "/dev/(null)")) {
		serial_fd = 0;
	} else {
		serial_fd = serialport_init(filename, 9600);
	}

	if (serial_fd < 0) {
		fprintf(stderr, "error %d opening %s: %s\n", errno,
		filename, strerror(errno));
		return -1;
	}

	set_fd(serial_fd);

	serial_running = true;

	if (pthread_create(&serial_thread, NULL, &start_serial_thread, NULL) != 0) {
		perror("pthread_create\n");
		serial_running = false;
		return -1;
	}

	return 0;
}

int remove_serial_thread() {

	serial_running = false;

	int errorcode = pthread_join(serial_thread, NULL);
	close(serial_fd);

	return errorcode;
}

void *start_serial_thread(void* arg) {
	fprintf(stdout, "Sleeping for 5 seconds to initialize serial communications\n");
	usleep(5000000);

	while(serial_running) {

		for (int i = 0; i < BUTTON_NO + AXIS_NO - 2; i++) {
			serial_state[i] = controller_state[i];
		}
		write_state(serial_state, BUTTON_NO + AXIS_NO - 2, 35000);
		for (uint i = 0; i < BUTTON_NO + AXIS_NO; i++) {
			fprintf(stderr, "%d ", (int)serial_state[i]);
		}
		fprintf(stderr, "\n");
	}
}
