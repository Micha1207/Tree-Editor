/*
 * sted.c - Incompatible with time Text Editor.
 *
 * Released under the GNU General Public License v2.0
 * See LICENSE file for details.
 *
 * Written by Michael Ł. (Micha1207) with ChatGPT help in GNU Emacs.
*/
#include <stdio.h>
#include <gtk/gtk.h>

GtkWidget *text_view;

void on_open_file(GtkWidget *widget, gpointer data) {
  GtkWidget *dialog;
  GtkTextBuffer *buffer;
  char *filename;
  char *contents;
  gsize length;

  dialog = gtk_file_chooser_dialog_new("Open file:", GTK_WINDOW(data), GTK_FILE_CHOOSER_ACTION_OPEN, "_Cancel", GTK_RESPONSE_CANCEL, "_Open", GTK_RESPONSE_ACCEPT, NULL);

  if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
    filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));

    if (g_file_get_contents(filename, &contents, &length, NULL)) {
      buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
      gtk_text_buffer_set_text(buffer, contents, length);
      g_free(contents);
    }

    g_free(filename);
  }

  gtk_widget_destroy(dialog);
}

void on_save_file(GtkWidget *widget, gpointer data) {
  GtkWidget *dialog;
  GtkTextBuffer *buffer;
  GtkTextIter start, end;
  char *filename;
  char *text;

  dialog = gtk_file_chooser_dialog_new("Save file", GTK_WINDOW(data),
                                       GTK_FILE_CHOOSER_ACTION_SAVE,
                                       "_CANCEL", GTK_RESPONSE_CANCEL,
                                       "_SAVE", GTK_RESPONSE_ACCEPT,
                                       NULL);

  if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
    filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));

    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    gtk_text_buffer_get_bounds(buffer, &start, &end);
    text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);

    g_file_set_contents(filename, text, -1, NULL);

    g_free(text);
    g_free(filename);
  }

  gtk_widget_destroy(dialog);
}

void on_quit(GtkWidget *widget, gpointer data) {
  gtk_main_quit();
}

int main(int argc, char *argv[]) {
  GtkWidget *window, *vbox, *toolbar, *open_button, *save_button, *quit_button, *scrolled_window;

  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "STED - Incompatible with time text editor");
  gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);
  g_signal_connect(window, "destroy", G_CALLBACK(on_quit), NULL);

  vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
  gtk_container_add(GTK_CONTAINER(window), vbox);

  toolbar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
  gtk_box_pack_start(GTK_BOX(vbox), toolbar, FALSE, FALSE, 0);

  open_button = gtk_button_new_with_label("OPEN");
  g_signal_connect(open_button, "clicked", G_CALLBACK(on_open_file), window);
  gtk_box_pack_start(GTK_BOX(toolbar), open_button, FALSE, FALSE, 0);

  save_button = gtk_button_new_with_label("SAVE");
  g_signal_connect(save_button, "clicked", G_CALLBACK(on_save_file), window);
  gtk_box_pack_start(GTK_BOX(toolbar), save_button, FALSE, FALSE, 0);

  scrolled_window = gtk_scrolled_window_new(NULL, NULL);
  gtk_widget_set_vexpand(scrolled_window, TRUE);
  gtk_box_pack_start(GTK_BOX(vbox), scrolled_window, TRUE, TRUE, 0);

  text_view = gtk_text_view_new();
  gtk_container_add(GTK_CONTAINER(scrolled_window), text_view);

  gtk_widget_show_all(window);

  gtk_main();

  return 0;
}
