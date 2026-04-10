#include <gtk/gtk.h>
#include <string.h>
#include "Array.h"
#include "file.h"
#include "edit.h"
#include "cursor.h"

char current_file[256] = "";
int file_opened = 0;

static GtkWidget *text;

static void gui_update()
{
    int pos = 0, len = 0;
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text));
    char display[ROW * (COL + 1)];
    
    int max_row = row_pos; 
    for (int i = 0; i < ROW; i++) {
        if (text_buffer[i][0] != '\0') {
            if (i > max_row) {
                max_row = i;
            }
        }
    }

    for(int i = 0; i <= max_row; i++){
        len = strlen(text_buffer[i]);

        if(pos + len + 2 >= sizeof(display))
            break;

        memcpy(&display[pos], text_buffer[i], len);
        pos += len;
        
        if (i < max_row) {
            display[pos] = '\n'; 
            pos++;
        }
    }

    display[pos] = '\0';
    
    gtk_text_buffer_set_text(buffer, display, -1);

    GtkTextIter iter;
    gtk_text_buffer_get_iter_at_line_offset(buffer, &iter, row_pos, col_pos);
    gtk_text_buffer_place_cursor(buffer, &iter);

    GtkTextMark *mark = gtk_text_buffer_get_insert(buffer);
    gtk_text_view_scroll_to_mark(GTK_TEXT_VIEW(text), mark, 0.0, FALSE, 0.0, 0.0);
}

static void warning(GtkWindow *parent)
{
    GtkAlertDialog *dialog = gtk_alert_dialog_new("Tidak ada FIle aktif");

    gtk_alert_dialog_set_detail(dialog, "Gunakan Fitur New untuk mulai menulis");

    gtk_alert_dialog_show(dialog, parent); 
}

static void action_new(GSimpleAction *action, GVariant *parameter, gpointer data)
{
    create_file(text_buffer);
    
    row_pos = 0;
    col_pos = 0;
    
    current_file[0] = '\0';
    file_opened = 1;

    gui_update();
    gtk_widget_grab_focus(text);
}

static void open_response(GObject *source, GAsyncResult *res, gpointer data)
{
    GtkFileDialog *dialog = GTK_FILE_DIALOG(source);
    GFile *file = gtk_file_dialog_open_finish(dialog, res, NULL);

    if (file != NULL) {
        char *path = g_file_get_path(file);

        open_file(path, text_buffer);

        strcpy(current_file, path);
        file_opened = 1;

        gui_update();

        gtk_widget_grab_focus(text);

        g_free(path);
        g_object_unref(file);
    }
}

static void action_open(GSimpleAction *action, GVariant *parameter, gpointer data)
{
    GtkFileDialog *dialog = gtk_file_dialog_new();

    gtk_file_dialog_open(dialog, NULL, NULL, open_response, NULL);
}

static void save_as_response(GObject *source, GAsyncResult *res, gpointer data)
{
    GtkFileDialog *dialog = GTK_FILE_DIALOG(source);
    GFile *file = gtk_file_dialog_save_finish(dialog, res, NULL);

    if (file != NULL) {
        char *path = g_file_get_path(file);

        save_as_file(path, text_buffer);

        strcpy(current_file, path);
        file_opened = 1;

        g_free(path);
        g_object_unref(file);
    }
}

static void action_save_as(GSimpleAction *action, GVariant *parameter, gpointer data)
{
    GtkFileDialog *dialog = gtk_file_dialog_new();

    gtk_file_dialog_save(dialog, NULL, NULL, save_as_response, NULL);
}

static void action_close(GSimpleAction *action, GVariant *parameter, gpointer data)
{
    create_file(text_buffer);

    row_pos = 0;
    col_pos = 0;

    current_file[0] = '\0';
    file_opened = 0;

    gui_update();

    gtk_widget_grab_focus(text);
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
                            gpointer data)
{
    int alert = 0;
    int state = 0;
    if((state & GDK_CONTROL_MASK) &&
       (keyval == GDK_KEY_c || keyval == GDK_KEY_C)){
        copy_selected_text(GTK_TEXT_VIEW(text));
        return TRUE;
    }

    if((state & GDK_CONTROL_MASK) &&
       (keyval == GDK_KEY_v || keyval == GDK_KEY_V)){
        paste_clipboard_text(text_buffer, &row_pos, &col_pos);
        gui_update();
       }
   
    printf("file_opened = %d\n", file_opened);
    if (!file_opened) {
        if(!alert){
            warning(NULL);
            alert = 1;
        }
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

    else if(keyval == GDK_KEY_Left){
        cursor_move_left(text_buffer, &row_pos, &col_pos);
    }

    else if(keyval == GDK_KEY_Right){
        cursor_move_right(text_buffer, &row_pos, &col_pos);
    }

    else if(keyval == GDK_KEY_Up){
        cursor_move_up(text_buffer, &row_pos, &col_pos);
    }

    else if(keyval == GDK_KEY_Down){
        cursor_move_down(text_buffer, &row_pos, &col_pos);
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

static void mouse_clicked(GtkGestureClick *gesture, int n_press, double x, double y, gpointer data)
{
    GtkTextIter iter;
    int buffer_x, buffer_y;

    gtk_text_view_window_to_buffer_coords(GTK_TEXT_VIEW(text), GTK_TEXT_WINDOW_WIDGET, (int)x, (int)y, &buffer_x, &buffer_y);

    gtk_text_view_get_iter_at_location(GTK_TEXT_VIEW(text), &iter, buffer_x, buffer_y);

    int target_row = gtk_text_iter_get_line(&iter);
    int target_col = gtk_text_iter_get_line_offset(&iter);

    set_cursor_position(text_buffer, &row_pos, &col_pos, target_row, target_col);
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
    g_menu_append(file_menu, "Close File", "app.close");
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
    GtkWidget *scroll;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Text Editor. By : Sendal Jepit Team");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_window_set_child(GTK_WINDOW(window), box);

    menu = createMenuBar();
    gtk_box_append(GTK_BOX(box), menu);

    scroll = gtk_scrolled_window_new();
    text = gtk_text_view_new();

    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text), GTK_WRAP_WORD_CHAR);
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scroll), text);
    gtk_widget_set_hexpand(scroll, TRUE);
    gtk_widget_set_vexpand(scroll, TRUE);

    gtk_box_append(GTK_BOX(box), scroll);
    
    GtkEventController *controller = gtk_event_controller_key_new();
    g_signal_connect(controller, "key-pressed", G_CALLBACK(key_pressed), NULL);
    GtkGesture *click_controller = gtk_gesture_click_new();
    g_signal_connect(click_controller, "pressed", G_CALLBACK(mouse_clicked), NULL);

    gtk_widget_add_controller(text, controller);
    gtk_widget_add_controller(text, GTK_EVENT_CONTROLLER(click_controller));

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

    GSimpleAction *open_action = g_simple_action_new("open", NULL);
    g_signal_connect(open_action, "activate", G_CALLBACK(action_open), NULL);
    g_action_map_add_action(G_ACTION_MAP(app), G_ACTION(open_action));

    GSimpleAction *save_as_action = g_simple_action_new("save_as", NULL);
    g_signal_connect(save_as_action, "activate", G_CALLBACK(action_save_as), NULL);
    g_action_map_add_action(G_ACTION_MAP(app), G_ACTION(save_as_action));

    GSimpleAction *close_action = g_simple_action_new("close", NULL);
    g_signal_connect(close_action, "activate", G_CALLBACK(action_close), NULL);
    g_action_map_add_action(G_ACTION_MAP(app), G_ACTION(close_action));
}