#include "robotJoystick.h"

void parse_config()
{
	char line[MAX_LINE];
	char word[MAX_LINE];
	int value;
	FILE* config;

	if ((config = fopen("./joy_config", "r"))==NULL) {
		perror("fopen in parse_config()");
		exit(EXIT_FAILURE);
	}

	// Read each line, check if corresponds to a button/axis
	// Store the values
	while (fgets(line, MAX_LINE - 1, config) != NULL) {
		// Lines starting with * should be ignored
		if (line[0] == '*') {
			continue;
		}

		int i = 0, j = 0;

		// Store all characters until whitespace
		while(line[i] != ' ') {
			word[j++] = line[i++];
		}

		word[j] = '\0'; // Terminate word

		// Skip all characters until the first digit
		while(((line[i] < '0') || (line[i] > '9')) && line[i] != '\0') {
			i++;
		}

		// If no number was found before the null-terminator, continue
		if (line[i] == '\0') {
			fprintf(stderr, "No value given for %s\n", word);
			continue;
		}

		value = atoi(&line[i]);

		if (strcmp(word, "") == 0) {
			fprintf(stderr, "Word is an empty string\n");
		} else if (strcmp(word, "A") == 0) {
			button_array[value] = A;
		} else if (strcmp(word, "B") == 0) {
			button_array[value] = B;
		} else if (strcmp(word, "X") == 0) {
			button_array[value] = X;
		} else if (strcmp(word, "Y") == 0) {
			button_array[value] = Y;
		} else if (strcmp(word, "R3") == 0) {
			button_array[value] = R3;
		} else if (strcmp(word, "L3") == 0) {
			button_array[value] = L3;
		}
		/*
		 * These lines were removed since the DPAD for the
		 * corded XBOX 360 controller are implemented as an
		 * axis while the wireless controller implmenets these
		 * as buttons. An axis/button agnostic setup would be
		 * preferable. However this is TODO.
		 */
		else if (strcmp(word, "DPAD_UP") == 0) {
			button_array[value] = UP;
		} else if (strcmp(word, "DPAD_DOWN") == 0) {
			button_array[value] = DOWN;
		} else if (strcmp(word, "DPAD_LEFT") == 0) {
			button_array[value] = LEFT;
		} else if (strcmp(word, "DPAD_RIGHT") == 0) {
			button_array[value] = RIGHT;
		} else if (strcmp(word, "START") == 0) {
			button_array[value] = START;
		} else if (strcmp(word, "SELECT") == 0) {
			button_array[value] = SELECT;
		} else if (strcmp(word, "L1") == 0) {
			button_array[value] = L1;
		} else if (strcmp(word, "R1") == 0) {
			button_array[value] = R1;
		} else if (strcmp(word, "AXIS_LY") == 0) {
			axis_array[value] = LY;
		} else if (strcmp(word, "AXIS_LX") == 0) {
			axis_array[value] = LX;
		} else if (strcmp(word, "AXIS_RY") == 0) {
			axis_array[value] = RY;
		} else if (strcmp(word, "AXIS_RX") == 0) {
			axis_array[value] = RX;
		} else if (strcmp(word, "AXIS_DPAD_Y") == 0) {
			axis_array[value] = DPAD_Y;
		} else if (strcmp(word, "AXIS_DPAD_X") == 0) {
			axis_array[value] = DPAD_X;
		} else if (strcmp(word, "XBOX") == 0) {
			button_array[value] = XBOX;
		} else if (strcmp(word, "L2") == 0) {
			axis_array[value] = L2;
		} else if (strcmp(word, "R2") == 0) {
			axis_array[value] = R2;
		}

	}
	fclose(config);
}

int get_joystick(char* joyfile)
{
	//Blocking mode
	int fd;
	if ((fd = open(joyfile, O_RDONLY)) == -1) {
		char* error = strerror(errno);
		fprintf(stderr, "get_joystick, open() on %s: %s\n", joyfile, error);
		return -1;
	}

	return fd;
}

static inline int get_real_button(int number)
{
	return button_array[number];
}

static inline int get_real_axis (int number)
{
	return axis_array[number];
}

int change_button_state(__u16 *button_state, struct js_event e)
{
	int button_pressed;

	if ((button_pressed = get_real_button(e.number)) == -1) {
		fprintf(stderr, "No entry for button: %d\n", e.number);
	} else {
		if (e.value)
			*button_state |= (1 << button_pressed);
		else
			*button_state &= ~(1 << button_pressed);
	}

	return button_pressed;
}

