#include <gtk/gtk.h>
int count=0;
void countbutton(GtkWidget *wid,gpointer ptr){
  char buffer[30];
  count++;
  sprintf(buffer , "Button pressed %d times",count);
  gtk_label_set_text(GTK_LABEL (ptr),buffer);
}
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
  GtkWidget *btn2=gtk_button_new_with_label ("count button");
  g_signal_connect(btn2,"clicked",G_CALLBACK(countbutton),lbl);
  gtk_box_pack_start(GTK_BOX(box),btn2,FALSE,TRUE,0);//third argument used for expand label or button in resizing
  gtk_box_pack_start(GTK_BOX (box),lbl,TRUE,FALSE,0);//fourth argument "fill" if expand is set to False has no effect
  gtk_box_pack_start(GTK_BOX(box),btn,FALSE,FALSE,0);//final argument is padding and it is space between all widget in pixels
  gtk_container_add(GTK_CONTAINER (win),box);//each container can contain just one widget!!
  gtk_widget_show_all(win);
  gtk_main();
  return 0;



}
