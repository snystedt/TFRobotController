#include "controller_thread.h"

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

unsigned char *get_controller_state() {
        return controller_state;
}
