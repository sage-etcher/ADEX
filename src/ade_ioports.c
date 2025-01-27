#include "ade.h"
#include "ade_extvars.h"
#include <termios.h>

gint ioports_initialised = 0;

int ttyflags;

void
build_ioports_widgets_from_gresources ()
{

/* debug checkbutton menu stuff */

  g_builder =
    gtk_builder_new_from_resource ("/au/com/itelsoft/ade/ioports.glade");

  g_Wioports_top = GTK_WIDGET (gtk_builder_get_object (g_builder, "ioports_top"));
  g_ioports_top = GTK_WINDOW (g_Wioports_top);
/*radiobuttons*/
  g_Wsio_in_change =
    GTK_WIDGET (gtk_builder_get_object (g_builder, "sio_in_change"));
  g_sio_in_change = GTK_BUTTON (g_Wsio_in_change);


  g_Wsio_in_detach =
    GTK_WIDGET (gtk_builder_get_object (g_builder, "sio_in_detach"));
  g_sio_in_detach = GTK_BUTTON (g_Wsio_in_detach);


  g_Wsio_out_change =
    GTK_WIDGET (gtk_builder_get_object (g_builder, "sio_out_change"));
  g_sio_out_change = GTK_BUTTON (g_Wsio_out_change);


  g_Wsio_out_detach =
    GTK_WIDGET (gtk_builder_get_object (g_builder, "sio_out_detach"));
  g_sio_out_detach = GTK_BUTTON (g_Wsio_out_detach);


  g_Wsio_dev_detach =
    GTK_WIDGET (gtk_builder_get_object (g_builder, "sio_dev_detach"));
  g_sio_out_detach = GTK_BUTTON (g_Wsio_dev_detach);


  g_Wsio_dev_change =
    GTK_WIDGET (gtk_builder_get_object (g_builder, "sio_dev_change"));
  g_pio_in_change = GTK_BUTTON (g_Wsio_dev_change);


  g_Wpio_in_change =
    GTK_WIDGET (gtk_builder_get_object (g_builder, "pio_in_change"));
  g_pio_in_change = GTK_BUTTON (g_Wpio_in_change);


  g_Wpio_in_detach =
    GTK_WIDGET (gtk_builder_get_object (g_builder, "pio_in_detach"));
  g_pio_in_detach = GTK_BUTTON (g_Wpio_in_detach);


  g_Wpio_out_change =
    GTK_WIDGET (gtk_builder_get_object (g_builder, "pio_out_change"));
  g_pio_out_change = GTK_BUTTON (g_Wpio_out_change);


  g_Wpio_out_detach =
    GTK_WIDGET (gtk_builder_get_object (g_builder, "pio_out_detach"));
  g_pio_out_detach = GTK_BUTTON (g_Wpio_out_detach);

  g_Wpio_dev_detach =
    GTK_WIDGET (gtk_builder_get_object (g_builder, "pio_dev_detach"));
  g_pio_out_detach = GTK_BUTTON (g_Wpio_dev_detach);


  g_Wpio_dev_change =
    GTK_WIDGET (gtk_builder_get_object (g_builder, "pio_dev_change"));
  g_pio_in_change = GTK_BUTTON (g_Wpio_dev_change);




  g_Wsio_dev_fn = GTK_WIDGET (gtk_builder_get_object (g_builder, "sio_dev_fn"));
  g_sio_dev_fn = GTK_LABEL (g_Wsio_dev_fn);
  g_Wpio_dev_fn = GTK_WIDGET (gtk_builder_get_object (g_builder, "pio_dev_fn"));
  g_pio_dev_fn = GTK_LABEL (g_Wpio_dev_fn);

  g_Wsio_in_fn = GTK_WIDGET (gtk_builder_get_object (g_builder, "sio_in_fn"));
  g_sio_in_fn = GTK_LABEL (g_Wsio_in_fn);
  g_Wpio_in_fn = GTK_WIDGET (gtk_builder_get_object (g_builder, "pio_in_fn"));
  g_pio_in_fn = GTK_LABEL (g_Wpio_in_fn);

  g_Wsio_out_fn = GTK_WIDGET (gtk_builder_get_object (g_builder, "sio_out_fn"));
  g_sio_out_fn = GTK_LABEL (g_Wsio_out_fn);
  g_Wpio_out_fn = GTK_WIDGET (gtk_builder_get_object (g_builder, "pio_out_fn"));
  g_pio_out_fn = GTK_LABEL (g_Wpio_out_fn);


  gtk_builder_connect_signals (g_builder, NULL);
  g_object_unref (g_builder);

}

