
#include "ade.h"
#include "ade_extvars.h"

gint slots_initialised = 0;


void
build_slotcard_widgets_from_gresources (void)
{

/* debug checkbutton menu stuff */

  g_builder = gtk_builder_new_from_resource ("/au/com/itelsoft/ade/slot.glade");

  g_Wslottop = GTK_WIDGET (gtk_builder_get_object (g_builder, "slot_top"));

/*radiobuttons*/
  g_Ws1button = GTK_WIDGET (gtk_builder_get_object (g_builder, "s1button"));
  g_s1button = GTK_BUTTON (g_Ws1button);


  g_Ws2button = GTK_WIDGET (gtk_builder_get_object (g_builder, "s2button"));
  g_s2button = GTK_BUTTON (g_Ws2button);


  g_Ws3button = GTK_WIDGET (gtk_builder_get_object (g_builder, "s3button"));
  g_s3button = GTK_BUTTON (g_Ws3button);


  g_Ws4button = GTK_WIDGET (gtk_builder_get_object (g_builder, "s4button"));
  g_s4button = GTK_BUTTON (g_Ws4button);


  g_Ws5button = GTK_WIDGET (gtk_builder_get_object (g_builder, "s5button"));
  g_s5button = GTK_BUTTON (g_Ws5button);


  g_Ws6button = GTK_WIDGET (gtk_builder_get_object (g_builder, "s6button"));
  g_s6button = GTK_BUTTON (g_Ws6button);

  g_Wsunused = GTK_WIDGET (gtk_builder_get_object (g_builder, "sunused"));
  g_sunused = GTK_BUTTON (g_Wsunused);

  g_Wp1button = GTK_WIDGET (gtk_builder_get_object (g_builder, "p1button"));
  g_p1button = GTK_BUTTON (g_Wp1button);


  g_Wp2button = GTK_WIDGET (gtk_builder_get_object (g_builder, "p2button"));
  g_p2button = GTK_BUTTON (g_Wp2button);


  g_Wp3button = GTK_WIDGET (gtk_builder_get_object (g_builder, "p3button"));
  g_p3button = GTK_BUTTON (g_Wp3button);


  g_Wp4button = GTK_WIDGET (gtk_builder_get_object (g_builder, "p4button"));
  g_p4button = GTK_BUTTON (g_Wp4button);


  g_Wp5button = GTK_WIDGET (gtk_builder_get_object (g_builder, "p5button"));
  g_p5button = GTK_BUTTON (g_Wp5button);


  g_Wp6button = GTK_WIDGET (gtk_builder_get_object (g_builder, "p6button"));
  g_p6button = GTK_BUTTON (g_Wp6button);

  g_Wpunused = GTK_WIDGET (gtk_builder_get_object (g_builder, "punused"));
  g_punused = GTK_BUTTON (g_Wpunused);


  g_Wh1button = GTK_WIDGET (gtk_builder_get_object (g_builder, "h1button"));
  g_h1button = GTK_BUTTON (g_Wh1button);


  g_Wh2button = GTK_WIDGET (gtk_builder_get_object (g_builder, "h2button"));
  g_h2button = GTK_BUTTON (g_Wh2button);


  g_Wh3button = GTK_WIDGET (gtk_builder_get_object (g_builder, "h3button"));
  g_h3button = GTK_BUTTON (g_Wh3button);


  g_Wh4button = GTK_WIDGET (gtk_builder_get_object (g_builder, "h4button"));
  g_h4button = GTK_BUTTON (g_Wh4button);


  g_Wh5button = GTK_WIDGET (gtk_builder_get_object (g_builder, "h5button"));
  g_h5button = GTK_BUTTON (g_Wh5button);


  g_Wh6button = GTK_WIDGET (gtk_builder_get_object (g_builder, "h6button"));
  g_h6button = GTK_BUTTON (g_Wh6button);

  g_Whunused = GTK_WIDGET (gtk_builder_get_object (g_builder, "hunused"));
  g_hunused = GTK_BUTTON (g_Whunused);

  g_Wsio_label = GTK_WIDGET (gtk_builder_get_object (g_builder, "sio_label"));
  g_sio_label = GTK_LABEL (g_Wsio_label);

  g_Wpio_label = GTK_WIDGET (gtk_builder_get_object (g_builder, "pio_label"));
  g_pio_label = GTK_LABEL (g_Wpio_label);

  g_Whdc_label = GTK_WIDGET (gtk_builder_get_object (g_builder, "hdc_label"));
  g_hdc_label = GTK_LABEL (g_Whdc_label);

  gtk_builder_connect_signals (g_builder, NULL);
  g_object_unref (g_builder);


}