int change_axis_state(int axis_state[], struct js_event e)
{
	int axis_pressed;

	if ((axis_pressed = get_real_axis(e.number)) == -1) {
		fprintf(stderr, "No entry for axis\n");
	} else {
		axis_state[axis_pressed]  = e.value;
	}

	return axis_pressed;
}

/**
	update_state is reponsible for updating the current controller state. That is,
	it keeps the axis_state array up to date.

	@param joyfd is an open filedescriptor for a joystick
	@param controller_data contains a 16-bit integer for buttons and an array for
							axis states
	@param state is a pointer to an array of integers of size BUTTON_NO + AXIS_NO

*/
void update_state(int joyfd, controller_struct *controller_data)
{
	struct js_event e;
	int axis_pressed;

	axis_pressed = -1;
	if (read(joyfd, &e, sizeof(e)) <= 0)
		perror("read() in read_input");

	if (e.type & JS_EVENT_AXIS)
		axis_pressed = change_axis_state(controller_data->axis_state, e);
	if (e.type & JS_EVENT_BUTTON)
		change_button_state(&controller_data->button_state, e);

#ifdef XBOX_FIX
	if (axis_pressed > 5) {
		fix_xbox_dpad(&controller_data->button_state,
							controller_data->axis_state, axis_pressed);
	}
#endif
}

void fix_xbox_dpad(__u16 *button_state, int axis_state[], int pressed)
{
	/*
	 * Dpad up/down is 11/12 and dpad left/right is 13/14
	 * Dpad_Y is 6 and dpad_X is 7, so this gives down or right
	 * adjust with - 1 for up/left
	 */
	int button = pressed * 2;

	// If d_pad is neutral, no d_pad_button is considered pressed
	if (axis_state[pressed] > 256) {
		*button_state |= (1 << button);
		*button_state &= ~(1 << (button - 1));
	} else if (axis_state[pressed] < -256) {
		*button_state &= ~(1 << button);
		*button_state |= (1 << (button - 1));
	} else {
		*button_state &= ~(1 << button);
		*button_state &= ~(1 << (button - 1));
	}
}

/**
	collect_state takes the 16-bit integer for button state and the axis state array
	and converts it into a byte array. The first 2 bytes of the byte array
	contains the button states and the other AXIS_NO bytes contain the axis
	states.

	The function also converts the axis output from [-32768, 32767] to [0 255].
	The button state is stored as bits in the byte array. i.e if button 4 is
	pressed the 4th bit of state[0] is 1. If button 11 is pressed the 3rd (11-8)
	bit of state[1] is 1.

	@param state the byte array where the state is stored
	@param button_state a 16-bit integer containing the state of the buttons
	@param axis_state
*/
void collect_state(controller_struct *controller_data, unsigned char *state)
{
	__u16 button_state = controller_data->button_state;
	int *axis_state = controller_data->axis_state;
	char axisTemp;
	memcpy(state, &button_state, 2*sizeof(char));
	for (int i = 0; i < AXIS_NO; i++) {
		axisTemp = (axis_state[i])/256 + 128;
		memcpy(state+2+i, &axisTemp, sizeof(char));
	}
}

void print_state(controller_struct *controller_data)
{
	__u16 button_state = controller_data->button_state;
	int *axis_state = controller_data->axis_state;

	fprintf(stderr, "A\tB\tX\tY\tR1\tR3\tL1\tN/A\t");
	fprintf(stderr, "UP\tDOWN\tLEFT\tRIGHT\tSTART\tSELECT\tL3\n");
	while (button_state) {
		fprintf(stderr, "%s", (button_state & 1) ? "1\t" : "\t");
		button_state >>= 1;
	}

	fprintf(stderr, "\nL2\tR2\tLY\tLX\tRY\tRX\n");
	for (int i = 0; i < AXIS_NO; i++) {
		fprintf(stderr, "%d\t", axis_state[i]);

	}
	fprintf(stderr, "\n--------------------------------\n");
	fflush(stderr);
}

int button_is_pressed(unsigned char *state, enum buttonEnum button)
{
	return state[button/8] & (1 << button % 8);
}

unsigned char get_axis(unsigned char *state, enum axisEnum axis)
{
	return state[axis-BUTTON_NO];
}

bool get_axis_thresh(unsigned char *state, enum axisEnum axis, int thresh)
{
	return (int)state[axis - BUTTON_NO] > thresh;
}

bool get_axis_interval(unsigned char *state, enum axisEnum axis, int lower, int upper)
{
	return (state[axis - BUTTON_NO] < upper && state[axis - BUTTON_NO] > lower);
}
