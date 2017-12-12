#ifndef THREADS_H
#define THREADS_H

#include <unistd.h>
#include "controller_thread.h"
#include "serial_thread.h"

typedef struct status_struct {
	unsigned char *controller_state;
	unsigned char *serial_state;
} status_struct;

extern status_struct status_data;

void setup_threads();

#endif
