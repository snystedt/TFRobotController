#ifndef ROBOTJOYSTICK_H
#define ROBOTJOYSTICK_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <linux/joystick.h>

#define MAX_LINE 1024 // Maximum length of line in config file
#define BUTTON_NO 2 // ceil(number_of_buttons/8), bytes needed to encode button
#define AXIS_NO 8 // To change this one must also change axis_struct!

typedef struct controller_struct {
	__u16 button_state;
	int axis_state[AXIS_NO];
} controller_struct;

enum buttonEnum {	A = 0, B = 1, X = 2, Y = 3,
	 					R1 = 4, R3 = 5, L1 = 6, L3 = 7,
						START = 8, SELECT = 9, XBOX = 10,
						UP = 11, DOWN = 12, LEFT = 13, RIGHT = 14};
enum axisEnum {L2 = 0, R2 = 1, LY = 2, LX = 3,
					RY = 4, RX = 5, DPAD_Y = 6, DPAD_X = 7};

static int button_array[16] = {-1};
static int axis_array[16] = {-1};

void parse_config();
int get_joystick(char* joyfile);
static inline int get_real_button(int number);
static inline int get_real_axis (int number);
int change_button_state(__u16 *button_state, struct js_event e);
int change_axis_state(int axis_state[], struct js_event e);
void update_state(int joyfd, controller_struct *controller_data);
void fix_xbox_dpad(__u16 *button_state, int axis_state[], int pressed);
void print_state(controller_struct *controller_data);
void collect_state(controller_struct *controller_data, unsigned char *state);
int button_is_pressed(unsigned char *state, enum buttonEnum button);
unsigned char get_axis(unsigned char *state, enum axisEnum axis);
bool get_axis_thresh(unsigned char *state, enum axisEnum axis, int thresh);
bool get_axis_interval(unsigned char *state, enum axisEnum axis, int lower, int upper);

#endif
