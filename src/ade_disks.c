
#include "ade.h"
#include "ade_extvars.h"

gint mdisks_initialised = 0;
const gchar *new_floppy_name;

void
build_disks_widgets_from_gresources (void)
{

/* debug checkbutton menu stuff */

  g_builder =
    gtk_builder_new_from_resource ("/au/com/itelsoft/ade/disks.glade");

  g_Wdisks_top = GTK_WIDGET (gtk_builder_get_object (g_builder, "disks_top"));
  g_disks_top = GTK_WINDOW (g_Wdisks_top);
/*radiobuttons*/
  g_Wfd1_change = GTK_WIDGET (gtk_builder_get_object (g_builder, "fd1_change"));
  g_fd1_change = GTK_BUTTON (g_Wfd1_change);


  g_Wfd1_eject = GTK_WIDGET (gtk_builder_get_object (g_builder, "fd1_eject"));
  g_fd1_eject = GTK_BUTTON (g_Wfd1_eject);


  g_Wfd2_change = GTK_WIDGET (gtk_builder_get_object (g_builder, "fd2_change"));
  g_fd2_change = GTK_BUTTON (g_Wfd2_change);


  g_Wfd2_eject = GTK_WIDGET (gtk_builder_get_object (g_builder, "fd2_eject"));
  g_fd2_eject = GTK_BUTTON (g_Wfd2_eject);

  g_Wnew_floppy_text =
    GTK_WIDGET (gtk_builder_get_object (g_builder, "new_floppy_text"));
  g_new_floppy_text = GTK_ENTRY (g_Wnew_floppy_text);
  gtk_widget_override_font (g_Wnew_floppy_text, g_mono_font);

  g_Wnew_floppy_button =
    GTK_WIDGET (gtk_builder_get_object (g_builder, "new_floppy_button"));
  g_new_floppy_button = GTK_BUTTON (g_Wnew_floppy_button);

  g_Whdd_change = GTK_WIDGET (gtk_builder_get_object (g_builder, "hdd_change"));
  g_hdd_change = GTK_BUTTON (g_Whdd_change);

  g_Whdd_eject = GTK_WIDGET (gtk_builder_get_object (g_builder, "hdd_eject"));
  g_hdd_eject = GTK_BUTTON (g_Whdd_eject);

  g_Whdfn = GTK_WIDGET (gtk_builder_get_object (g_builder, "hdfn"));
  g_hdfn = GTK_LABEL (g_Whdfn);
  g_Wfd1fn = GTK_WIDGET (gtk_builder_get_object (g_builder, "fd1fn"));
  g_fd1fn = GTK_LABEL (g_Wfd1fn);
  g_Wfd2fn = GTK_WIDGET (gtk_builder_get_object (g_builder, "fd2fn"));
  g_fd2fn = GTK_LABEL (g_Wfd2fn);


  gtk_builder_connect_signals (g_builder, NULL);
  g_object_unref (g_builder);


}

void
disks_unhide (void)
{
  if (!mdisks_initialised)
    {
      mdisks_setup ();
    }
  gtk_widget_show (g_Wdisks_top);
  gtk_window_move (g_disks_top, 20, 200);
  gtk_window_set_keep_above (g_disks_top, TRUE);
}



void
mdisks_setup (void)
{
  /* initialise disks to what's mounted */
  if (g_nsd[0].fdd != NULL)
    {
      show_fd1_current ();
    }

  if (g_nsd[1].fdd != NULL)
    {
      show_fd2_current ();
    }

  if (g_nshd.hdd != NULL)
    {
      show_hdd_current ();
    }
  mdisks_initialised = 1;
  gtk_widget_show (g_Wdisks_top);
}


void
disks_hide (void)
{
  gtk_window_set_keep_above (g_disks_top, FALSE);
  gtk_widget_hide (g_Wdisks_top);
}


void
show_fd1_current (void)
{
  if (g_nsd[0].fdd != NULL)
    {
      gtk_label_set_text (g_fd1fn, (const gchar *) g_nsd[0].fdfn);
    }
  else
    {
      gtk_label_set_text (g_fd1fn, (const gchar *) " EMPTY ");
    }
}

void
show_fd2_current (void)
{
  if (g_nsd[1].fdd != NULL)
    {
      gtk_label_set_text (g_fd2fn, (const gchar *) g_nsd[1].fdfn);
    }
  else
    {
      gtk_label_set_text (g_fd2fn, (const gchar *) " EMPTY ");
    }

}

