#include "robotGui.h"

gboolean set_disconnect_sensitivity() {
	gtk_widget_set_sensitive(serial_disconnect_button, true);
	fprintf(stderr, "Turning on sensitivity\n");
	return FALSE;
}

static void connect_joystick(GtkWidget *button, gpointer widget)
{
        gchar *menu_text;
        GtkWidget *menu = widget;

        menu_text = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(menu));

        char joyfile[256];
        sprintf(joyfile, "/dev/input/%s", menu_text);
        g_free(menu_text);

        if (create_controller_thread(joyfile) == -1) {
                fprintf(stderr, "error in create_controller_thread()\n");
                return;
        }

        gtk_widget_set_sensitive(joystick_connect_button, false);
        gtk_widget_set_sensitive(joystick_disconnect_button, true);
}

static void disconnect_joystick(GtkWidget *menu, gpointer user_data)
{
        if (remove_controller_thread() != 0) {
                fprintf(stderr, "error in remove_controller_thread()\n");
                return;
        }

        gtk_widget_set_sensitive(joystick_connect_button, true);
        gtk_widget_set_sensitive(joystick_disconnect_button, false);
}

static void refresh_joystick(GtkWidget *button, gpointer widget) {
        GtkBuilder *builder = widget;

        populate_joystick(builder);
}

static void connect_serial(GtkWidget *button, gpointer widget)
{
        gchar *menu_text;
        GtkWidget *menu = widget;

        menu_text = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(menu));

        char serialfile[256];
        sprintf(serialfile, "/dev/%s", menu_text);
        g_free(menu_text);

        if (create_serial_thread(serialfile) != 0) {
                fprintf(stderr, "error in create_serial_thread()\n");
                return;
        }

        gtk_widget_set_sensitive(serial_connect_button, false);
        g_timeout_add(5500, (GSourceFunc)set_disconnect_sensitivity, NULL);
}

static void disconnect_serial(GtkWidget *menu, gpointer user_data)
{
        if (remove_serial_thread() != 0) {
                fprintf(stderr, "error in remove_serial_thread()\n");
                return;
        }

        gtk_widget_set_sensitive(serial_connect_button, true);
        gtk_widget_set_sensitive(serial_disconnect_button, false);
}

static void refresh_serial(GtkWidget *button, gpointer widget) {
        GtkBuilder *builder = widget;

        populate_serial(builder);
}

static gboolean update_status (gpointer data)
{
        if (status_data.serial_state == NULL) {
                return TRUE;
        }

#ifdef _MY_CONTROLLER_H_
        int msg_len = MY_MSG_LENGTH;
#else
        int msg_len = 8;
#endif

        char *buff;

        for (int i = 0; i < msg_len; i++) {
                buff = g_strdup_printf("      %03d      ", (int)status_data.serial_state[i]);
                gtk_label_set_text(GTK_LABEL(status_bar[i]), buff);
                g_free(buff);
        }

	return TRUE;
}

static gboolean update_button(gpointer data)
{
        if (status_data.controller_state == NULL) {
                return TRUE;;
        }
        char *buff;

        for (int i = 0; i < 15; i++) {
                buff = g_strdup_printf("      %s      ", (status_data.controller_state[i/8] & (1 << (i%8))) >= 1 ? "X" : "   ");
                gtk_label_set_text(GTK_LABEL(button_bar[i]), buff);
                g_free(buff);
        }

	return TRUE;
}

static gboolean update_axis(gpointer data)
{
        if (status_data.controller_state == NULL) {
                fprintf(stderr, "Error: couldn't fetch controller axis data\n");
                return TRUE;
        }

        char *buff;

        for (int i = 0; i < AXIS_NO_SHOW; i++) {
                buff = g_strdup_printf("     %03d     ", status_data.controller_state[i+2]);
                gtk_label_set_text(GTK_LABEL(axis_bar[i]), buff);
                g_free(buff);
        }

	return TRUE;
}

