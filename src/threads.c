#include "threads.h"

// Status struct for debug
status_struct status_data;

void setup_threads() {
	status_data.controller_state = get_controller_state();
	status_data.serial_state = get_serial_state();
	set_controller_state(status_data.controller_state);
}