void
show_hdd_current (void)
{
  if (g_nshd.hdd != NULL)
    {
      gtk_label_set_text (g_hdfn, (const gchar *) g_nshd.hdfn);
    }
  else
    {
      gtk_label_set_text (g_hdfn, (const gchar *) " EMPTY ");
    }
}

void
mount_new_fd1 (void)
{
  char cfoldername[128];
  g_choose_mode = GTK_FILE_CHOOSER_ACTION_OPEN;

  if (strlen (g_cfg_arg[DISKD]))
    {
      strcpy (cfoldername, g_cfg_arg[DISKD]);
    }

  select_a_file (g_choose_mode, // allow creation of new file  or not? set 'save' or 'open'
                 cfoldername,   // current folder to look in at start
                 "Looking for a disk image for FD1",    // title for the file-chooser dialog
                 g_window               // chooser dialog parent window
    );

  if ((g_file_choice_val < 0) || (g_file_choice_name == NULL))
    {
      sprintf (g_vstring, "\nNo Change in Floppy Drive 1");
      status_print (g_vstring, 0);
    }
  else
    {
      umount (1);
      floppy_mount (1, g_file_choice_name, 0);
      show_fd1_current ();
      strcpy (g_cfg_arg[FD1], g_file_choice_name);
      strcpy (g_cfg_arg[DISKD], dirname (g_file_choice_name));
      save_configuration ();
    }
}

void
mount_new_fd2 (void)
{
  char cfoldername[128];
  g_choose_mode = GTK_FILE_CHOOSER_ACTION_OPEN;

  if (strlen (g_cfg_arg[DISKD]))
    {
      strcpy (cfoldername, g_cfg_arg[DISKD]);
    }


  g_choose_mode = GTK_FILE_CHOOSER_ACTION_OPEN;

  select_a_file (g_choose_mode, // allow creation of new file  or not? set 'save' or 'open'
                 cfoldername,   // current folder to look in at start
                 "Looking for a disk image for FD2",    // title for the file-chooser dialog
                 g_window               // chooser dialog parent window
    );

  if ((g_file_choice_val < 0) || (g_file_choice_name == NULL))
    {
      sprintf (g_vstring, "\nNo Change in Floppy Drive 2");
      status_print (g_vstring, 0);
    }
  else
    {
      umount (2);
      floppy_mount (2, g_file_choice_name, 0);
      show_fd2_current ();
      strcpy (g_cfg_arg[FD2], g_file_choice_name);
      strcpy (g_cfg_arg[DISKD], dirname (g_file_choice_name));
      save_configuration ();
    }
}

void
mount_new_hdd (void)
{

  char cfoldername[128];
  g_choose_mode = GTK_FILE_CHOOSER_ACTION_OPEN;

  if (strlen (g_cfg_arg[DISKD]))
    {
      strcpy (cfoldername, g_cfg_arg[DISKD]);
    }

  g_choose_mode = GTK_FILE_CHOOSER_ACTION_SAVE;

  select_a_file (g_choose_mode, // allow creation of new file  or not? set 'save' or 'open'
                 cfoldername,   // current folder to look in at start
                 "Looking for a disk image for Hard Disk",      // title for the file-chooser dialog
                 g_window               // chooser dialog parent window
    );

  if ((g_file_choice_val < 0) || (g_file_choice_name == NULL))
    {
      sprintf (g_vstring, "\nNo Change in Hard Disk Drive");
      status_print (g_vstring, 0);
    }
  else
    {
      hdumount ();
      hdmount (g_file_choice_name);
      show_hdd_current ();
      strcpy (g_cfg_arg[HDD], g_file_choice_name);
      strcpy (g_cfg_arg[DISKD], dirname (g_file_choice_name));
      save_configuration ();
    }
}

void
umount_fd1 (void)
{
  if (g_nsd[0].fdd != NULL)
    {
      sprintf (g_vstring, "\nFLOPPY 1  Ejected");
      status_print (g_vstring, FALSE);
    }
  else
    {
      sprintf (g_vstring, "\nCan't  Eject. No Floppy 1.");
      status_print (g_vstring, TRUE);
    }

  umount (1);
  show_fd1_current ();
  strcpy (g_cfg_arg[FD1], "");
  save_configuration ();
}

