#include <gtk/gtk.h>
void end_program(GtkWidget *wid , gpointer ptr)
{
  gtk_main_quit();
}
int main(int argc , char * argv[]){
  gtk_init(&argc , &argv);
  GtkWidget *win = gtk_window_new ( GTK_WINDOW_TOPLEVEL);
  GtkWidget *btn = gtk_button_new_with_label ("Close window");
  g_signal_connect(btn,"clicked",G_CALLBACK(end_program),NULL);//signal of gtk_button_new_with_label
  g_signal_connect (win,"delete_event",G_CALLBACK(end_program)NULL);//signal of x button in window
  gtk_container_add ( GTK_CONTAINER (win) , btn);//place button inside the window
  gtk_widget_show_all(win);
  gtk_main();
  return 0;



}
