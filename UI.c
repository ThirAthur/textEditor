#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>
#include "Fathir.h"
#include "file.h"
#include "shortcut.h"
#include "cursor.h"
#include "edit.h"

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
    edit_init_history();
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
    gtk_text_view_scroll_mark_onscreen(GTK_TEXT_VIEW(text_view), mark);
}

static void warning(GtkWindow *parent)
{
    GtkAlertDialog *dialog = gtk_alert_dialog_new("Tidak ada File aktif");
    
    
    gtk_alert_dialog_set_detail(dialog, "Gunakan Fitur New untuk mulai menulis");
    gtk_alert_dialog_show(dialog, parent); 
}

static void action_new(GSimpleAction *action, GVariant *parameter, gpointer data)
{
    create_file(&text_list);            // Reset List
    char empty_line[clmn] = "";
    InsChFirst(&text_list, empty_line);
    poscursor = First(text_list);
    col_pos = 0;
    
    file_opened = 1;

    edit_clear_history();

    gui_update();
    gtk_widget_grab_focus(text_view);
}

static void open_response (GObject *source_object, GAsyncResult *res, gpointer data){
    GtkFileDialog *dialog = GTK_FILE_DIALOG(source_object);
    GError *error = NULL;
    GFile *file = gtk_file_dialog_open_finish(dialog, res, &error);

    if (file != NULL){
        char *path = g_file_get_path(file);
        if (path != NULL){
            strcpy(current_file, path);
            file_opened = 1;

            open_file(&text_list, path);

            poscursor = First(text_list);
            col_pos = 0;

            edit_clear_history();

            gui_update();
            g_free(path);
        }
        g_object_unref(file);
    }

}

static void action_open(GSimpleAction *action, GVariant *parameter, gpointer data)
{
    GtkFileDialog *dialog = gtk_file_dialog_new();

    GtkFileFilter *filter = gtk_file_filter_new();
    gtk_file_filter_set_name(filter, "Txt Files");
    gtk_file_filter_add_pattern(filter, "*.txt");

    GListStore *filters = g_list_store_new(GTK_TYPE_FILE_FILTER);
    g_list_store_append(filters, filter);

    gtk_file_dialog_set_filters(dialog, G_LIST_MODEL(filters));
    gtk_file_dialog_set_default_filter(dialog, filter);

    g_object_unref(filters);
    g_object_unref(filter);
   
    gtk_file_dialog_open(dialog, NULL, NULL, open_response, NULL);
}

static void save_as_response (GObject *source_object, GAsyncResult *res, gpointer data)
{
    GtkFileDialog *dialog = GTK_FILE_DIALOG(source_object);
    GError *error = NULL;
    GFile *file = gtk_file_dialog_save_finish(dialog, res, &error);

    if (file != NULL){
        char *path = g_file_get_path(file);
        if (path != NULL){
            strcpy(current_file, path);
            file_opened = 1;

            save_as_file(&text_list, path);

            g_free(path);
        }
        g_object_unref(file);
    }
}

static void action_save_as(GSimpleAction *action, GVariant *parameter, gpointer data)
{
    // TODO: Sesuaikan dengan file.c versi Linked List

    GtkFileDialog *dialog = gtk_file_dialog_new();

    GtkFileFilter *filter = gtk_file_filter_new();
    gtk_file_filter_set_name(filter, "Txt Files");
    gtk_file_filter_add_pattern(filter, "*.txt");

    GListStore *filters = g_list_store_new(GTK_TYPE_FILE_FILTER);
    g_list_store_append(filters, filter);

    gtk_file_dialog_set_filters(dialog, G_LIST_MODEL(filters));
    gtk_file_dialog_set_default_filter(dialog, filter);

    g_object_unref(filters);
    g_object_unref(filter);

   
    gtk_file_dialog_save(dialog, NULL, NULL, save_as_response, NULL);

}

static void action_save(GSimpleAction *action, GVariant *parameter, gpointer data)
{
    if (!file_opened) {
        warning(NULL);
        return;
    }

    if (current_file[0] == '\0') {
        action_save_as(action, parameter, data);
        return;
    }

    save_file(&text_list, current_file);

    g_print("File berhasil disimpan: %s\n", current_file);
}

static void action_close(GSimpleAction *action, GVariant *parameter, gpointer data)
{
    create_file(&text_list); 
    current_file[0] = 0;
    char empty_line[clmn] = "";
    InsChFirst(&text_list, empty_line);
    poscursor = First(text_list);
    col_pos = 0;

    edit_clear_history();

    file_opened = 0;

    gui_update();
    gtk_widget_grab_focus(text_view);
    
}