char** populate_menu(size_t *size, char* dir, char **names, size_t names_size)
{
        /* opendir(dir) won't work without this line... weird behaviour */
        char directoryName[256];
        snprintf(directoryName, 256, "%s", dir);

        DIR *dev;
        if ((dev = opendir(directoryName)) == NULL) {
                char* error = strerror(errno);
                fprintf(stderr, "Error opening directory %s, error msg: %s\n", dir, error);
                *size = 0;
                return NULL;
        }

        struct dirent *entry;

        const int max_entries = 30;

        *size = 0;
        char **serial = calloc(max_entries, sizeof(char*));

        while ((entry = readdir(dev)) != NULL) {
                bool accept = false;
                for (size_t i = 0; i < names_size; i++) {
                        accept = !strncmp(entry->d_name, names[i], strlen(names[i])) || accept;
                }

                if (accept) {
                        size_t name_len = strlen(entry->d_name);
                        serial[*size] = calloc(name_len + 1, sizeof(char));
                        memcpy(serial[*size], entry->d_name, name_len + 1);
                        *size = *size + 1;
                        if (*size == max_entries) break; // Break
                }
        }

#ifdef DEBUG
        if (*size < max_entries) {
                char debug_entry[] = "(null)";
                serial[*size] = calloc(strlen(debug_entry) + 1, sizeof(char));
                strncpy(serial[*size], debug_entry, strlen(debug_entry) + 1);
                *size = *size + 1;
        }
#endif

        if (*size < 30) {
                serial = realloc(serial, (*size) * sizeof(char*));
        }
        else if (*size == 0) {
                serial = NULL;
        }

        return serial;
}

void populate_serial(GtkBuilder *builder)
{
        GObject *combobar;
        combobar = gtk_builder_get_object(builder, "serial_menu");
        gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(combobar));
        size_t names_size = 2;
        char *names[2];
        names[0] = "ttyACM"; names[1] = "ttyUSB";
        size_t size;
        char **items = populate_menu(&size, "/dev/", names, names_size);
        for(size_t i = 0; i < size; i++) {
                gtk_combo_box_text_prepend(GTK_COMBO_BOX_TEXT(combobar), NULL, items[i]);
                free(items[i]);
        }

        free(items);
}

void populate_joystick(GtkBuilder *builder)
{
        GObject *combobar;
        combobar = gtk_builder_get_object(builder, "joystick_menu");
        gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(combobar));
        size_t names_size = 1;
        char *names[1];
        names[0] = "js";
        size_t size;
        char **items = populate_menu(&size, "/dev/input/", names, names_size);

        for(size_t i = 0; i < size; i++) {
                fprintf(stderr, "Appending: %s\n", items[i]);
                gtk_combo_box_text_prepend(GTK_COMBO_BOX_TEXT(combobar), NULL, items[i]);
                free(items[i]);
        }

        free(items);
}

void populate_controller_debug(GObject *grid)
{

        gtk_grid_set_column_spacing(GTK_GRID(grid), 2);
        char *names[] = {"A", "B", "X", "Y", "R1", "R3", "L1", "L3", "START",
                        "SELECT", "XBOX", "UP", "DOWN", "LEFT", "RIGHT"};

        for (int i = 0; i < 15; i++) {
                GtkWidget *frame = gtk_frame_new(NULL);
                GtkWidget *label = gtk_label_new(names[i]);
                gtk_grid_attach(GTK_GRID(grid), label, i % 8, 2 * (i / 8), 1, 1);
                gtk_widget_show(label);
                gtk_grid_attach(GTK_GRID(grid), frame, i % 8, 1 + 2 * (i / 8), 1, 1);
                gtk_widget_show(frame);

                button_bar[i] = gtk_label_new("             ");
                gtk_container_add(GTK_CONTAINER(frame), button_bar[i]);
                gtk_widget_show(button_bar[i]);
        }
}

void populate_controller_axis_debug(GObject *grid)
{
        gtk_grid_set_column_spacing(GTK_GRID(grid), 2);
        char *names[] = {"L2", "R2", "LY", "LX", "RY", "RX", "D_Y", "D_X"};

        for (int i = 0; i < AXIS_NO_SHOW; i++) {
                GtkWidget *frame = gtk_frame_new(NULL);
                GtkWidget *label = gtk_label_new(names[i]);
                gtk_grid_attach(GTK_GRID(grid), label, i, 0, 1, 1);
                gtk_widget_show(label);
                gtk_grid_attach(GTK_GRID(grid), frame, i, 1, 1, 1);
                gtk_widget_show(frame);

                axis_bar[i] = gtk_label_new("             ");
                gtk_container_add(GTK_CONTAINER(frame), axis_bar[i]);
                gtk_widget_show(axis_bar[i]);
        }
}