// gtk button handlers - wrappers

void
detach_sio_in ()
{
  char streamnbuff[12];
  strcpy (streamnbuff, "sio_in");
  if (detach (streamnbuff))
    {
      strcpy (g_cfg_arg[SIOI], "");
      save_configuration ();
    }
  show_sio_in_current ();
}

void
detach_sio_out ()
{
  char streamnbuff[12];
  strcpy (streamnbuff, "sio_out");
  if (detach (streamnbuff))
    {
      strcpy (g_cfg_arg[SIOO], "");
      save_configuration ();
    }

  show_sio_out_current ();
}

void
detach_sio_device (void)
{
  char streamnbuff[12];
  strcpy (streamnbuff, "sio_dev");
  if (detach (streamnbuff))
    {
      strcpy (g_cfg_arg[SIODEV], "");
      save_configuration ();
    }
  show_sio_device_current ();
}



void
detach_pio_in ()
{
  char streamnbuff[12];
  strcpy (streamnbuff, "pio_in");
  if (detach (streamnbuff))
    {
      strcpy (g_cfg_arg[PIOI], "");
      save_configuration ();
    }
  show_pio_in_current ();
}

void
detach_pio_out ()
{
  char streamnbuff[12];
  strcpy (streamnbuff, "pio_out");
  if (detach (streamnbuff))
    {
      strcpy (g_cfg_arg[PIOO], "");
      save_configuration ();
    }

  show_pio_out_current ();
}


void
detach_pio_device ()
{
  char streamnbuff[12];
  strcpy (streamnbuff, "pio_dev");
  if (detach (streamnbuff))
    {
      strcpy (g_cfg_arg[PIODEV], "");
      save_configuration ();
    }
  show_pio_device_current ();
}


void
ioports_unhide ()
{
  if (!ioports_initialised)
    {
      ioports_setup ();
    }
  gtk_widget_show (g_Wioports_top);
  gtk_window_set_keep_above (g_ioports_top, TRUE);
}



void
ioports_setup ()
{
  /* initialise ioports to what's attached */
  {
    show_sio_in_current ();
  }

  {
    show_sio_out_current ();
  }

  {
    show_pio_in_current ();
  }

  {
    show_pio_out_current ();
  }

  {
    show_sio_device_current ();
  }

  {
    show_pio_device_current ();
  }

  ioports_initialised = 1;
  gtk_widget_show (g_Wioports_top);
}


void
ioports_hide ()
{
  gtk_widget_hide (g_Wioports_top);
}


void
show_sio_in_current ()
{
  xlog (INFO, "show_sio_in_current NI\n");
  if (siotab[2].filename != NULL)
    {
      gtk_label_set_text (g_sio_in_fn, (const gchar *) (siotab[2].filename));
    }
  else
    {
      gtk_label_set_text (g_sio_in_fn, (const gchar *) " sio_in     Detached");
    }

}


void
show_sio_device_current ()
{
  xlog (INFO, "show_sio_device_current: %s\n", siotab[6].filename);
  if (siotab[6].filename != NULL)
    {
      gtk_label_set_text (g_sio_dev_fn, (const gchar *) (siotab[6].filename));
    }
  else
    {
      gtk_label_set_text (g_sio_dev_fn,
			  (const gchar *) " sio_dev     Detached");
    }

}

void
show_sio_out_current ()
{
  xlog (INFO, "show_sio_out_current NI\n");
  if (siotab[3].filename != NULL)
    {
      gtk_label_set_text (g_sio_out_fn, (const gchar *) (siotab[3].filename));
    }
  else
    {
      gtk_label_set_text (g_sio_out_fn,
			  (const gchar *) " sio_out     Detached");
    }

}