void
slot_unhide (void)
{
  gtk_widget_show (g_Wslottop);
}

void
slot_hide (void)
{
  gtk_widget_hide (g_Wslottop);
}

void
initialise_slots_array (void)
{
  int i;
  for (i = 1; i < 7; i++)
    {
      g_slotx = (&g_slot[i]);
      strcpy (g_slotx->slotname, "");
      g_slotx->slot_id = 0xFF;
      g_slotx->slotval = 0;
    }
}

void
set_slots_config (void)
{
  int slot_number;


  if (!slots_initialised)
    {
      initialise_slots_array ();
      slots_initialised = 0;
    }

  slot_number = (atoi (g_cfg_arg[SLOTS]));
  if (slot_number)
    {
      sio_add_slot (slot_number);
    }


  slot_number = (atoi (g_cfg_arg[SLOTP]));

  if (slot_number)
    {
      pio_add_slot (slot_number);
    }


  slot_number = (atoi (g_cfg_arg[SLOTH]));
  if (slot_number)
    {
      hdc_add_slot (slot_number);
    }

  show_slot_list ();

}



void
show_slot_list (void)
{
  int i;
  for (i = 1; i < 7; i++)
    {
      g_slotx = (&g_slot[i]);
      if (g_slotx->slot_id == 0xFF)
        {
          xlog (INFO,
                "   Slot No.%d (Ports %cX H):  No peripheral card inserted.  \n",
                i, (6 - i) + '0');
        }
      else
        {
          xlog
            (INFO,
             "   Slot No.%d (Ports %cX H):  Card is \"%s\", card-id of 0x%02X \n",
             i, (6 - i) + '0', g_slotx->slotname, g_slotx->slot_id);
        }
    }
}


void
add_slot_card (const char *cardname, int slotnum, int verbal)
{

  char cname[10];
  int i;
  int found;
/*convert cardname to uppercase*/
  strcpy (cname, cardname);
  for (i = 0;
       (((unsigned int) i < strlen (cardname)) && ((unsigned int) i < 10));
       i++)
    {
      cname[i] = toupper (cname[i]);
    }
  cname[9] = '\0';
  g_slotx = (&g_slot[slotnum]);
  found = 0;


  if (!found)
    {
      if (strcmp ("HDC", cname) == 0)
        {
          found = 1;
          strcpy (g_slotx->slotname, "HDC");
          g_slotx->slot_id = HDC_ID;
          g_hdcslot = slotnum;
        }
    }

  if (!found)
    {
      if (strcmp ("SIO", cname) == 0)
        {
          found = 1;
          strcpy (g_slotx->slotname, "SIO");
          g_slotx->slot_id = SIO_ID;
          g_sioslot = slotnum;
        }
    }


  if (!found)
    {
      if (strcmp ("PIO", cname) == 0)
        {
          found = 1;
          strcpy (g_slotx->slotname, "PIO");
          g_slotx->slot_id = PIO_ID;
          g_pioslot = slotnum;
        }
    }

  if (!found)
    {

      xlog (MOTHERBOARD,
            "add_slot_card:  Card type \"%s\" not available for inserting in slot %d\n",
            cname, slotnum);
      if (verbal)
        {
          printf
            ("   Card type \"%s\" not available for inserting in slot %d\n",
             cname, slotnum);
        }
    }
  else
    {
      xlog (MOTHERBOARD,
            "add_slot_card:  Card type \"%s\"   inserted in slot %d    ID = %02X\n",
            cname, slotnum, g_slotx->slot_id);
      if (verbal)
        {
          printf ("   Card type \"%s\"   inserted in slot %d ID = %02X\n",
                  cname, slotnum, g_slotx->slot_id);
        }
    }


}


