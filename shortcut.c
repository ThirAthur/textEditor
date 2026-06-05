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
                GDK_KEY_s,          // s 
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

        gtk_shortcut_controller_add_shortcut(controller, 
        gtk_shortcut_new (
            gtk_keyval_trigger_new(
                GDK_KEY_s,          // s
                GDK_CONTROL_MASK    // ctrl
            ), gtk_named_action_new(
                "app.save"
            )
        )
    );

    gtk_shortcut_controller_add_shortcut(controller, 
        gtk_shortcut_new (
            gtk_keyval_trigger_new(
                GDK_KEY_c,          // c
                GDK_CONTROL_MASK    // ctrl
            ), gtk_named_action_new(
                "app.copy"
            )
        )
    );

    gtk_shortcut_controller_add_shortcut(controller, 
        gtk_shortcut_new (
            gtk_keyval_trigger_new(
                GDK_KEY_v,          // v
                GDK_CONTROL_MASK    // ctrl
            ), gtk_named_action_new(
                "app.paste"
            )
        )
    );
    

    gtk_shortcut_controller_add_shortcut(controller, 
        gtk_shortcut_new (
            gtk_keyval_trigger_new(
                GDK_KEY_z,          // z
                GDK_CONTROL_MASK    // ctrl
            ), gtk_named_action_new(
                "app.undo"
            )
        )
    );
    
   
    gtk_shortcut_controller_add_shortcut(controller, 
        gtk_shortcut_new (
            gtk_keyval_trigger_new(
                GDK_KEY_y,          // y
                GDK_CONTROL_MASK    // ctrl
            ), gtk_named_action_new(
                "app.redo"
            )
        )
    );


    gtk_shortcut_controller_add_shortcut(controller, 
        gtk_shortcut_new (
            gtk_keyval_trigger_new(
                GDK_KEY_f,          // f
                GDK_CONTROL_MASK    // ctrl
            ), gtk_named_action_new(
                "app.find"
            )
        )
    );


    gtk_shortcut_controller_add_shortcut(controller, 
        gtk_shortcut_new (
            gtk_keyval_trigger_new(
                GDK_KEY_h,          // h
                GDK_CONTROL_MASK    // ctrl
            ), gtk_named_action_new(
                "app.replace"
            )
        )
    );
        gtk_widget_add_controller(
        window,
        GTK_EVENT_CONTROLLER(controller)
    );
}
