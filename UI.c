#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>
#include "Fathir.h"
#include "shortcut.h"
#include "deva.h"
#include "shortcut.h"
#include "cursor.h"

// Variabel Global Baru untuk Linked List
List text_list;
address poscursor;
int col_pos = 0;

char current_file[256] = "";
int file_opened = 0;

static GtkWidget *text_view;

// Helper: Mencari baris ke-berapa kursor berada sekarang untuk keperluan visualisasi GTK
int get_current_row_index() {
    int row = 0;
    address P = First(text_list);
    while (P != NULL && P != poscursor) {
        row++;
        P = P->next;
    }
    return row;
}

// Inisialisasi awal list kosong
void init_editor() {
    CreateList(&text_list);
    char empty_line[clmn] = "";
    InsChFirst(&text_list, empty_line);
    poscursor = First(text_list);
    col_pos = 0;
}

static void gui_update()
{
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    
    // Menggunakan GString bawaan GLib untuk menggabungkan teks dari Linked List
    GString *display_str = g_string_new("");
    address P = First(text_list);
    
    while (P != NULL) {
        g_string_append(display_str, P->info);
        if (P->next != NULL) {
            g_string_append_c(display_str, '\n');
        }
        P = P->next;
    }
    
    gtk_text_buffer_set_text(buffer, display_str->str, -1);
    g_string_free(display_str, TRUE);

    // Set kursor visual GTK
    int current_row = get_current_row_index();
    GtkTextIter iter;
    gtk_text_buffer_get_iter_at_line_offset(buffer, &iter, current_row, col_pos);
    gtk_text_buffer_place_cursor(buffer, &iter);

    GtkTextMark *mark = gtk_text_buffer_get_insert(buffer);
    gtk_text_view_scroll_to_mark(GTK_TEXT_VIEW(text_view), mark, 0.0, FALSE, 0.0, 0.0);
}

static void warning(GtkWindow *parent)
{
    GtkAlertDialog *dialog = gtk_alert_dialog_new("Tidak ada File aktif");
    gtk_alert_dialog_set_detail(dialog, "Gunakan Fitur New untuk mulai menulis");
    gtk_alert_dialog_show(dialog, parent); 
}

static void action_new(GSimpleAction *action, GVariant *parameter, gpointer data)
{
    create_file(&Lisi, &Lnama);       
    memset(text_buffer, 0, sizeof(text_buffer));
    row_pos = 0;
    col_pos = 0;
    file_opened = 1;
    current_file[0] = '\0';
    gui_update();
    gtk_widget_grab_focus(text);
}