void
remove_slot_card (int s)
{

  g_slotx = (&g_slot[s]);
  if (g_slotx->slot_id == 0xFF)
    {
      printf
        ("   Slot Number %d:  ERROR. Can't remove card. None inserted.\n", s);
    }
  else
    {
      xlog (MOTHERBOARD,
            "   Slot card \"%s\", ID=%2X   removed from slot %d\n",
            g_slotx->slotname, g_slotx->slot_id, s);
      printf ("   Slot card \"%s\",  ID = 0x%2X   removed from slot %d\n",
              g_slotx->slotname, g_slotx->slot_id, s);
      g_slotx->slot_id = 0xFF;
      strcpy (g_slotx->slotname, "");
    }
}


void
hdc_add_slot (int newslot)
{

  gchar newinfo[80];

  if (g_hdcslot != newslot)
    {
      if (g_hdcslot)
        {
          remove_slot_card (g_hdcslot);
        }
      add_slot_card ("HDC", newslot, 0);
      g_hdcslot = newslot;
      sprintf (g_cfg_arg[SLOTH], "%d", newslot);
      save_configuration ();
    }
  sprintf (newinfo, "HDC Hard Drive Controller Card Inserted in Slot %d",
           newslot);

  gtk_label_set_text (g_hdc_label, (const gchar *) newinfo);
}

void
sio_add_slot (int newslot)
{

  gchar newinfo[80];

  if (g_sioslot != newslot)
    {
      if (g_sioslot)
        {
          remove_slot_card (g_sioslot);
        }
      add_slot_card ("SIO", newslot, 0);
      g_sioslot = newslot;
      sprintf (g_cfg_arg[SLOTS], "%d", newslot);
      save_configuration ();
    }
  sprintf (newinfo, "SIO Serial Port I/O Card Inserted in Slot %d", newslot);

  gtk_label_set_text (g_sio_label, (const gchar *) newinfo);
}

void
pio_add_slot (int newslot)
{

  char newinfo[80];

  if (g_pioslot != newslot)
    {
      if (g_pioslot)
        {
          remove_slot_card (g_pioslot);
        }
      add_slot_card ("PIO", newslot, 0);
      g_pioslot = newslot;
      sprintf (g_cfg_arg[SLOTP], "%d", newslot);
      save_configuration ();
    }
  sprintf (newinfo, "PIO Parallel Port I/O Card Inserted in Slot %d",
           newslot);

  gtk_label_set_text (g_pio_label, (const gchar *) newinfo);
}



void
slot_0_hx (void)
{
  if (g_hdcslot)
    {
      remove_slot_card (g_hdcslot);
      g_hdcslot = 0;
      gtk_label_set_text (g_hdc_label,
                          (const gchar *)
                          "HDC Hard Drive Controller Card Not Installed");
      sprintf (g_cfg_arg[SLOTH], "0");
      save_configuration ();
    }
}



void
slot_1_hx (void)
{
  if ((g_pioslot == 1) || (g_sioslot == 1))
    {
      status_print ("\nSlot 1 Already in Use", TRUE);
    }
  else
    {
      hdc_add_slot (1);
    }
}

void
slot_2_hx (void)
{
  if ((g_pioslot == 2) || (g_sioslot == 2))
    {
      status_print ("\nSlot 2 Already in Use", TRUE);
    }
  else
    {
      hdc_add_slot (2);
    }
}

void
slot_3_hx (void)
{
  if ((g_pioslot == 3) || (g_sioslot == 3))
    {
      status_print ("\nSlot 3 Already in Use", TRUE);
    }
  else
    {
      hdc_add_slot (3);
    }
}

void
slot_4_hx (void)
{
  if ((g_pioslot == 4) || (g_sioslot == 4))
    {
      status_print ("\nSlot 4 Already in Use", TRUE);
    }
  else
    {
      hdc_add_slot (4);
    }
}

