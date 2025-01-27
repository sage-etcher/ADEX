#include "ade.h"
#include "ade_extvars.h"



void
build_break_widgets_from_gresources (void)
{

/* debug checkbutton menu stuff */

  g_builder =
    gtk_builder_new_from_resource ("/au/com/itelsoft/ade/break_top.glade");

  g_Wbreak_top = GTK_WIDGET (gtk_builder_get_object (g_builder, "break_top"));
  g_break_top = GTK_WINDOW (g_Wioports_top);

  g_Wbreak_enable =
    GTK_WIDGET (gtk_builder_get_object (g_builder, "break_enable"));
  g_break_enable = GTK_CHECK_BUTTON (g_Wbreak_enable);

  g_Wtrap_enable = GTK_WIDGET (gtk_builder_get_object (g_builder, "trap_enable"));
  g_trap_enable = GTK_CHECK_BUTTON (g_Wtrap_enable);

  g_Wbreak_entry = GTK_WIDGET (gtk_builder_get_object (g_builder, "break_entry"));
  g_break_entry = GTK_ENTRY (g_Wbreak_entry);

  g_Wtrap_entry = GTK_WIDGET (gtk_builder_get_object (g_builder, "trap_entry"));
  g_trap_entry = GTK_ENTRY (g_Wtrap_entry);

  g_Wbreak_label = GTK_WIDGET (gtk_builder_get_object (g_builder, "break_label"));
  g_break_label = GTK_LABEL (g_Wbreak_label);

  g_Wtrap_label = GTK_WIDGET (gtk_builder_get_object (g_builder, "trap_label"));
  g_trap_label = GTK_LABEL (g_Wtrap_label);


  gtk_builder_connect_signals (g_builder, NULL);
  g_object_unref (g_builder);

}


void
break_unhide (void)
{
  gtk_widget_show (g_Wbreak_top);
  gtk_window_set_keep_above (g_break_top, TRUE);
}


void
break_hide (void)
{
  gtk_widget_hide (g_Wbreak_top);
  sprintf (g_cfg_arg[BRKA], "%04X", (g_break_address & 0x0FFFF));

  sprintf (g_cfg_arg[TRAPA], "%04X", (g_cpux->trap_address & 0x0FFFF));
  save_configuration ();
}





void
trap_toggle (void)
{
  gint gx;
  gx = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (g_trap_enable));
  if (gx)
    {
      status_print ("\nTrap Address is now ENABLED", 0);
      strcpy (g_cfg_arg[TRAPE], "on");
      g_trap_active = TRUE;
    }
  else
    {
      status_print ("\nTrap Address is now DISABLED", 0);
      strcpy (g_cfg_arg[TRAPE], "off");
      g_trap_active = FALSE;
    }
  save_configuration ();

}


void
break_toggle (void)
{
  gint gx;
  gx = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (g_break_enable));

  if (gx)
    {
      g_break_active = TRUE;
      status_print ("\nBreak Address is now ENABLED", 0);
      strcpy (g_cfg_arg[BRKE], "on");
    }
  else
    {
      status_print ("\nBreak Address is now DISABLED", 0);
      strcpy (g_cfg_arg[BRKE], "off");
      g_break_active = FALSE;
    }
  save_configuration ();
}


void
trap_enter (void)
{
  char trap_hex[6];

  strcpy (g_trap_string, gtk_entry_get_text (g_trap_entry));
  if ((strlen (g_trap_string)) > 4)
    {
      g_trap_string[4] = '\0';  //truncate to first 4 chars if longer
    }
  g_cpux->trap_address = asc2hex ((char *) g_trap_string);
  g_cpux->trap_address = g_cpux->trap_address & 0x0FFFF;

  sprintf (trap_hex, "%04X", g_cpux->trap_address);
  gtk_entry_set_text (g_trap_entry, trap_hex);
  strcpy (g_cfg_arg[TRAPA], trap_hex);
  gtk_entry_set_text (g_trap_entry, trap_hex);
  if (g_trap_active)
    {
      strcpy (g_cfg_arg[TRAPE], "on");
    }
  else
    {
      strcpy (g_cfg_arg[TRAPE], "off");
    }
  save_configuration ();
}

void
break_enter (void)
{
  char break_hex[6];

  strcpy (g_break_string, gtk_entry_get_text (g_break_entry));
  if ((strlen (g_break_string)) > 4)
    {
      g_break_string[4] = '\0'; //truncate to first 4 chars if longer
    }
  g_break_address = asc2hex ((char *) g_break_string);
  g_break_address = g_break_address & 0x0FFFF;

  sprintf (break_hex, "%04X", g_break_address);
  gtk_entry_set_text (g_break_entry, break_hex);
  strcpy (g_cfg_arg[BRKA], break_hex);
  if (g_break_active)
    {
      strcpy (g_cfg_arg[BRKE], "on");
    }
  else
    {
      strcpy (g_cfg_arg[BRKE], "off");
    }
  save_configuration ();
}