void
show_pio_in_current ()
{
  xlog (INFO, "show_pio_in_current NI\n");
  if (siotab[4].filename != NULL)
    {
      gtk_label_set_text (g_pio_in_fn, (const gchar *) (siotab[4].filename));
    }
  else
    {
      gtk_label_set_text (g_pio_in_fn, (const gchar *) " pio_in     Detached");
    }

}


void
show_pio_out_current ()
{
  xlog (INFO, "show_pio_out_current NI\n");
  if (siotab[5].filename != NULL)
    {
      gtk_label_set_text (g_pio_out_fn, (const gchar *) (siotab[5].filename));
    }
  else
    {
      gtk_label_set_text (g_pio_out_fn,
			  (const gchar *) " pio_out     Detached");
    }

}


void
show_pio_device_current ()
{
  xlog (INFO, "show_pio_device_current NI\n");
  if (siotab[7].filename != NULL)
    {
      gtk_label_set_text (g_pio_dev_fn, (const gchar *) (siotab[7].filename));
    }
  else
    {
      gtk_label_set_text (g_pio_dev_fn,
			  (const gchar *) " pio_dev     Detached");
    }

}


void
attach_new_sio_device (void)
{
  char streamnbuff[12];

  strcpy (streamnbuff, "sio_dev");

  xlog (INFO, "attach new sio_dev\n");
  g_choose_mode = GTK_FILE_CHOOSER_ACTION_OPEN;
  select_a_file (g_choose_mode,	// allow creation of new file  or not? set 'save' or 'open'
		 "",		// current folder to look in at start - unspecified here
		 "Looking for a BiDirectional SIO DEVICE",	// title for the file-chooser dialog
		 g_window		// chooser dialog parent window
    );
  if ((g_file_choice_val < 0) || (g_file_choice_name == NULL))
    {
      sprintf (g_vstring, "\nNo Change in SIO DEVICE");
      status_print (g_vstring, 0);
    }
  else
    {
      detach (streamnbuff);
      xlog (INFO, "attach_new_sio_device: %s    %s\n", streamnbuff,
	    g_file_choice_name);
      attach (streamnbuff, g_file_choice_name);
      show_sio_device_current ();
      strcpy (g_cfg_arg[SIODEV], g_file_choice_name);
      detach_sio_in ();		/* replace separate i/o with bidirectional */
      detach_sio_out ();
      save_configuration ();
    }
}

void
attach_new_sio_in ()
{
  char streamnbuff[12];
  strcpy (streamnbuff, "sio_in");

  xlog (INFO, "attach new sio_in NI\n");
  g_choose_mode = GTK_FILE_CHOOSER_ACTION_OPEN;
  select_a_file (g_choose_mode,	// allow creation of new file  or not? set 'save' or 'open'
		 "",		// current folder to look in at start - unspecified here
		 "Looking for an Input File  for SIO-in",	// title for the file-chooser dialog
		 g_window		// chooser dialog parent window
    );
  if ((g_file_choice_val < 0) || (g_file_choice_name == NULL))
    {
      sprintf (g_vstring, "\nNo Change in SIO-in attachment");
      status_print (g_vstring, 0);
    }
  else
    {
      detach (streamnbuff);
      detach_sio_device ();
      attach (streamnbuff, g_file_choice_name);
      show_sio_in_current ();
      strcpy (g_cfg_arg[SIOI], g_file_choice_name);
      save_configuration ();
    }
}

void
attach_new_sio_out ()
{
  char streamnbuff[12];
  strcpy (streamnbuff, "sio_out");

  xlog (INFO, "attach new sio_out NI\n");
  g_choose_mode = GTK_FILE_CHOOSER_ACTION_SAVE;
  select_a_file (g_choose_mode,	// allow creation of new file  or not? set 'save' or 'open'
		 "",		// current folder to look in at start - unspecified here
		 "Looking for an Output File  for SIO-out",	// title for the file-chooser dialog
		 g_window		// chooser dialog parent window
    );
  if ((g_file_choice_val < 0) || (g_file_choice_name == NULL))
    {
      sprintf (g_vstring, "\nNo Change in SIO-out attachment");
      status_print (g_vstring, 0);
    }
  else
    {
      detach (streamnbuff);
      detach_sio_device ();
      attach (streamnbuff, g_file_choice_name);
      show_sio_out_current ();
      strcpy (g_cfg_arg[SIOO], g_file_choice_name);
      save_configuration ();
    }
}


