#include "shortcut.h"

void setup_shortcuts(GtkWidget *window){

    GtkShortcutController *controller;

    controller = GTK_SHORTCUT_CONTROLLER(
        gtk_shortcut_controller_new()
    );

    gtk_shortcut_controller_add_shortcut(controller, 
        gtk_shortcut_new (
            gtk_keyval_trigger_new(
                GDK_KEY_n,          // n 
                GDK_CONTROL_MASK    // ctrl
            ), gtk_named_action_new(
                "app.new"
            )
        )
    );

    gtk_shortcut_controller_add_shortcut(controller, 
        gtk_shortcut_new (
            gtk_keyval_trigger_new(
                GDK_KEY_o,          // o
                GDK_CONTROL_MASK    // ctrl
            ), gtk_named_action_new(
                "app.open"
            )
        )
    );

    gtk_shortcut_controller_add_shortcut(controller, 
        gtk_shortcut_new (
            gtk_keyval_trigger_new(
                GDK_KEY_n,          // n 
                GDK_CONTROL_MASK | GDK_SHIFT_MASK   // ctrl + shift
                
            ), gtk_named_action_new(
                "app.save_as"
            )
        )
    );

    gtk_shortcut_controller_add_shortcut(controller, 
        gtk_shortcut_new (
            gtk_keyval_trigger_new(
                GDK_KEY_w,          // w
                GDK_CONTROL_MASK    // ctrl
            ), gtk_named_action_new(
                "app.close"
            )
        )
    );

        gtk_widget_add_controller(
        window,
        GTK_EVENT_CONTROLLER(controller)
    );
}