void
umount_fd2 (void)
{
  if (g_nsd[1].fdd != NULL)
    {
      sprintf (g_vstring, "\nFLOPPY 2  Ejected");
      status_print (g_vstring, FALSE);
    }
  else
    {
      sprintf (g_vstring, "\nCan't  Eject. No Floppy 2.");
      status_print (g_vstring, TRUE);
    }

  umount (2);
  show_fd2_current ();
  strcpy (g_cfg_arg[FD2], "");
  save_configuration ();
}

void
umount_hdd (void)
{
  hdumount ();
  g_nshd.hdd = NULL;
  strcpy (g_cfg_arg[HDD], "");
  save_configuration ();
  show_hdd_current ();
  sprintf (g_vstring, "\nHARD DRIVE  now OFF");
  status_print (g_vstring, TRUE);
}


char absolute_floppy_name[70];


void
get_new_floppy_name (void)
{
  gboolean name_ok;

  new_floppy_name = gtk_entry_get_text (g_new_floppy_text);
  if (*new_floppy_name != '/')
    {                           //NOT ABSOLUTE Filename, = RELATIVE, ADD PWD
      strcpy (absolute_floppy_name, g_cfg_arg[DISKD]);
      strcat (absolute_floppy_name, new_floppy_name);
    }
  else
    {
      strcpy (absolute_floppy_name, new_floppy_name);
    }
  name_ok = check_new_floppy_name ();
  if (!name_ok)
    {
      sprintf (g_vstring, "\nFloppy Name Not Permitted");
      status_print (g_vstring, TRUE);
    }
}

gboolean name_ok = 0;

gboolean
check_new_floppy_name (void)
{
  return (TRUE);
}


void
create_new_floppy (void)
{
  int i, j, sectors, sectsize;
  FILE *nfile;
  int good;
  int bad;
  char xbasename[64];
  BYTE volname[9];
  BYTE nfbuff[512];

  get_new_floppy_name ();
  if ((nfile = fopen (absolute_floppy_name, "wb")) == NULL)
    {
      sprintf (g_vstring, "\nSorry. Can't open <%s>. Aborting\n",
               absolute_floppy_name);
      status_print (g_vstring, TRUE);
    }
  else
    {
      sectors = 700;
      sectsize = 512;
      for (i = 0; i < 8; i++)
        {
          volname[i] = ' ';
        }

      // get 'basename' of new floppy file
      strcpy (xbasename, basename (absolute_floppy_name));
//      make first NSDOS entry name for the disk UPPERCASE
      i = 0;
      while ((*(xbasename + i) != '.') && (i < 8))
        {
          volname[i] = toupper (*(xbasename + i));
          i++;
        }

//      move name to floppy - first entry in directory
      for (i = 0; i < 8; i++)
        {
          nfbuff[i] = volname[i];
        }

      for (j = 8; j < 15; j++)  /* zero out rest of 1st directory entry */
        {
          nfbuff[j] = 0;
        }

      nfbuff[10] = 0x04;        /* set length of directory to 4 sectors */
      nfbuff[12] = 0x80;        /*set disk density to DD */
//      'space/blank' out rest of first sector data
      for (j = 15; j < sectsize; j++)
        {
          nfbuff[j] = 0x20;
        }
      fwrite (nfbuff, sectsize, 1, nfile);      /* write first sector; directory sector */
// prepare one sector's worth of blank-space data for rest of disk
      for (j = 0; j < sectsize; j++)    /* rest of sectors are 'clean' */
        {
          nfbuff[j] = 0x20;
        }
      bad = 0;
// write out the remaining 699 of the 700 disk sectors
      for (j = 1; j < sectors; j++)
        {                       /* write out rest of sectors */
          good = fwrite (nfbuff, sectsize, 1, nfile);
          if (!good)
            {
              bad = TRUE;
            }
        }
      fclose (nfile);
    }
  if (!bad)
    {
      sprintf (g_vstring, "\nNew Floppy \"%s\" Created",
               (char *) absolute_floppy_name);
      status_print (g_vstring, 0);
    }
  else
    {
      sprintf (g_vstring, "\nNew Floppy \"%s\" NOT Created!!!",
               absolute_floppy_name);
      status_print (g_vstring, 1);
    }
  gtk_entry_set_text (g_new_floppy_text, "");

}
