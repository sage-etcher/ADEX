#include "ade.h"
#include "ade_extvars.h"


void
open_conf_file (void)
{
  int error = 0;
  int bad_open = 0;
  char conf_fn[64];
  char new_dirs[64];
  int dir_ok = 0;

  set_work_dir ();
  // confname already contains a copy of the app's work_dir
  // $HOME/XXXXXXXX/

  if ((dir_ok = is_dir (g_confname)) < 1)
    {
      mkdir_p (g_confname);
      strcpy (new_dirs, g_confname);
      strcat (new_dirs, "disks");
      mkdir_p (new_dirs);
    }

  if (!g_configfileflag)
    {
/* generate default config file name */
      strcpy (conf_fn, "ade");
      strcat (conf_fn, ".conf");
      strcat (g_confname, conf_fn);     // now got $HOME/XXXXXXXX/ade.conf
    }
  else
    {
/* generate special config file name */
      strcat (g_confname, basename (g_xconfigfilename));
    }

  xlog (ALL, "ConfName = \"%s\"\n", g_confname);
  if ((g_conf = fopen (g_confname, "r+")) == NULL)
    {
      error = new_config_file ();
      if ((g_conf = fopen (g_confname, "r+")) == NULL)
        {
          bad_open = 1;
        }
    }


  if ((bad_open) || (error))
    {
      xlog (ALL, "get_config:Starting with blank configuration.\n");
      load_keywords ();
      save_configuration ();
    }

}








void
get_config (void)
{
  if (g_conf == NULL)
    {
      open_conf_file ();
    }
  if (g_conf == NULL)
    {
      sprintf (g_vstring, "Can't open conf file \"%s\"\n", g_confname);
      status_print (g_vstring, TRUE);
      xlog (ALL, "Can't open conf file \"%s\"\n", g_confname);
    }
  else
    {
      load_configuration ();
    }
}



void
load_configuration (void)
{
  char cfgbuff[128];
  int i;
  char *tkey;
  char *targ;
  int tkeynum;
  load_keywords ();
  while ((fgets (cfgbuff, 127, g_conf)) != NULL)
    {
      if ((cfgbuff[0] != '#') && (strlen (cfgbuff) > 3))
        {
          cfgbuff[strlen (cfgbuff) - 1] = '\0'; /*scrub \n from end of second token */
          tkey = strtok (cfgbuff, g_white);
          targ = strtok (NULL, g_white);
          /*convert key to lower-case */
          for (i = 0; (unsigned int) i < strlen (tkey); i++)
            {
              *(tkey + i) = tolower (*(tkey + i));
            }
          i = 0;
          tkeynum = (-1);       /* error value default if not found */
          while (i < (g_max_cfg_key + 1))
            {
              if ((strcmp (g_cfg_key[i], tkey)) == 0)
                {
                  tkeynum = i;
                  i = 1000;     /* exit */
                }
              else
                {
                  i++;
                }
            }
          if (tkeynum > -1)
            {                   /*found a keyword */
              if (targ != NULL)
                {
                  strcpy (g_cfg_arg[tkeynum], targ);
                }
              else
                {
                  strcpy (g_cfg_arg[tkeynum], "");
                }
            }

        }
    }
}



void
load_keywords (void)
{
/*restricts number of, case of, config keywords */
  g_max_cfg_key = 0;
  strcpy (g_cfg_key[HDD], "hdd");
  strcpy (g_cfg_key[FD1], "fd1");
  strcpy (g_cfg_key[FD2], "fd2");
  strcpy (g_cfg_key[DISKD], "disk_dir");
  strcpy (g_cfg_key[DLY], "hd_delay");
  strcpy (g_cfg_key[CAPS], "capslock");
  strcpy (g_cfg_key[CURS], "cursor_lock");
  strcpy (g_cfg_key[SLOTH], "slot_hdc");
  strcpy (g_cfg_key[SLOTS], "slot_sio");
  strcpy (g_cfg_key[SLOTP], "slot_pio");
  strcpy (g_cfg_key[SIODEV], "sio_dev");
  strcpy (g_cfg_key[SIOI], "sio_in");
  strcpy (g_cfg_key[SIOO], "sio_out");
  strcpy (g_cfg_key[PIOI], "pio_in");
  strcpy (g_cfg_key[PIOO], "pio_out");
  strcpy (g_cfg_key[PIODEV], "pio_dev");
  strcpy (g_cfg_key[LOG], "log");
  strcpy (g_cfg_key[SLOG], "screenlog");
  strcpy (g_cfg_key[SEP], "=======");
  strcpy (g_cfg_key[DBG], "debug_level");
  strcpy (g_cfg_key[BRKA], "break_addr");
  strcpy (g_cfg_key[BRKE], "break_on");
  strcpy (g_cfg_key[TRAPA], "trap_addr");
  strcpy (g_cfg_key[TRAPE], "trap_on");
  strcpy (g_cfg_key[CKEND], "END");
  g_max_cfg_key = CKEND;
}


