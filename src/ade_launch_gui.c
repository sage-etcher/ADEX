#include "ade.h"
#include "ade_extvars.h"

/* action button call backs */
void
go_action (void)
{
  if (!g_stopsim)
    {                           //already running
      sprintf (g_vstring, "\nADE already running");
      status_print (g_vstring, TRUE);
    }
  else
    {
      if (g_no_boot_disk)
        {
          wait_for_boot_floppy ();
        }
      else
        {
          if (!g_started)
            {
              clear_video_ram ();
              clear_display_buffer ();
            }
          sprintf (g_vstring, "\nAdvantage Emulator is RUNNING");
          status_print (g_vstring, FALSE);
          g_stopsim = 0;
          g_started = TRUE;
        }
    }
}

void
wait_for_boot_floppy (void)
{
  sleep (1);
  if (g_warn_boot_floppy)
    {
      if (g_nsd[0].fdd == NULL)
        {
          sprintf (g_vstring,
                   "\n!!!!  NO BOOT FLOPPY FOUND!   LOAD BOOT FLOPPY (FD1)");
          status_print (g_vstring, TRUE);
          g_warn_boot_floppy = 0;
        }
    }
}


void
pause_action (void)
{
  if (!g_started)
    {
      sprintf (g_vstring,
               "\nCan't 'PAUSE'. Not started yet. Hit 'GO' button to start");
      status_print (g_vstring, TRUE);
    }
  else
    {
      g_stopsim = 1;
      sprintf (g_vstring,
               "\nAdvantage Emulator PAUSED. Hit 'go' to run again.");
      status_print (g_vstring, FALSE);
    }
}

void
reset_action (void)
{
  if (!g_started)
    {
      sprintf (g_vstring,
               "\n'RESET' not valid. Not started yet. Hit 'GO' button to start");
      status_print (g_vstring, TRUE);
    }
  else
    {
      clear_video_ram ();
      clear_display_buffer ();
      g_machine_reset_flag = TRUE;
      sprintf (g_vstring, "\n\nAdvantage Emulator RESET. Rebooted.");
      status_print (g_vstring, FALSE);
      g_stopsim = 0;
    }
}


gboolean
exit_action (void)
{
  /* TODO - tidy up files, environment, etc */
  save_configuration ();
//  sync ();
//  sync ();
  g_stopsim = TRUE;
  gtk_window_close (GTK_WINDOW (g_window));
  topwindow_destroy ();
  set_k_locks_end ();
  gtk_main_quit ();
  sio_cooked_tty (&siotab[SIO_CARD_IN]);
  pio_cooked_tty (&siotab[PIO_CARD_IN]);
  exit (0);
}


/* ************************************ */
/* toggles callbacks */

void
toggle_capslock (void)
{

  g_capslock ^= TRUE;
  if (g_capslock)
    {
      turn_capslock (ON);
//      status_print ("\nCapslock is now ON", 0);
      strcpy (g_cfg_arg[CAPS], "on");
    }
  else
    {
      turn_capslock (OFF);
//      status_print ("\nCapslock is now OFF", 0);
      strcpy (g_cfg_arg[CAPS], "off");
    }
  save_configuration ();
}


/* ************************************ */
/* toggles callbacks */

void
toggle_cursor_lock (void)
{

  g_cursor_lock ^= TRUE;
  if (g_cursor_lock)
    {
      turn_numlock (OFF);
//      status_print ("\nCursor Lock is ON, NUMLOCK is OFF", 0);
      strcpy (g_cfg_arg[CURS], "on");
    }
  else
    {
      turn_numlock (ON);
//      status_print ("\nCursor Lock is OFF, NUMLOCK is ON", 0);
      strcpy (g_cfg_arg[CURS], "off");
    }
  save_configuration ();
}


void
toggle_hd_delay (void)
{
  g_hd_delay ^= TRUE;
  if (g_hd_delay)
    {
      status_print ("\nHD Delay is now ON", 0);
      strcpy (g_cfg_arg[DLY], "on");
    }
  else
    {
      status_print ("\nHD Delay is now OFF", 0);
      strcpy (g_cfg_arg[DLY], "off");
    }
  save_configuration ();
}


void
status_print (const char *scptr, int beep)
{
  GtkTextMark *mark;
  mark = gtk_text_buffer_get_insert (g_status_buffer);
  gtk_text_buffer_get_iter_at_mark (g_status_buffer, &g_status_end_iter, mark);
  gtk_text_buffer_insert (g_status_buffer, &g_status_end_iter,
                          (const gchar *) scptr, -1);
  gtk_text_view_set_buffer (GTK_TEXT_VIEW (g_statusinfo), g_status_buffer);
  mark = gtk_text_buffer_get_insert (g_status_buffer);
  gtk_text_view_scroll_mark_onscreen (g_statusinfo, mark);
  if ((beep) && (!NOBEEP))
    {
      gdk_display_beep (g_gdkdisplay);
    }

}





void
build_widgets_from_gresources (void)
{
  build_mem_widgets_from_gresources ();
  build_debug_widgets_from_gresources ();
  build_slotcard_widgets_from_gresources ();
  build_disks_widgets_from_gresources ();
  build_ioports_widgets_from_gresources ();
  build_break_widgets_from_gresources ();
  build_main_widgets_from_gresources ();
}