void
attach_new_pio_device (void)
{
  char streamnbuff[12];

  strcpy (streamnbuff, "pio_dev");

  xlog (INFO, "attach new pio_dev\n");
  g_choose_mode = GTK_FILE_CHOOSER_ACTION_OPEN;
  select_a_file (g_choose_mode,	// allow creation of new file  or not? set 'save' or 'open'
		 "",		// current folder to look in at start - unspecified here
		 "Looking for a BiDirectional PIO DEVICE",	// title for the file-chooser dialog
		 g_window		// chooser dialog parent window
    );
  if ((g_file_choice_val < 0) || (g_file_choice_name == NULL))
    {
      sprintf (g_vstring, "\nNo Change in PIO DEVICE");
      status_print (g_vstring, 0);
    }
  else
    {
      detach (streamnbuff);
      xlog (INFO, "attach_new_pio_device: %s    %s\n", streamnbuff,
	    g_file_choice_name);
      attach (streamnbuff, g_file_choice_name);
      show_pio_device_current ();
      strcpy (g_cfg_arg[PIODEV], g_file_choice_name);
      detach_pio_in ();		/* replace separate i/o with bidirectional */
      detach_pio_out ();
      save_configuration ();
    }
}

void
attach_new_pio_in ()
{
  char streamnbuff[12];
  strcpy (streamnbuff, "pio_in");

  xlog (INFO, "attach new pio_in NI\n");
  g_choose_mode = GTK_FILE_CHOOSER_ACTION_OPEN;
  select_a_file (g_choose_mode,	// allow creation of new file  or not? set 'save' or 'open'
		 "",		// current folder to look in at start - unspecified here
		 "Looking for an Input File  for PIO-in",	// title for the file-chooser dialog
		 g_window		// chooser dialog parent window
    );
  if ((g_file_choice_val < 0) || (g_file_choice_name == NULL))
    {
      sprintf (g_vstring, "\nNo Change in PIO-in attachment");
      status_print (g_vstring, 0);
    }
  else
    {
      detach (streamnbuff);
      detach_pio_device ();
      attach (streamnbuff, g_file_choice_name);
      show_pio_in_current ();
      strcpy (g_cfg_arg[PIOI], g_file_choice_name);
      save_configuration ();
    }
}


void
attach_new_pio_out ()
{
  char streamnbuff[12];
  strcpy (streamnbuff, "pio_out");

  xlog (INFO, "attach new pio_out NI\n");
  g_choose_mode = GTK_FILE_CHOOSER_ACTION_SAVE;
  select_a_file (g_choose_mode,	// allow creation of new file  or not? set 'save' or 'open'
		 "",		// current folder to look in at start - unspecified here
		 "Looking for an Input File  for PIO-out",	// title for the file-chooser dialog
		 g_window		// chooser dialog parent window
    );
  if ((g_file_choice_val < 0) || (g_file_choice_name == NULL))
    {
      sprintf (g_vstring, "\nNo Change in PIO-out attachment");
      status_print (g_vstring, 0);
    }
  else
    {
      detach (streamnbuff);
      detach_pio_device ();
      attach (streamnbuff, g_file_choice_name);
      show_pio_out_current ();
      strcpy (g_cfg_arg[PIOO], g_file_choice_name);
      save_configuration ();
    }
}