static void open_response(GObject *source, GAsyncResult *res, gpointer data)
{
    GtkFileDialog *dialog = GTK_FILE_DIALOG(source);
    GFile *file = gtk_file_dialog_open_finish(dialog, res, NULL);

    if (file != NULL) {
        char *path = g_file_get_path(file);

        open_file(&Lisi, &Lnama, path);      
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
    if (file_opened == 1) {

        close_file(&Lisi, &Lnama);
        memset(text_buffer, 0, sizeof(text_buffer));
        file_opened = 0;
    }

    GtkFileDialog *dialog = gtk_file_dialog_new();
    gtk_file_dialog_open(dialog, NULL, NULL, open_response, NULL);
    g_object_unref(dialog);
}

static void save_as_response(GObject *source, GAsyncResult *res, gpointer data)
{
    GtkFileDialog *dialog = GTK_FILE_DIALOG(source);
    GFile *file = gtk_file_dialog_save_finish(dialog, res, NULL);

    if (file != NULL) {
        char *path = g_file_get_path(file);

        save_as_file(&Lisi, &Lnama, path);        
        strcpy(current_file, path);
        file_opened = 1;

        g_free(path);
        g_object_unref(file);
    }
}

static void action_save(GSimpleAction *action, GVariant *parameter, gpointer data)
{
    if (!file_opened) {
        warning(NULL);
        return;
    }

    /* Jika file sudah punya nama/path, langsung save */
    if (current_file[0] != '\0') {
        save_file(current_file, text_buffer);
        gtk_widget_grab_focus(text);
        return;
    }
    /* Jika file baru dan belum punya nama, arahkan ke Save As */
    GtkFileDialog *dialog = gtk_file_dialog_new();
    gtk_file_dialog_save(dialog, NULL, NULL, save_as_response, NULL);
}

static void action_save_as(GSimpleAction *action, GVariant *parameter, gpointer data)
{
    if (!file_opened) {
        warning(GTK_WINDOW(gtk_widget_get_root(text)));
        return;
    }

    GtkFileDialog *dialog = gtk_file_dialog_new();
    gtk_file_dialog_save(dialog, NULL, NULL, save_as_response, NULL);
    g_object_unref(dialog);
}

static void action_close(GSimpleAction *action, GVariant *parameter, gpointer data)
{
    if (!file_opened) return;

    close_file(&Lisi, &Lnama);             
    memset(text_buffer, 0, sizeof(text_buffer));
    row_pos = 0;
    col_pos = 0;
    current_file[0] = '\0';
    file_opened = 0;

    gui_update();
    gtk_widget_grab_focus(text);
}

static void action_copy(GSimpleAction *action, GVariant *parameter, gpointer data)
{
    // TODO: Sesuaikan dengan edit.c versi Linked List
    g_print("Fitur Copy belum dihubungkan dengan Linked List.\n");
}

static void action_paste(GSimpleAction *action, GVariant *parameter, gpointer data)
{
    // TODO: Sesuaikan dengan edit.c versi Linked List
    g_print("Fitur Paste belum dihubungkan dengan Linked List.\n");
}

static gboolean key_pressed(GtkEventControllerKey *controller,
                            guint keyval,
                            gpointer data)
{
    int alert = 0;
    GdkModifierType state = gtk_event_controller_get_current_event_state(GTK_EVENT_CONTROLLER(controller));
    
    if((state & GDK_CONTROL_MASK) && (keyval == GDK_KEY_c || keyval == GDK_KEY_C)){
        action_copy(NULL, NULL, NULL);
        return TRUE;
    }

    if((state & GDK_CONTROL_MASK) && (keyval == GDK_KEY_v || keyval == GDK_KEY_V)){
        action_paste(NULL, NULL, NULL);
        return TRUE;
    }
   
    if (!file_opened) {
        if(!alert){
            warning(NULL);
            alert = 1;
        }
        return TRUE;
    }

    if(keyval == GDK_KEY_BackSpace){
        delete_char(&text_list, &poscursor, &col_pos);
    }
    else if(keyval == GDK_KEY_Tab){
        indention(&text_list, &poscursor, &col_pos);
    }
    else if(keyval == GDK_KEY_Return){
        new_line(&text_list, &poscursor, &col_pos);
    } 
    else if(keyval == GDK_KEY_Left){
        cursor_move_left(&text_list, &poscursor, &col_pos);
    }
    else if(keyval == GDK_KEY_Right){
        cursor_move_right(&text_list, &poscursor, &col_pos);
    }
    else if(keyval == GDK_KEY_Up){
        cursor_move_up(&text_list, &poscursor, &col_pos);
    }
    else if(keyval == GDK_KEY_Down){
        cursor_move_down(&text_list, &poscursor, &col_pos);
    }
    else if(keyval >= 32 && keyval <= 126){
        insert_char(&text_list, &poscursor, &col_pos, (char)keyval);
    }
    else{
        return FALSE; // Biarkan GTK menghandle shortcut lain
    }

    array_checker(text_list, poscursor);
    gui_update();
    return TRUE;
}

static void mouse_clicked(GtkGestureClick *gesture, int n_press, double x, double y, gpointer data)
{
    GtkTextIter iter;
    int buffer_x, buffer_y;

    gtk_text_view_window_to_buffer_coords(GTK_TEXT_VIEW(text_view), GTK_TEXT_WINDOW_WIDGET, (int)x, (int)y, &buffer_x, &buffer_y);
    gtk_text_view_get_iter_at_location(GTK_TEXT_VIEW(text_view), &iter, buffer_x, buffer_y);

    int target_row = gtk_text_iter_get_line(&iter);
    int target_col = gtk_text_iter_get_line_offset(&iter);

    // Iterasi list untuk mencari baris tujuan klik
    address P = First(text_list);
    int r = 0;
    while (P != NULL && r < target_row) {
        P = P->next;
        r++;
    }

    if (P != NULL) {
        poscursor = P;
        col_pos = target_col;
        if (col_pos > strlen(poscursor->info)) {
            col_pos = strlen(poscursor->info);
        }
    }
    
    gui_update();
}

GMenu *createFileMenu()
{
    GMenu *file_menu = g_menu_new();
    g_menu_append(file_menu, "New", "app.new");
    g_menu_append(file_menu, "Open", "app.open");
    g_menu_append(file_menu, "Save", "app.save");
    g_menu_append(file_menu, "Save As", "app.save_as");
    g_menu_append(file_menu, "Close File", "app.close");
    g_menu_append(file_menu, "Exit", "app.exit"); // Pastikan action exit dikelola di main
    return file_menu;
}

GMenu *createEditMenu()
{
    GMenu *edit_menu = g_menu_new();
    g_menu_append(edit_menu, "Undo", "app.undo");
    g_menu_append(edit_menu, "Redo", "app.redo");
    g_menu_append(edit_menu, "Copy", "app.copy");
    g_menu_append(edit_menu, "Paste", "app.paste");
    return edit_menu;
}

GtkWidget *createMenuBar()
{
    GMenu *menubar = g_menu_new();
    g_menu_append_submenu(menubar, "File", G_MENU_MODEL(createFileMenu()));
    g_menu_append_submenu(menubar, "Edit", G_MENU_MODEL(createEditMenu()));
    return gtk_popover_menu_bar_new_from_model(G_MENU_MODEL(menubar));
}

void activate(GtkApplication *app, gpointer user_data)
{
    // Inisiasi data editor pertama kali
    init_editor();

    GtkWidget *window;
    GtkWidget *box;
    GtkWidget *menu;
    GtkWidget *scroll;

    CreateList(&Lisi);
    CreateList(&Lnama);

    window = gtk_application_window_new(app);
    setup_shortcuts(window); 
    gtk_window_set_title(GTK_WINDOW(window), "Text Editor. By : Sendal Jepit Team");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_window_set_child(GTK_WINDOW(window), box);

    menu = createMenuBar();
    gtk_box_append(GTK_BOX(box), menu);

    scroll = gtk_scrolled_window_new();
    text_view = gtk_text_view_new();

    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scroll), text_view);
    gtk_widget_set_halign(scroll, GTK_ALIGN_CENTER);
    gtk_widget_set_size_request(scroll, 666, -1);

    gtk_widget_set_hexpand(scroll, FALSE);
    gtk_widget_set_vexpand(scroll, TRUE);
    gtk_widget_set_margin_start(text_view, 10);
    gtk_widget_set_margin_end(text_view, 10);
    gtk_box_append(GTK_BOX(box), scroll);
    
    GtkEventController *controller = gtk_event_controller_key_new();
    g_signal_connect(controller, "key-pressed", G_CALLBACK(key_pressed), NULL);
    
    GtkGesture *click_controller = gtk_gesture_click_new();
    g_signal_connect(click_controller, "pressed", G_CALLBACK(mouse_clicked), NULL);

    gtk_widget_add_controller(text_view, controller);
    gtk_widget_add_controller(text_view, GTK_EVENT_CONTROLLER(click_controller));

    gui_update();
    gtk_window_present(GTK_WINDOW(window));

    GSimpleAction *new_action = g_simple_action_new("new", NULL);       // buat action new
    g_signal_connect(                                                   // jika new_action terpanggil, maka panggil callback (hanya menghubungkan)
        new_action,                                                     
        "activate", 
        G_CALLBACK(action_new), 
        NULL
    );
    g_action_map_add_action(G_ACTION_MAP(app), G_ACTION(new_action));       // mapping sehingga app.new dikenali oleh 
                                                                            // shortcut dan gmenu

    GSimpleAction *copy_action = g_simple_action_new("copy", NULL);
    g_signal_connect(copy_action, "activate", G_CALLBACK(action_copy), NULL);
    g_action_map_add_action(G_ACTION_MAP(app), G_ACTION(copy_action));

    GSimpleAction *paste_action = g_simple_action_new("paste", NULL);
    g_signal_connect(paste_action, "activate", G_CALLBACK(action_paste), NULL);
    g_action_map_add_action(G_ACTION_MAP(app), G_ACTION(paste_action));

    GSimpleAction *open_action = g_simple_action_new("open", NULL);
    g_signal_connect(open_action, "activate", G_CALLBACK(action_open), NULL);
    g_action_map_add_action(G_ACTION_MAP(app), G_ACTION(open_action));

    GSimpleAction *save_action = g_simple_action_new("save", NULL);
    g_signal_connect(save_action, "activate", G_CALLBACK(action_save), NULL);
    g_action_map_add_action(G_ACTION_MAP(app), G_ACTION(save_action));
    
    GSimpleAction *save_as_action = g_simple_action_new("save_as", NULL);           
    g_signal_connect(save_as_action, "activate", G_CALLBACK(action_save_as), NULL);
    g_action_map_add_action(G_ACTION_MAP(app), G_ACTION(save_as_action));               // app.save_as

    GSimpleAction *close_action = g_simple_action_new("close", NULL);
    g_signal_connect(close_action, "activate", G_CALLBACK(action_close), NULL);
    g_action_map_add_action(G_ACTION_MAP(app), G_ACTION(close_action));
}