static void action_copy(GSimpleAction *action, GVariant *parameter, gpointer data)
{
    edit_end_session();

    if (!file_opened) {
        warning(NULL);
        return;
    }

    copy_selected_text(GTK_TEXT_VIEW(text_view));
    gtk_widget_grab_focus(text_view);}

static void action_paste(GSimpleAction *action, GVariant *parameter, gpointer data)
{
    edit_end_session();

    if (!file_opened) {
        warning(NULL);
        return;
    }

    if (internal_clipboard[0] == '\0') {
        return;
    }

    edit_begin_single_action(&text_list, poscursor, col_pos);
    paste_clipboard_text(&text_list, &poscursor, &col_pos);

    gui_update();
    gtk_widget_grab_focus(text_view);
}

static void action_undo(GSimpleAction *action, GVariant *parameter, gpointer data)
{
    if (!file_opened) {
        warning(NULL);
        return;
    }

    edit_undo(&text_list, &poscursor, &col_pos);

    gui_update();
    gtk_widget_grab_focus(text_view);
}

static void action_redo(GSimpleAction *action, GVariant *parameter, gpointer data)
{
    if (!file_opened) {
        warning(NULL);
        return;
    }

    edit_redo(&text_list, &poscursor, &col_pos);

    gui_update();
    gtk_widget_grab_focus(text_view);
}

static char last_search_term[256] = "";

static void on_search_button_clicked(GtkWidget *widget, gpointer user_data)
{
    GtkEntry *entry = GTK_ENTRY(user_data);
    const char *search_term = gtk_editable_get_text(GTK_EDITABLE(entry));

    if (strlen(search_term) == 0) return;

    // 1. Deteksi apakah ini kata baru yang dicari
    int is_new_search = (strcmp(search_term, last_search_term) != 0);
    strcpy(last_search_term, search_term);

    // Jika pencarian baru, set kursor ke NULL agar searchLogic milikmu mulai dari First(L)
    if (is_new_search) {
        poscursor = NULL; 
        col_pos = 0;
    } else {
        // Jika Find Next, geser 1 karakter agar tidak mendeteksi kata yang sama di titik yang sama
        col_pos += 1; 
    }

    // 3. Panggil fungsi searchLogic dari array.c
    boolean found = searchLogic(text_list, &poscursor, &col_pos, (char *)search_term);

    if (found) {
        char *match_ptr = strstr(poscursor->info + col_pos, search_term);
        if (match_ptr != NULL) {
            col_pos = match_ptr - poscursor->info; // Dapatkan indeks asli kursor
        }

        gui_update(); 

        // 5. Menerapkan Blok Warna Kuning (Highlight)
        GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
        GtkTextTagTable *tag_table = gtk_text_buffer_get_tag_table(buffer);
        GtkTextTag *tag = gtk_text_tag_table_lookup(tag_table, "highlight");
        
        if (tag == NULL) {
            gtk_text_buffer_create_tag(buffer, "highlight", 
                                       "background", "yellow", 
                                       "foreground", "black", 
                                       NULL);
        }

        int current_row = get_current_row_index();
        GtkTextIter start_iter, end_iter;
        
        gtk_text_buffer_get_iter_at_line_offset(buffer, &start_iter, current_row, col_pos);
        gtk_text_buffer_get_iter_at_line_offset(buffer, &end_iter, current_row, col_pos + strlen(search_term));
        
        gtk_text_buffer_apply_tag_by_name(buffer, "highlight", &start_iter, &end_iter);
        
    } else {
        // 6. Tampilkan peringatan jika tidak ditemukan
        GtkWidget *dialog = gtk_window_new();
        gtk_window_set_title(GTK_WINDOW(dialog), "Warning!");
        gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);
        gtk_window_set_default_size(GTK_WINDOW(dialog), 300, 120);

        GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
        gtk_widget_set_margin_start(box, 15);
        gtk_widget_set_margin_end(box, 15);
        gtk_widget_set_margin_top(box, 15);
        gtk_widget_set_margin_bottom(box, 15);

        gtk_window_set_child(GTK_WINDOW(dialog), box);
        GtkWidget *label = gtk_label_new("Kata tidak ditemukan.");
        gtk_box_append(GTK_BOX(box), label);
        GtkWidget *ok_btn = gtk_button_new_with_label("OK");
        gtk_box_append(GTK_BOX(box), ok_btn);

        g_signal_connect_swapped(ok_btn, "clicked", G_CALLBACK(gtk_window_destroy), dialog);
        gtk_window_present(GTK_WINDOW(dialog));

        last_search_term[0] = '\0'; // Reset state agar bisa dicari ulang
    }
}

