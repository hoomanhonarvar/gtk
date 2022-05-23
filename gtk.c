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
  g_signal_connect (win,"delete_event",G_CALLBACK(end_program),NULL);//signal of x button in window
  GtkWidget *box=gtk_vbox_new(TRUE,5);
  GtkWidget *lbl =gtk_label_new ("alaki");
  gtk_box_pack_start(GTK_BOX (box),lbl,TRUE,TRUE,0);
  gtk_box_pack_start(GTK_BOX(box),btn,TRUE,TRUE,0);
  gtk_container_add(GTK_CONTAINER (win),box);//each container can contain just one widget!!
  gtk_widget_show_all(win);
  gtk_main();
  return 0;



}
