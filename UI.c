#include <gtk/gtk.h>

static void activate (GtkApplication *app, gpointer user_data)
{
  GtkWidget *window;
  GtkWidget *text;

  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "TeDit");
  gtk_window_set_default_size (GTK_WINDOW (window), 1280, 720);

  text = gtk_text_view_new ();
  gtk_window_set_child (GTK_WINDOW (window), GTK_WIDGET (text));
  gtk_widget_set_hexpand (text, TRUE);
  gtk_widget_set_vexpand (text, TRUE);

  gtk_window_present (GTK_WINDOW (window));
}

int main (int    argc,
      char **argv)
{
  GtkApplication *app;
  int status;

  app = gtk_application_new ("org.TeDit.example", G_APPLICATION_DEFAULT_FLAGS | G_APPLICATION_HANDLES_OPEN );
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}