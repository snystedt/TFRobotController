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
static GtkWidget *joystick_refresh_button;
static GtkWidget *serial_connect_button;
static GtkWidget *serial_disconnect_button;
static GtkWidget *serial_refresh_button;

// A B X Y R1 R3 L1 L3 START SELECT XBOX DPAD_UP DPAD_DOWN DPAD_LEFT DPAD_RIGHT

static void connect_joystick(GtkWidget *button, gpointer widget);
static void disconnect_joystick(GtkWidget *menu, gpointer user_data);
static void refresh_joystick(GtkWidget *button, gpointer widget);
static void connect_serial(GtkWidget *button, gpointer widget);
static void disconnect_serial(GtkWidget *menu, gpointer user_data);
static void refresh_serial(GtkWidget *button, gpointer widget);
static gboolean update_status (gpointer data);
static gboolean update_button(gpointer data);
static gboolean update_axis(gpointer data);
char** populate_menu(size_t *size, char* dir, char **names, size_t names_size);
void populate_serial(GtkBuilder *builder);
void populate_joystick(GtkBuilder *builder);
void populate_controller_debug(GObject *grid);
void populate_controller_axis_debug(GObject *grid);
void populate_serial_debug(GObject *grid);
int gui_main(int argc, char* argv[]);
#endif
