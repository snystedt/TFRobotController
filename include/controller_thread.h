#ifndef CONTROLLER_THREAD_H
#define CONTROLLER_THREAD_H

#include <unistd.h>
#include <pthread.h>
#include "robotJoystick.h"

// Controller thread data
static pthread_t controller_thread;
static int joy_fd;
static controller_struct *controller_data = NULL;
static unsigned char controller_state[BUTTON_NO + AXIS_NO - 2] = {0, 0, 1, 1, 128, 128, 128, 128};

int create_controller_thread(char *joy_file);
int remove_controller_thread();
void *controller_thread_start(void *arg);
unsigned char *get_controller_state();

#endif