static void on_replace_button_clicked(GtkWidget *widget, gpointer user_data)
{
    GtkEntry *replace_entry = GTK_ENTRY(user_data);
    GtkEntry *search_entry = GTK_ENTRY(g_object_get_data(G_OBJECT(replace_entry), "search_entry"));

    const char *search_term = gtk_editable_get_text(GTK_EDITABLE(search_entry));
    const char *replace_term = gtk_editable_get_text(GTK_EDITABLE(replace_entry));

    if (strlen(search_term) == 0) return;

    replaceLogic(&text_list, &poscursor, col_pos, (char*)search_term, (char*)replace_term);

    gui_update();
}

void callsearch_callfind()
{
    GtkWidget *dialog = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(dialog), "Cari Kata");
    gtk_window_set_default_size(GTK_WINDOW(dialog), 300, 120);

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_set_margin_start(box, 15);
    gtk_widget_set_margin_end(box, 15);
    gtk_widget_set_margin_top(box, 15);
    gtk_widget_set_margin_bottom(box, 15);
    gtk_window_set_child(GTK_WINDOW(dialog), box);

    GtkWidget *label = gtk_label_new("Word:");
    gtk_box_append(GTK_BOX(box), label);

    GtkWidget *entry = gtk_entry_new();
    gtk_box_append(GTK_BOX(box), entry);

    GtkWidget *search_btn = gtk_button_new_with_label("Search");
    gtk_box_append(GTK_BOX(box), search_btn);

    GtkWidget *replace_label = gtk_label_new("Replace with:");
    gtk_box_append(GTK_BOX(box), replace_label);

    GtkWidget *replace_entry = gtk_entry_new();
    gtk_box_append(GTK_BOX(box), replace_entry);

    GtkWidget *replace_btn = gtk_button_new_with_label("Replace");
    gtk_box_append(GTK_BOX(box), replace_btn);

    g_object_set_data(G_OBJECT(replace_entry), "search_entry", entry);

    g_signal_connect(search_btn, "clicked", G_CALLBACK(on_search_button_clicked), entry);
    g_signal_connect(entry, "activate", G_CALLBACK(on_search_button_clicked), entry);

    g_signal_connect(replace_btn, "clicked", G_CALLBACK(on_replace_button_clicked), replace_entry);
    g_signal_connect(replace_entry, "activate", G_CALLBACK(on_replace_button_clicked), replace_entry);

    gtk_window_present(GTK_WINDOW(dialog));
}

static void action_find(GSimpleAction *action, GVariant *parameter, gpointer data)
{
    callsearch_callfind();
}

static gboolean key_pressed(GtkEventControllerKey *controller,
                            guint keyval,
                            gpointer data)
{
    GdkModifierType state = gtk_event_controller_get_current_event_state(GTK_EVENT_CONTROLLER(controller));
    GtkWindow *parent_window = GTK_WINDOW(gtk_widget_get_root(text_view));
    


    if ((state & GDK_CONTROL_MASK)) {
        if ((keyval == GDK_KEY_n) || (keyval == GDK_KEY_N)){
            return FALSE;
        } 
        if ((keyval == GDK_KEY_o) || (keyval == GDK_KEY_O)){
            return FALSE;
        }
        if ((keyval == GDK_KEY_s) || (keyval == GDK_KEY_S)){
            if ((state && GDK_SHIFT_MASK) && file_opened == 0){
                warning(parent_window);
            } else if ((state & GDK_SHIFT_MASK) && file_opened == 1) {
                return FALSE;
            } else if (file_opened == 0) {
                warning(parent_window);
            } else {
                return FALSE;
            }
            return TRUE;
        }
        if ((keyval == GDK_KEY_w) || (keyval == GDK_KEY_w)){
            return FALSE;
        }

        if ((keyval == GDK_KEY_y) || ((keyval == GDK_KEY_Y)) ||
            (keyval == GDK_KEY_z) || (keyval == GDK_KEY_Z) || 
            (keyval == GDK_KEY_c) || (keyval == GDK_KEY_C) ||
            (keyval == GDK_KEY_v) || (keyval == GDK_KEY_V) ||
            (keyval == GDK_KEY_f) || (keyval == GDK_KEY_F) ){
                if(!file_opened){
                    warning(parent_window);
                    return TRUE;
                }
                return FALSE;
            }

        return TRUE;
    }

    if (!file_opened) {
        warning(parent_window);
        return TRUE;
    }

    if(keyval == GDK_KEY_BackSpace){
        edit_begin_delete(&text_list, poscursor, col_pos);
        delete_char(&text_list, &poscursor, &col_pos);
    }
    else if(keyval == GDK_KEY_Tab){
        edit_begin_typing(&text_list, poscursor, col_pos);
        indention(&text_list, &poscursor, &col_pos);
    }
    else if(keyval == GDK_KEY_Return){
        edit_begin_typing(&text_list, poscursor, col_pos);
        new_line(&text_list, &poscursor, &col_pos);
    } 
    else if(keyval == GDK_KEY_Left){
        edit_end_session();
        cursor_move_left(&text_list, &poscursor, &col_pos);
    }
    else if(keyval == GDK_KEY_Right){
        edit_end_session();
        cursor_move_right(&text_list, &poscursor, &col_pos);
    }
    else if(keyval == GDK_KEY_Up){
        edit_end_session();
        cursor_move_up(&text_list, &poscursor, &col_pos);
    }
    else if(keyval == GDK_KEY_Down){
        edit_end_session();
        cursor_move_down(&text_list, &poscursor, &col_pos);
    }
    else if(keyval >= 32 && keyval <= 126){
        edit_begin_typing(&text_list, poscursor, col_pos);
        insert_char(&text_list, &poscursor, &col_pos, (char)keyval);
    }
    else{
        return FALSE; // Biarkan GTK menghandle shortcut lain
    }

    gui_update();
    return TRUE;
   
    
}

