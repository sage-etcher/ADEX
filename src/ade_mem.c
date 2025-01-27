#include <gtk/gtk.h>
#include "ade.h"
#include "ade_extvars.h"


void
build_mem_widgets_from_gresources (void)
{
//  GtkBuilder *builder;
  char mpstring[128];
  g_memq = (&g_kqueue[1]);

  g_cline_ptr = g_cline;		//initialise char pointer to start of cline buffer

  g_builder =
    gtk_builder_new_from_resource ("/au/com/itelsoft/ade/mem_top.glade");

  g_Wmemtw = GTK_WIDGET (gtk_builder_get_object (g_builder, "memtw"));
  g_Wmemtext = GTK_WIDGET (gtk_builder_get_object (g_builder, "memtext"));
  g_Wmemscrl = GTK_WIDGET (gtk_builder_get_object (g_builder, "memscrl"));
  g_memscrl = GTK_SCROLLED_WINDOW (g_Wmemscrl);
  gtk_builder_connect_signals (g_builder, NULL);
  g_object_unref (g_builder);


  g_memtext = GTK_TEXT_VIEW (g_Wmemtext);
  g_mem_buffer = gtk_text_view_get_buffer (g_memtext);
  gtk_text_view_set_buffer (GTK_TEXT_VIEW (g_memtext), g_mem_buffer);
  sprintf (mpstring, "RAM DISPLAY -    Hit '?' for Help Information\n\n-");
  mem_print (mpstring);

/*set up monospace font for use with BOTH textview printouts */
  g_mono_font = pango_font_description_from_string ("monospace 10");
  gtk_widget_override_font (g_Wmemtext, g_mono_font);
}


void
mem (void)
{
  gtk_widget_show (g_Wmemtw);
}



void
mem_hide (void)
{
  gtk_widget_hide (g_Wmemtw);
}


gboolean
mem_key (GtkWidget * widget, GdkEventKey * event, gpointer user_data)
{
  WORD extkey;
  char cbuff[10];
  char c;

  UNUSED (widget);
  UNUSED (user_data);

  extkey = event->keyval;

  xlog (KEYB, "Latest key pressed keyval is %06X \n", extkey);
  if (extkey & 0xff00)
    {
      switch (extkey)
	{
	case 0xff0d:
	  extkey = 0x0d;
	  break;
	default:
	  extkey = 0;
	  break;
	}
    }



  c = (extkey & 0x00007f);
// check for capslock
  if (g_capslock)
    {
      c = toupper (c);
    }


//  if ((c > 0x1f) && (c < 0x7f))
//    {
//      prn = c;
//    }


  if (c != 0)
    {
      jqin (1, c, g_memq);
      sprintf (cbuff, "%c", c);
      gtk_text_buffer_place_cursor (g_mem_buffer, &g_mem_end_iter);
      mem_print (cbuff);
      if (c == 0x0d)
	{
	  activate_cli ();
	}
      else
	{
	  *g_cline_ptr = c;
	  g_cline_ptr++;
	  *g_cline_ptr = '\0';
	}
    }
  return TRUE;
}


void
activate_cli (void)
{

  if (g_examine_flag)
    {
      printf ("activate_cli:  examine value = '%s'\n", g_cline);
    }
  else
    {
      strcpy (g_mcmd, g_cline);
      memmon ();
    }
//prepare cline for next use
  g_cline_ptr = g_cline;

}



void
mem_print (char *mptr)
{
  GtkTextMark *mark;
  mark = gtk_text_buffer_get_insert (g_mem_buffer);
  gtk_text_buffer_get_iter_at_mark (g_mem_buffer, &g_mem_end_iter, mark);

  gtk_text_buffer_insert (g_mem_buffer, &g_mem_end_iter, (gchar *) mptr, -1);
  gtk_text_view_set_buffer (GTK_TEXT_VIEW (g_memtext), g_mem_buffer);
  mark = gtk_text_buffer_get_insert (g_mem_buffer);
  gtk_text_view_scroll_mark_onscreen (g_memtext, mark);
  gtk_text_buffer_place_cursor (g_mem_buffer, &g_mem_end_iter);

}