int
attach (char *stream, char *iofilename)
{
  int file_desc, i, opflags;
  struct sio *s;
  char *p;
  char modestr[5];
  char *mode;

  mode = modestr;

  xlog (INFO, "attach: stream= \"%s\"   iofilename=\"%s\"\n", stream,
	iofilename);
  if (stream)
    {
      p = stream + strlen (stream);
      if (p > stream && *--p == ':')
	{
	  *p = '\0';
	}
      for (i = 0; i < MAXPSTR; i++)
	{
	  s = siotab + i;
	  if (strcmp (stream, s->streamname) == 0)
	    break;
	}
      if (i == MAXPSTR)
	{
	  xlog (INFO, "   Stream not recognized: %s", stream);
	  return 0;
	}

 //     xlog (ALL, "ATTACH:     s->strtype = %d\n", s->strtype);


      if (s->strtype == ST_IN2)
	{
	  if (strcmp (stream, (s + 1)->streamname) == 0)
	    {
	      s++;
	      opflags = O_WRONLY | O_CREAT | O_TRUNC;
	    }
	  else if (strcmp (stream, s->streamname) == 0)
	    {
//	      xlog (ALL, "ATTACH_ELSE_0: stream=%s\n", stream);
	      opflags = O_RDONLY;
	    }
	  else
	    {
	      opflags = O_RDWR | O_CREAT;
	      xlog (ALL, "ATTACH_ELSE_1: stream=%s\n", stream);
	    }
	}
      else
	{
//	  xlog (ALL, "ATTACH_ELSE_2: stream=%s  strtype=%d\n", stream,
//		s->strtype);
	  if (s->strtype == ST_IN)
	    {
	      opflags = O_RDONLY | O_NOCTTY | O_NONBLOCK;
	      xlog (ALL, "ST_IN\n");
	    }
	  else
	    {
	      xlog (ALL, "NOT ST_IN\n");
	      opflags = O_WRONLY | O_NONBLOCK | O_CREAT | O_TRUNC;
	    }
	}

//      opflags =
//        s->strtype ==
//        ST_IN ? O_RDONLY | O_NONBLOCK : O_WRONLY | O_NONBLOCK | O_CREAT | O_TRUNC;
//        }


      if ((strcmp (s->streamname, "pio_in")) == 0)
	{
//	  xlog (ALL, "ATTACH: PIO_IN\n");
	  calloc_buffer_pointer (s->streamname, &g_pio_character_buff_ptr);
	}

      if ((strcmp (s->streamname, "sio_in")) == 0)
	{
//	  xlog (ALL, "ATTACH: SIO_IN\n");
	  calloc_buffer_pointer (s->streamname, &g_sio_character_buff_ptr);
	}

      if (!iofilename || !*iofilename)
	{
	  printf ("   Need a filename");
	  return 0;
	}
      errno = 0;

      if (s->fp)		// is file active? End it if it is.
	{
	  fclose (s->fp);	// end file activity
	  s->fp = NULL;		// destroy file pointer
	  xlog (INFO, "   Detaching \"%s\" from \"%s\"\n", s->streamname,
		s->filename);
	  s->filename[0] = '\0';	// destroy filename
	}

// now attach stream to clean file/port
      xlog (INFO, "   Stream \"%s\" attached to file \"%s\"\n",
	    s->streamname, iofilename);
      if ((file_desc = open (iofilename, opflags, 0666)) < 0)
	{
	  sprintf (g_vstring, "\n%s: Can't Open I/O Port File \"%s\"\n",
		   s->streamname, iofilename);
	  status_print (g_vstring, TRUE);
	}
      else
	{
//	  xlog (ALL, "ATTACH: s->strtype = %d\n", s->strtype);
	  strcpy (mode, "rb");
	  if (opflags & O_WRONLY)
	    {
	      strcpy (mode, "wb");
	    }
	  else
	    {
	      if (opflags & O_RDWR)
		{
		  strcpy (mode, "r+b");
		}
	    }
	  s->filename = iofilename;
	  s->fp = fdopen (file_desc, mode);
	  s->tty = isatty (fileno (s->fp));
	  if (s->tty)
	    {
	      ttyflags = ISATTY;
	    }
	  // if we're looking at ST_IN streams
	  if (!s->strtype)
	    {
	      // if we have 'sio-in'
	      if (!strcmp (s->streamname, "sio_in"))
		{
		  if (tcgetattr (fileno (s->fp), &g_sio_cooked) != 0)
		    {
		      perror ("tcgetattr");
		      exit (1);
		    }
		  else
		    {
//		      xlog (ALL, "tcgetattr OK\n");

		      g_sio_raw = g_sio_cooked;
		      g_sio_raw.c_iflag = 0;
		      g_sio_raw.c_oflag = 0;
		      g_sio_raw.c_lflag = 0;
		      memset (g_sio_raw.c_cc, 0, NCCS);
		      g_sio_raw.c_cc[VINTR] = 0;
		      g_sio_raw.c_cc[VMIN] = 1;
		      sio_raw_tty (s);
		    }
		}


	      // if we have 'pio-in'
	      if (!strcmp (s->streamname, "pio_in"))
		{
		  if (tcgetattr (fileno (s->fp), &g_pio_cooked) != 0)
		    {
		      perror ("tcgetattr");
		      exit (1);
		    }
		  else
		    {
		      xlog (ALL, "tcgetattr OK\n");

		      g_pio_raw = g_pio_cooked;
		      g_pio_raw.c_iflag = 0;
		      g_pio_raw.c_oflag = 0;
		      g_pio_raw.c_lflag = 0;
		      memset (g_pio_raw.c_cc, 0, NCCS);
		      g_pio_raw.c_cc[VINTR] = 0;
		      g_pio_raw.c_cc[VMIN] = 1;
		      pio_raw_tty (s);
		    }
		}
	    }
	}
    }
  else
    {				// show attach table if no args to 'attach'
      for (i = 0; i < MAXPSTR; i++)
	{
	  s = siotab + i;
	  if (s->fp)
	    printf ("   %6s:\t%s\n", s->streamname, s->filename);
	}
    }
  return 0;
}