int
new_config_file (void)
{
  int error = 0;
  int dir_ok;
  char newbuff[128];
  set_work_dir ();

  if ((dir_ok = is_dir (g_confname)) < 1)
    {
      mkdir_p (g_confname);
      error = 1;                // nor directory, therefor no conf file
    }
  strcpy (newbuff, g_confname);
  strcat (newbuff, "ade.conf");


  if ((g_conf = fopen (newbuff, "w")) == NULL)
    {
      printf ("Unfortunately, we still can't create our config file.");
      error = 2;
    }
  else
    {
      fclose (g_conf);
    }
  return (error);
}



void
save_configuration (void)
{
  char conf_line[128];

  int i;
  int k;
  int j;
  g_max_cfg_key = CKEND;
//  prepare conf filename
  set_work_dir ();

  if (!g_configfileflag)
    {
/* generate default config file name */
      strcat (g_confname, "ade.conf");
    }
  else
    {
/* generate special config file name */
      strcat (g_confname, basename (g_xconfigfilename));
    }

/* 'wipe' config file. (truncate). Reset file-pointer to start */
  if ((g_conf = fopen (g_confname, "w")) == NULL)
    {
      xlog (ALL, "save_config:  Can't open \"%s\" for writing.\n", g_confname);
    }
  else
    {
      /*write  config file header */
      sprintf (conf_line,
               "##### Configuration File for North Star ADE Emulator (c) %s\n",
               TODAY);
      fputs (conf_line, g_conf);
      strcpy (conf_line, "#####\n");
      fputs (conf_line, g_conf);
      strcpy (conf_line,
              "##### Avoid Editing This File Manually. Any Changes You Make Can\n");
      fputs (conf_line, g_conf);
      strcpy (conf_line,
              "##### Be Automatically Overwritten at Any Time.\n\n");
      fputs (conf_line, g_conf);
      // store log file names from header file
      for (i = 0; i < (g_max_cfg_key); i++)
        {
          k = (14 - strlen (g_cfg_key[i]));
          strcpy (conf_line, g_cfg_key[i]);     /*start with key string */
          if (strlen (g_cfg_arg[i]))
            {                   /* no more unless cfg_arg present */

              for (j = 0; j < k; j++)
                {
                  strcat (conf_line, " ");
                }
              strcat (conf_line, g_cfg_arg[i]);
            }
          strcat (conf_line, "\n");
          xlog (INFO, "conf_line:\t\t\t%s", conf_line);
          fputs (conf_line, g_conf);
        }
      fflush (g_conf);
      fclose (g_conf);
    }
}


void
list_configuration (void)
{
  int i;
  int j;
  int k;
  for (i = 0; i < g_max_cfg_key; i++)
    {
      k = (14 - strlen (g_cfg_key[i]));
      xlog (INFO, "%s", g_cfg_key[i]);
      if (strlen (g_cfg_arg[i]))
        {                       /* no more unless cfg_arg present */

          for (j = 0; j < k; j++)
            {
              xlog (INFO, " ");
            }
          xlog (INFO, "%s", g_cfg_arg[i]);
        }
      xlog (INFO, "\n");
    }
}