void
slot_5_hx (void)
{
  if ((g_pioslot == 5) || (g_sioslot == 5))
    {
      status_print ("\nSlot 5 Already in Use", TRUE);
    }
  else
    {
      hdc_add_slot (5);
    }
}

void
slot_6_hx (void)
{
  if ((g_pioslot == 6) || (g_sioslot == 6))
    {
      status_print ("\nSlot 6 Already in Use", TRUE);
    }
  else
    {
      hdc_add_slot (6);
    }
}


void
slot_0_sx (void)
{
  if (g_sioslot)
    {
      remove_slot_card (g_sioslot);
      g_sioslot = 0;
      gtk_label_set_text (g_sio_label,
                          (const gchar *)
                          "SIO Serial Port I/O Card Not Installed");
      sprintf (g_cfg_arg[SLOTS], "0");
      save_configuration ();
    }
}

void
slot_1_sx (void)
{
  if ((g_pioslot == 1) || (g_hdcslot == 1))
    {
      status_print ("\nSlot 1 Already in Use", TRUE);
    }
  else
    {
      sio_add_slot (1);
    }
}

void
slot_2_sx (void)
{
  if ((g_pioslot == 2) || (g_hdcslot == 2))
    {
      status_print ("\nSlot 2 Already in Use", TRUE);
    }
  else
    {
      sio_add_slot (2);
    }
}

void
slot_3_sx (void)
{
  if ((g_pioslot == 3) || (g_hdcslot == 3))
    {
      status_print ("\nSlot 3 Already in Use", TRUE);
    }
  else
    {
      sio_add_slot (3);
    }
}

void
slot_4_sx (void)
{
  if ((g_pioslot == 4) || (g_hdcslot == 4))
    {
      status_print ("\nSlot 4 Already in Use", TRUE);
    }
  else
    {
      sio_add_slot (4);
    }
}

void
slot_5_sx (void)
{
  if ((g_pioslot == 5) || (g_hdcslot == 5))
    {
      status_print ("\nSlot 5 Already in Use", TRUE);
    }
  else
    {
      sio_add_slot (5);
    }
}

void
slot_6_sx (void)
{
  if ((g_pioslot == 6) || (g_hdcslot == 6))
    {
      status_print ("\nSlot 6 Already in Use", TRUE);
    }
  else
    {
      sio_add_slot (6);
    }
}


void
slot_0_px (void)
{
  printf ("slot_0_px\n");
  if (g_pioslot)
    {
      remove_slot_card (g_pioslot);
      g_pioslot = 0;
      gtk_label_set_text (g_pio_label,
                          (const gchar *)
                          "PIO Parallel Port I/O Card Not Installed");
      sprintf (g_cfg_arg[SLOTP], "0");
      save_configuration ();
    }

}

void
slot_1_px (void)
{
  if ((g_sioslot == 1) || (g_hdcslot == 1))
    {
      status_print ("\nSlot 1 Already in Use", TRUE);
    }
  else
    {
      pio_add_slot (1);
    }
}

void
slot_2_px (void)
{
  if ((g_sioslot == 2) || (g_hdcslot == 2))
    {
      status_print ("\nSlot 2 Already in Use", TRUE);
    }
  else
    {
      pio_add_slot (2);
    }
}

void
slot_3_px (void)
{
  if ((g_sioslot == 3) || (g_hdcslot == 3))
    {
      status_print ("\nSlot 3 Already in Use", TRUE);
    }
  else
    {
      pio_add_slot (3);
    }
}

void
slot_4_px (void)
{
  if ((g_sioslot == 4) || (g_hdcslot == 4))
    {
      status_print ("\nSlot 4 Already in Use", TRUE);
    }
  else
    {
      pio_add_slot (4);
    }
}

void
slot_5_px (void)
{
  if ((g_sioslot == 5) || (g_hdcslot == 5))
    {
      status_print ("\nSlot 5 Already in Use", TRUE);
    }
  else
    {
      pio_add_slot (5);
    }
}

void
slot_6_px (void)
{
  if ((g_sioslot == 6) || (g_hdcslot == 6))
    {
      status_print ("\nSlot 6 Already in Use", TRUE);
    }
  else
    {
      pio_add_slot (6);
    }
}