void
sio_raw_tty (struct sio *s)
{
//  xlog (ALL, "sio_raw_tty:  checking s : s->streamname= %s\n", s->streamname);
  if ((ttyflags & (ISATTY | ISRAW)) == ISATTY)
    {
      tcsetattr (fileno (s->fp), TCSAFLUSH, &g_sio_raw);
      ttyflags |= ISRAW;
    }
}

void
pio_raw_tty (struct sio *s)
{
//  xlog (ALL, "pio_raw_tty:  checking s : s->streamname= %s\n", s->streamname);
  if ((ttyflags & (ISATTY | ISRAW)) == ISATTY)
    {
      tcsetattr (fileno (s->fp), TCSAFLUSH, &g_pio_raw);
      ttyflags |= ISRAW;
    }
}

void
sio_cooked_tty (struct sio *s)
{
  if (s->fp != NULL)
    {
      if (ttyflags & ISRAW)
	{
	  tcsetattr (fileno (s->fp), TCSAFLUSH, &g_sio_cooked);
	  ttyflags &= ~ISRAW;
	}
    }
}


void
pio_cooked_tty (struct sio *s)
{
  if (s->fp != NULL)
    {
      if (ttyflags & ISRAW)
	{
	  tcsetattr (fileno (s->fp), TCSAFLUSH, &g_pio_cooked);
	  ttyflags &= ~ISRAW;
	}
    }
}


int
detach (char *stream)
{
  int i;
  struct sio *s;
  char *p;


  if (stream)
    {
      p = stream + strlen (stream);
      if (p > stream && (*(--p)) == ':')
	{
	  *p = '\0';
	}
      for (i = 0; i < MAXPSTR; i++)
	{
	  s = siotab + i;
	  if (strcmp (stream, s->streamname) == 0)
	    break;
	}
      if (i == MAXPSTR)
	{
	  xlog (INFO, "   Stream not recognized: %s\n", stream);
	  return 0;
	}
      if (s->fp)
	{
	  fclose (s->fp);
	  s->fp = NULL;
	  s->filename = NULL;
	}
    }
  return (1);
}


void
calloc_buffer_pointer (const char *sname, BYTE ** buff_ptr)
{
  xlog (MOTHERBOARD,
	"calloc_buffer_pointerA: %s_character_buff_ptr = (int) %08X\n",
	sname, *buff_ptr);


  if (*buff_ptr == NULL)
    {
      *buff_ptr = (BYTE *) calloc (PORT_IN_BUFF_SIZE, 1);
      if (*buff_ptr == NULL)
	{
	  sprintf (g_vstring, "ERROR: No space for %s_nput Buffer\n", sname);
	  status_print (g_vstring, TRUE);
	}
    }
  xlog (MOTHERBOARD,
	"calloc_buffer_pointerB: %s_character_buff_ptr = (int) %08X\n",
	sname, *buff_ptr);


}
