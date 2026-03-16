#include <gtk/gtk.h>
#include "Array.h"
#include "array.c" //tidak bisa hanya Array.h masalah pada enviorment

//penerapan array untuk menyimpan teks yang diinputkan (dalam uji coba sementara)
char text_buffer[ROW][COL] = {0};
int row_pos = 0;
int col_pos = 0;

static gboolean key_pressed(GtkEventControllerKey *controller,
                            guint keyval,
                            guint keycode,
                            GdkModifierType state,
                            gpointer data)
{
    if(keyval >= 32 && keyval <= 126){
        insert_char(text_buffer, &row_pos, &col_pos, (char)keyval);
    }

    if(keyval == GDK_KEY_Return){
        insert_char(text_buffer, &row_pos, &col_pos, '\n');
    }

    return FALSE;
}

// Menu setting
GMenu *createFileMenu()
{
    GMenu *file_menu;

    file_menu = g_menu_new();

    g_menu_append(file_menu, "New", "app.new");
    g_menu_append(file_menu, "Open", "app.open");
    g_menu_append(file_menu, "Save", "app.save");
    g_menu_append(file_menu, "Save As", "app.save_as");
    g_menu_append(file_menu, "Exit", "app.exit");

    return file_menu;
}

GMenu *createEditMenu()
{
    GMenu *edit_menu;

    edit_menu = g_menu_new();

    g_menu_append(edit_menu, "Undo", "app.undo");
    g_menu_append(edit_menu, "Redo", "app.redo");
    g_menu_append(edit_menu, "Copy", "app.copy");
    g_menu_append(edit_menu, "Paste", "app.paste");

    return edit_menu;
}

GtkWidget *createMenuBar()
{
    GMenu *menubar;
    GtkWidget *menu_widget;

    menubar = g_menu_new();

    g_menu_append_submenu(menubar, "File", G_MENU_MODEL(createFileMenu()));
    g_menu_append_submenu(menubar, "Edit", G_MENU_MODEL(createEditMenu()));

    menu_widget = gtk_popover_menu_bar_new_from_model(G_MENU_MODEL(menubar));

    return menu_widget;
}

// Aktivasi dan memunculkan window
static void activate(GtkApplication *app, gpointer user_data)
{
    GtkWidget *window;
    GtkWidget *text;
    GtkWidget *box;
    GtkWidget *menu;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "TeDit");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_window_set_child(GTK_WINDOW(window), box);

    menu = createMenuBar();
    gtk_box_append(GTK_BOX(box), menu);

    text = gtk_text_view_new();
    gtk_box_append(GTK_BOX(box), text);

    GtkEventController *controller = gtk_event_controller_key_new();
    g_signal_connect(controller, "key-pressed", G_CALLBACK(key_pressed), NULL);

    gtk_widget_add_controller(text, controller);

    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv)
{
    GtkApplication *app;
    int status;

    app = gtk_application_new("org.tedit.example",
                              G_APPLICATION_DEFAULT_FLAGS);

    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    status = g_application_run(G_APPLICATION(app), argc, argv);

    g_object_unref(app);

    return status;
}