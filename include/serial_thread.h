#ifndef SERIAL_THREAD_H
#define SERIAL_THREAD_H

#define SERVO_NO 8

#include <unistd.h>
#include <pthread.h>
#include "arduinoController.h"
#include "serial_lib.h"
#include "robotJoystick.h"

// Serial thread data
static pthread_t serial_thread;
static int serial_fd;
static unsigned char serial_state[SERVO_NO];
// Serial running
pthread_mutex_t serial_running_lock;
static bool serial_running = false;

static unsigned char *controller_state_;

int create_serial_thread(char* filename);
int remove_serial_thread();
void *start_serial_thread(void* arg);
void set_controller_state(unsigned char *state);
unsigned char *get_serial_state();

#endif
