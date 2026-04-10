#include <gtk/gtk.h>
#include "Array.h"
#include <string.h>
#include "file.h"
#include "edit.h"

char current_file[256] = "";
int file_opened = 0;

static GtkWidget *text;

static void gui_update()
{
    int pos, len;

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text));
    char display[ROW * (COL + 1)];
    pos = 0;

    for(int i = 0; i < ROW; i++){
        len = strlen(text_buffer[i]);

        if(pos + len + 2 >= sizeof(display))
            break;

        memcpy(&display[pos], text_buffer[i], len);
        pos += len;

        if(i < ROW - 1 && text_buffer[i + 1][0] != '\0'){
            display[pos++] = '\n';
        }
    }

    display[pos] = '\0';
    gtk_text_buffer_set_text(buffer, display, -1);

}

static void action_new(GSimpleAction *action, GVariant *parameter, gpointer data)
{
    create_file(text_buffer);

    row_pos = 0;
    col_pos = 0;

    current_file[0] = '\0';
    file_opened = 0;

    gui_update();
}

static void action_copy(GSimpleAction *action, GVariant *parameter, gpointer data)
{
    copy_selected_text(GTK_TEXT_VIEW(text));
}

/* TAMBAHAN: fungsi paste */
static void action_paste(GSimpleAction *action, GVariant *parameter, gpointer data)
{
    paste_clipboard_text(text_buffer, &row_pos, &col_pos);
    gui_update();
}

//penerapan array untuk menyimpan teks yang diinputkan (dalam uji coba sementara)
static gboolean key_pressed(GtkEventControllerKey *controller,
                            guint keyval,
                            guint keycode,
                            GdkModifierType state,
                            gpointer data)
{
    if((state & GDK_CONTROL_MASK) &&
       (keyval == GDK_KEY_c || keyval == GDK_KEY_C)){
        copy_selected_text(GTK_TEXT_VIEW(text));
        return TRUE;
    }

    if((state & GDK_CONTROL_MASK) &&
       (keyval == GDK_KEY_v || keyval == GDK_KEY_V)){
        paste_clipboard_text(text_buffer, &row_pos, &col_pos);
        gui_update();
        return TRUE;
    }

    if(keyval == GDK_KEY_BackSpace){
        delete_char(text_buffer, &row_pos, &col_pos);
    }

    else if(keyval == GDK_KEY_Tab){
        indention(text_buffer, &row_pos, &col_pos);
    }

    else if(keyval == GDK_KEY_Return){
        new_line(text_buffer, &row_pos, &col_pos);
    } 

    else if(keyval >= 32 && keyval <= 126){
        insert_char(text_buffer, &row_pos, &col_pos, (char)keyval);
    }

    else{
        return FALSE;
    }

    array_checker(text_buffer, &row_pos, &col_pos);
    gui_update();

    return TRUE;
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
void activate(GtkApplication *app, gpointer user_data)
{
    GtkWidget *window;
    GtkWidget *box;
    GtkWidget *menu;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "TeDit");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_window_set_child(GTK_WINDOW(window), box);

    menu = createMenuBar();
    gtk_box_append(GTK_BOX(box), menu);

    text = gtk_text_view_new();
    gtk_box_append(GTK_BOX(box), text);
    gtk_widget_set_hexpand(text, TRUE);
    gtk_widget_set_vexpand(text, TRUE);

    GtkEventController *controller = gtk_event_controller_key_new();
    g_signal_connect(controller, "key-pressed", G_CALLBACK(key_pressed), NULL);

    gtk_widget_add_controller(text, controller);

    gui_update();
    gtk_window_present(GTK_WINDOW(window));

    GSimpleAction *new_action = g_simple_action_new("new", NULL);
    g_signal_connect(new_action, "activate", G_CALLBACK(action_new), NULL);
    g_action_map_add_action(G_ACTION_MAP(app), G_ACTION(new_action));

    GSimpleAction *copy_action = g_simple_action_new("copy", NULL);
    g_signal_connect(copy_action, "activate", G_CALLBACK(action_copy), NULL);
    g_action_map_add_action(G_ACTION_MAP(app), G_ACTION(copy_action));

    GSimpleAction *paste_action = g_simple_action_new("paste", NULL);
    g_signal_connect(paste_action, "activate", G_CALLBACK(action_paste), NULL);
    g_action_map_add_action(G_ACTION_MAP(app), G_ACTION(paste_action));
}