void
load_config_parameters (void)
{
  char streamnbuff[12];

  /* PARAMETERS LOADED FROM CONFIG FILE                    */


  strcpy (g_work_dir, (getenv ("HOME")));       // /home directory
  strcat (g_work_dir, "/");     // $HOME/
  strcat (g_work_dir, ADE_CONF_DIR);    // $HOME/advantage
  /* hardware slots */
  set_slots_config ();          //need HDC to be installed before loading disks
/* disk storage */
  if ((strlen (g_cfg_arg[FD1])) > 4)
    {
      floppy_mount (1, g_cfg_arg[FD1], 0);
      g_no_boot_disk = FALSE;
    }

  if ((strlen (g_cfg_arg[FD2])) > 4)
    {
      floppy_mount (2, g_cfg_arg[FD2], 0);
    }

  g_hd5 = (&g_nshd);
  if ((strlen (g_cfg_arg[HDD])) > 4)
    {
      hdmount (g_cfg_arg[HDD]);
    }

  /*debug value */
  g_ade_debug = asc2hex (g_cfg_arg[DBG]);
  sprintf (g_hexstring, "%04X", g_ade_debug);
  gtk_entry_set_text (g_debugvalue, g_hexstring);
  /* break address */
  g_break_address = asc2hex (g_cfg_arg[BRKA]);
  sprintf (g_hexstring, "%04X", g_break_address);
  gtk_entry_set_text (g_break_entry, g_hexstring);
  /* break_enable */
  if ((strcmp (g_cfg_arg[BRKE], "on")) == 0)
    {
      g_break_active = TRUE;
      gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (g_break_enable), TRUE);
    }
  else
    {
      g_break_active = FALSE;
      gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (g_break_enable), FALSE);
      strcpy (g_cfg_arg[BRKE], "off");
    }


  /* trap address */
  g_cpux->trap_address = asc2hex (g_cfg_arg[TRAPA]);
  sprintf (g_hexstring, "%04X", g_cpux->trap_address);
  gtk_entry_set_text (g_trap_entry, g_hexstring);
  /* trap_enable */
  if ((strcmp (g_cfg_arg[TRAPE], "on")) == 0)
    {
      gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (g_trap_enable), TRUE);
      g_trap_active = TRUE;
    }
  else
    {
      g_trap_active = FALSE;
      strcpy (g_cfg_arg[TRAPE], "off");
      gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (g_trap_enable), FALSE);
    }


  /*capslock value */
  if ((strcmp (g_cfg_arg[CAPS], "on")) == 0)
    {
      g_capslock = TRUE;
      turn_capslock (ON);
    }
  else
    {
      g_capslock = FALSE;
      turn_capslock (OFF);
      strcpy (g_cfg_arg[CAPS], "off");
    }

  /*cursor_lock value */
  if ((strcmp (g_cfg_arg[CURS], "on")) == 0)
    {
      g_cursor_lock = TRUE;
      turn_numlock (OFF);
    }
  else
    {
      g_cursor_lock = FALSE;
      turn_numlock (ON);
      strcpy (g_cfg_arg[CURS], "off");
    }

  /*hd_delay value */
  if ((strcmp (g_cfg_arg[DLY], "on")) == 0)
    {
      g_hd_delay = TRUE;
    }
  else
    {
      g_hd_delay = FALSE;
      strcpy (g_cfg_arg[DLY], "off");
    }


  /* logfile name - NOTE 'work_dir' already ends with a '/' */
  sprintf (g_logfilename, "%s%s", g_work_dir, LOGFILENAME);
  sprintf (g_cfg_arg[LOG], "%s%s", g_work_dir, LOGFILENAME);
  /* screenlog name */
  sprintf (g_slogfilename, "%s%s", g_work_dir, SCREENLOGFILENAME);
  sprintf (g_cfg_arg[SLOG], "%s%s", g_work_dir, SCREENLOGFILENAME);
  /*    ioport connections */
  if (strlen (g_cfg_arg[SIOI]))
    {
      strcpy (streamnbuff, "sio_in");
      attach (streamnbuff, g_cfg_arg[SIOI]);
    }

  if (strlen (g_cfg_arg[SIOO]))
    {
      strcpy (streamnbuff, "sio_out");
      attach (streamnbuff, g_cfg_arg[SIOO]);
    }

  if (strlen (g_cfg_arg[SIODEV]))
    {
      strcpy (streamnbuff, "sio_dev");
      attach (streamnbuff, g_cfg_arg[SIODEV]);
    }


  if (strlen (g_cfg_arg[PIOI]))
    {
      strcpy (streamnbuff, "pio_in");
      attach (streamnbuff, g_cfg_arg[PIOI]);
    }


  if (strlen (g_cfg_arg[PIOO]))
    {
      strcpy (streamnbuff, "pio_out");
      attach (streamnbuff, g_cfg_arg[PIOO]);
    }

  if (strlen (g_cfg_arg[PIODEV]))
    {
      strcpy (streamnbuff, "pio_dev");
      attach (streamnbuff, g_cfg_arg[PIODEV]);
    }


  save_configuration ();
}




void
set_work_dir (void)
{

  g_confname = g_confnamebuff;
  xlog (INFO, "PWD= \"%s\"\n", (getenv ("PWD")));
  strcpy (g_confname, (getenv ("HOME")));       // USERNAME /home  directory
  strcat (g_confname, "/");
  strcat (g_confname, "advantage");             // $HOME/XXXXXXXX
  strcat (g_confname, "/");                     // $HOME/XXXXXXXX/
  strcpy (g_work_dir, g_confname);              /* save the working directory info */
}