void populate_serial_debug(GObject *grid)
{
        gtk_grid_set_column_spacing(GTK_GRID(grid), 2);
#ifdef _MY_CONTROLLER_H_
        char *names[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12"};
        int msg_len = MY_MSG_LENGTH;
#else
        char *names[] = {"0", "1", "2", "3", "4", "5", "6", "7"};
        int msg_len = 8;
#endif

        for (int i = 0; i < msg_len; i++) {
                GtkWidget *frame = gtk_frame_new(NULL);
                GtkWidget *label = gtk_label_new(names[i]);
                gtk_grid_attach(GTK_GRID(grid), label, i, 0, 1, 1);
                gtk_widget_show(label);
                gtk_grid_attach(GTK_GRID(grid), frame, i, 1, 1, 1);
                gtk_widget_show(frame);

                status_bar[i] = gtk_label_new("             ");
                gtk_container_add(GTK_CONTAINER(frame), status_bar[i]);
                gtk_widget_show(status_bar[i]);
        }
}

int gui_main(int argc, char* argv[])
{
        GtkBuilder *builder;
        GObject *window;
        GObject *button;
        GObject *grid;
        GObject *menu;

        gtk_init(&argc, &argv);

        // Load the interface from the xml file
        builder = gtk_builder_new();
        gtk_builder_add_from_file(builder, "builder.ui", NULL);

        // Create window and setup destruction
        window = gtk_builder_get_object(builder, "window");
        g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

        /* Menu bar for the joystick and serial port */
        populate_joystick(builder);
        populate_serial(builder);

        // Debug-bar for the controller buttons
        grid = gtk_builder_get_object(builder, "controller_grid");
        populate_controller_debug(grid);
        g_timeout_add(30, (GSourceFunc)update_button, NULL);

        // Debug-bar for the controller axis
        grid = gtk_builder_get_object(builder, "controller_grid_axis");
        populate_controller_axis_debug(grid);
        g_timeout_add(30, (GSourceFunc)update_axis, NULL);

        // Debug-bar for the serial data
        grid = gtk_builder_get_object(builder, "serial_grid");
        populate_serial_debug(grid);
        g_timeout_add(30, (GSourceFunc)update_status, NULL);

        // Get pointers to all connect/disconnect buttons
        joystick_connect_button = GTK_WIDGET(gtk_builder_get_object(builder, "connect_joystick"));
        joystick_disconnect_button = GTK_WIDGET(gtk_builder_get_object(builder, "disconnect_joystick"));
        joystick_refresh_button = GTK_WIDGET(gtk_builder_get_object(builder, "refresh_joystick"));
        serial_connect_button = GTK_WIDGET(gtk_builder_get_object(builder, "connect_serial"));
        serial_disconnect_button = GTK_WIDGET(gtk_builder_get_object(builder, "disconnect_serial"));
        serial_refresh_button = GTK_WIDGET(gtk_builder_get_object(builder, "refresh_serial"));

        // Connect joystick button
        button = G_OBJECT(joystick_connect_button);
        menu = gtk_builder_get_object(builder, "joystick_menu");
        g_signal_connect(button, "clicked", G_CALLBACK(connect_joystick), menu);

        // Disconnect joystick button
        button = G_OBJECT(joystick_disconnect_button);
        g_signal_connect(button, "clicked", G_CALLBACK(disconnect_joystick), NULL);
        gtk_widget_set_sensitive(GTK_WIDGET(button), false);

        // Refresh joystick button
        button = G_OBJECT(joystick_refresh_button);
        g_signal_connect(button, "clicked", G_CALLBACK(refresh_joystick), builder);

        // Connect serial button
        button = G_OBJECT(serial_connect_button);
        menu = gtk_builder_get_object(builder, "serial_menu");
        g_signal_connect(button, "clicked", G_CALLBACK(connect_serial), menu);

        // Disconnect serial button
        button = G_OBJECT(serial_disconnect_button);
        g_signal_connect(button, "clicked", G_CALLBACK(disconnect_serial), NULL);
        gtk_widget_set_sensitive(GTK_WIDGET(button), false);

        // Refresh serial button
        button = G_OBJECT(serial_refresh_button);
        g_signal_connect(button, "clicked", G_CALLBACK(refresh_serial), builder);

        setup_threads();

        gtk_main();

        return 0;
}
