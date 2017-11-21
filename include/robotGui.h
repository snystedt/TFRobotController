#ifndef ROBOTGUI_H
#define ROBOTGUI_H
#include <stdlib.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <glib.h>
#include <errno.h>

#include "threads.h"

#define BYTE_NO 8
#define AXIS_NO_SHOW 6

static GtkWidget *status_bar[BYTE_NO];
static GtkWidget *button_bar[16];
static GtkWidget *axis_bar[8];
static char *status_name[BYTE_NO] = {"", "", "",  "", "",
                                        "", "", ""};

static GtkWidget *joystick_connect_button;
static GtkWidget *joystick_disconnect_button;
static GtkWidget *serial_connect_button;
static GtkWidget *serial_disconnect_button;

// A B X Y R1 R3 L1 L3 START SELECT XBOX DPAD_UP DPAD_DOWN DPAD_LEFT DPAD_RIGHT

int gui_main(int argc, char* argv[]);
#endif