static void mouse_clicked(GtkGestureClick *gesture, int n_press, double x, double y, gpointer data)
{
    edit_end_session();

    GtkTextIter iter;
    int buffer_x, buffer_y;

    gtk_text_view_window_to_buffer_coords(GTK_TEXT_VIEW(text_view), GTK_TEXT_WINDOW_WIDGET, (int)x, (int)y, &buffer_x, &buffer_y);
    gtk_text_view_get_iter_at_location(GTK_TEXT_VIEW(text_view), &iter, buffer_x, buffer_y);

    int target_row = gtk_text_iter_get_line(&iter);
    int target_col = gtk_text_iter_get_line_offset(&iter);

    address P = First(text_list);
    address last_valid_P = P; // Menyimpan node terakhir yang valid
    int r = 0;
    
    while (P != NULL && r < target_row) {
        last_valid_P = P; // Selalu simpan node saat ini sebelum maju ke P->next
        P = P->next;
        r++;
    }

    if (P == NULL) {
        P = last_valid_P;
    }

    if (P != NULL) {
        poscursor = P;
        col_pos = target_col;
        
        // Mencegah kursor melebihi panjang teks di baris yang di-klik
        if (col_pos > strlen(poscursor->info)) {
            col_pos = strlen(poscursor->info);
        }
    }
}

static void on_copy_clipboard(GtkTextView *text_view, gpointer data)
{
    action_copy(NULL, NULL, NULL);

    g_signal_stop_emission_by_name(text_view, "copy-clipboard");
}

static void on_paste_clipboard(GtkTextView *text_view, gpointer data)
{
   
    action_paste(NULL, NULL, NULL);

    g_signal_stop_emission_by_name(text_view, "paste-clipboard");
}

