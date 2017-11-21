#ifndef THREADS_H
#define THREADS_H

#include <unistd.h>
#include "robotJoystick.h"
#include "arduinoController.h"
#include "serial_lib.h"
#include "servo.h"

typedef struct status_struct {
	const unsigned char *controller_state;
	const unsigned char *serial_state;
} status_struct;

// Controller thread data
static pthread_t controller_thread;
static int joy_fd;
static controller_struct *controller_data = NULL;
static unsigned char controller_state[BUTTON_NO + AXIS_NO - 2];

// Serial thread data
static pthread_t serial_thread;
static int serial_fd;
static unsigned char serial_state[SERVO_NO];
static bool serial_running = false;

extern status_struct status_data;

int create_controller_thread(char *joy_file);
int remove_controller_thread();
void *controller_thread_start(void *arg);
int create_serial_thread(char* filename);
int remove_serial_thread();
void *start_serial_thread(void* arg);

#endif
