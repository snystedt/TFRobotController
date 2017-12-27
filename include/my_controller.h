#ifndef _MY_CONTROLLER_H_
#define _MY_CONTROLLER_H_

#define MY_MSG_LENGTH 4

static unsigned char msg[MY_MSG_LENGTH];

unsigned char *get_msg(const unsigned char *controller, int length);

#endif