static gboolean on_capture_keys(GtkEventControllerKey *controller,
                                guint keyval,
                                guint keycode,
                                GdkModifierType state,
                                gpointer data)
{
    // Jika tidak ada file yang terbuka, biarkan event lolos 
    // agar fungsi key_pressed bawaanmu bisa menampilkan peringatan.
    if (!file_opened) return FALSE;

    // 1. Menangani Shortcut dengan Control (Ctrl+Z / Ctrl+Y)
    if (state & GDK_CONTROL_MASK) {
        if (keyval == GDK_KEY_z || keyval == GDK_KEY_Z) {
            action_undo(NULL, NULL, NULL); 
            return TRUE; // Blokir GTK
        }
        if (keyval == GDK_KEY_y || keyval == GDK_KEY_Y) {
            action_redo(NULL, NULL, NULL); 
            return TRUE; // Blokir GTK
        }
    }

    // 2. Menangani Tombol Navigasi (Home / End)
    if (keyval == GDK_KEY_Home || keyval == GDK_KEY_KP_Home) {
        edit_end_session();
        col_pos = 0; // Ke awal baris
        gui_update();
        return TRUE; 
    }

    if (keyval == GDK_KEY_End || keyval == GDK_KEY_KP_End) {
        edit_end_session();
        if (poscursor != NULL) {
            col_pos = strlen(poscursor->info); // Ke akhir teks di baris ini
        }
        gui_update();
        return TRUE; 
    }

    if (keyval == GDK_KEY_Page_Up || keyval == GDK_KEY_Page_Down) {
        return TRUE; // Blokir Page Up/Down bawaan GTK
    }

    if (keyval == GDK_KEY_Delete || keyval == GDK_KEY_KP_Delete) {
        return TRUE;
    }

    return FALSE; 
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
    g_menu_append(edit_menu, "Find", "app.find");
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

    window = gtk_application_window_new(app);
    setup_shortcuts(window);
    gtk_window_set_title(GTK_WINDOW(window), "Text Editor. By : Sendal Jepit Team");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_window_set_child(GTK_WINDOW(window), box);

    menu = createMenuBar();
    gtk_box_append(GTK_BOX(box), menu);

    scroll = gtk_scrolled_window_new();
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    text_view = gtk_text_view_new();
    gtk_text_view_set_monospace(GTK_TEXT_VIEW(text_view), TRUE);

    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scroll), text_view);
    gtk_widget_set_halign(scroll, GTK_ALIGN_CENTER);
    gtk_widget_set_size_request(scroll, 740, -1);

    gtk_widget_set_hexpand(scroll, FALSE);
    gtk_widget_set_vexpand(scroll, TRUE);
    
    gtk_widget_set_margin_top(scroll, 10);
    gtk_widget_set_margin_bottom(scroll, 10);

    gtk_box_append(GTK_BOX(box), scroll);
    
    GtkEventController *controller = gtk_event_controller_key_new();
    g_signal_connect(controller, "key-pressed", G_CALLBACK(key_pressed), NULL);
    
    GtkGesture *click_controller = gtk_gesture_click_new();
    g_signal_connect(click_controller, "pressed", G_CALLBACK(mouse_clicked), NULL);

    gtk_widget_add_controller(text_view, controller);
    gtk_widget_add_controller(text_view, GTK_EVENT_CONTROLLER(click_controller));

    g_signal_connect(text_view, "copy-clipboard", G_CALLBACK(on_copy_clipboard), NULL);
    g_signal_connect(text_view, "paste-clipboard", G_CALLBACK(on_paste_clipboard), NULL);

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    gtk_text_buffer_set_enable_undo(buffer, FALSE);

    GtkEventController *capture_ctrl = gtk_event_controller_key_new();
    gtk_event_controller_set_propagation_phase(GTK_EVENT_CONTROLLER(capture_ctrl), GTK_PHASE_CAPTURE);
    g_signal_connect(capture_ctrl, "key-pressed", G_CALLBACK(on_capture_keys), NULL);
    gtk_widget_add_controller(text_view, capture_ctrl);

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

    GSimpleAction *undo_action = g_simple_action_new("undo", NULL);
    g_signal_connect(undo_action, "activate", G_CALLBACK(action_undo), NULL);
    g_action_map_add_action(G_ACTION_MAP(app), G_ACTION(undo_action));

    GSimpleAction *redo_action = g_simple_action_new("redo", NULL);
    g_signal_connect(redo_action, "activate", G_CALLBACK(action_redo), NULL);
    g_action_map_add_action(G_ACTION_MAP(app), G_ACTION(redo_action));

    GSimpleAction *open_action = g_simple_action_new("open", NULL);
    g_signal_connect(open_action, "activate", G_CALLBACK(action_open), NULL);
    g_action_map_add_action(G_ACTION_MAP(app), G_ACTION(open_action));

    GSimpleAction *save_action = g_simple_action_new("save", NULL);
    g_signal_connect(save_action, "activate", G_CALLBACK(action_save), NULL);
    g_action_map_add_action(G_ACTION_MAP(app), G_ACTION(save_action));
    
    GSimpleAction *save_as_action = g_simple_action_new("save_as", NULL);
    g_signal_connect(save_as_action, "activate", G_CALLBACK(action_save_as), NULL);
    g_action_map_add_action(G_ACTION_MAP(app), G_ACTION(save_as_action));

    GSimpleAction *find_action = g_simple_action_new("find", NULL);
    g_signal_connect(find_action, "activate", G_CALLBACK(callsearch_callfind), NULL);
    g_action_map_add_action(G_ACTION_MAP(app), G_ACTION(find_action));

    GSimpleAction *close_action = g_simple_action_new("close", NULL);
    g_signal_connect(close_action, "activate", G_CALLBACK(action_close), NULL);
    g_action_map_add_action(G_ACTION_MAP(app), G_ACTION(close_action));
}