void
make_new_pixbufs (void)
{
  g_Wade_win = gtk_image_new_from_pixbuf (g_bannerpb);
  gtk_image_set_from_pixbuf (GTK_IMAGE (g_ade_win), g_bannerpb);
}

int
launch_gui (void)
{
  const gchar *display_name;
  build_widgets_from_gresources ();
//  make_rgb_blank_data ();

// setup keyboard queue pointerss
  g_advq = (&g_kqueue[0]);              // ade main window
  g_memq = (&g_kqueue[1]);              // memtext window
  /*cook status textview widget */
  g_statusinfo = GTK_TEXT_VIEW (g_Wstatusinfo);
  g_settings = GTK_MENU_BAR (g_Wsettings);
  g_ade_win = GTK_IMAGE (g_Wade_win);
  g_winstatusscroll = GTK_SCROLLED_WINDOW (g_Wwinstatusscroll);
  g_status_buffer = gtk_text_view_get_buffer (g_statusinfo);
  gtk_text_view_set_buffer (GTK_TEXT_VIEW (g_statusinfo), g_status_buffer);
  gtk_text_buffer_set_text (g_status_buffer, (const gchar *) "",
                            (gint) 0 /*llen */ );

  g_dots_per_pixel = 2;
  g_x_dots_per_pixel = 1;
  g_y_dots_per_pixel = 2;
  g_display_len =
    (g_dots_per_pixel * ADV_SCREEN_COLS * ADV_VIDRAM_ROWS *
     RGB_BYTES_PER_PIXEL * DISPLAY_PAGES);
  g_rgb_pixbuf_data_len = RGB_PB_SIZE;
  g_rgb_page_offset = g_display_len / 2;

  // display buffer
  g_display_buffer = calloc (1, (g_display_len * 2));
  if (g_display_buffer == NULL)
    {
      printf ("Bad display_buffer memory allocation\n");
      exit (1);
    }

  // pixbuf_data

  g_rgb_pixbuf_data = calloc (1, g_rgb_pixbuf_data_len);
  if (g_rgb_pixbuf_data == NULL)
    {
      printf ("Bad pixbuf_data memory allocation\n");
      exit (1);
    }


  prepare_banner ();
  make_new_pixbufs ();
/* cook mem textview widget */
  g_memtext = GTK_TEXT_VIEW (g_Wmemtext);
  g_mem_buffer = gtk_text_view_get_buffer (g_memtext);
/*set up monospace font for use with BOTH textview printouts */
  g_mono_font = pango_font_description_from_string ("monospace 8");
  gtk_widget_override_font (g_Wstatusinfo, g_mono_font);
  /* stoke up main window */
  gtk_widget_show_all (g_Wwindow);
  g_gdkdisplay = gdk_display_get_default ();
  display_name = (const gchar *) gdk_display_get_name (g_gdkdisplay);
  gdk_display_open (display_name);
  g_signal_connect (G_OBJECT (g_window), "destroy", G_CALLBACK (exit_action),
                    NULL);
  /*status_print (display_name, 0); */
  launch_advantage ();
  g_idle_add ((GSourceFunc) run, NULL);
  g_idle_add ((GSourceFunc) xscreen, NULL);
  g_idle_add ((GSourceFunc) input_idle, NULL);

  gtk_main ();
  return 0;
}



// called when window is closed
void
topwindow_destroy (void)
{
  gtk_window_close (GTK_WINDOW (g_window));
  gtk_main_quit ();
}


void
build_main_widgets_from_gresources (void)
{


/* ADE - main window stuff */

  g_builder =
    gtk_builder_new_from_resource ("/au/com/itelsoft/ade/gxe_top.glade");
  g_Wwindow = GTK_WIDGET (gtk_builder_get_object (g_builder, "topwindow"));
  g_window = GTK_WINDOW (g_Wwindow);
  g_Wstatusinfo = GTK_WIDGET (gtk_builder_get_object (g_builder, "statusinfo"));
  g_Wwinstatusscroll =
    GTK_WIDGET (gtk_builder_get_object (g_builder, "winstatusscroll"));
  g_Wade_win = GTK_WIDGET (gtk_builder_get_object (g_builder, "ade_win"));
  g_Wexit_button = GTK_WIDGET (gtk_builder_get_object (g_builder, "exit_button"));
  g_exit_button = GTK_BUTTON (g_Wexit_button);
  gtk_builder_connect_signals (g_builder, NULL);
  g_object_unref (g_builder);
}




void
open_log_files (void)
{

  if (DEBUG_TERM)
    {
      strcpy (g_nc_logfilename, g_work_dir);
      strcat (g_nc_logfilename, NC_LOGFILENAME);
      g_nc_log = fopen (g_nc_logfilename, "w");
      if ((g_nc_log = fopen (g_nc_logfilename, "w")) == NULL)
        {
          printf ("!!!!: nc_log \"%s\" not opened.\n", g_nc_logfilename);
        }
    }


  strcpy (g_logfilename, g_work_dir);
  strcat (g_logfilename, LOGFILENAME);
  if ((g_logfile = fopen (g_logfilename, "w")) == NULL)
    {
      printf ("!!!!: logfile \"%s\" not opened.\n", g_logfilename);
    }

  strcpy (g_slogfilename, g_work_dir);
  strcat (g_slogfilename, SCREENLOGFILENAME);
  if ((g_slog = fopen (g_slogfilename, "w")) == NULL)
    {
      printf ("!!!!: slogfile \"%s\" not opened.\n", g_slogfilename);
    }
}
