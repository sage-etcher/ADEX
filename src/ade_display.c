#include "ade.h"
#include "ade_extvars.h"

#define NIL (0)			// A name for the void pointer



/* ****** Advantage Window **********/


void
make_xlate_table ()
{
  int ii;
  int xx;
  int yy;


  // index video RAM location to 24-bit display location
  for (ii = 0; ii < ADVANTAGE_VID_RAM_LEN; ii++)
    {
      xx = (ii / 0x100);
      yy = (ii % 0x100) * g_y_dots_per_pixel;

      g_xlt_addr[ii] =
	((yy * 80 * g_x_dots_per_pixel) +
	 (xx * g_x_dots_per_pixel)) * RGB_BYTES_PER_PIXEL * 8;
    }
}

void
expand_bits_to_3_bytes (BYTE * disp_pixel_pointer, BYTE k)
{
  unsigned int j;
  unsigned int v;

/* 8 bits -> 3x8 bytes giving 24 bits per pixel */

  for (j = 0; j < 8; j++)
    {
      for (v = 0; v < RGB_BYTES_PER_PIXEL; v++)
	{
/*** check value of display_pixel_pointer *** gdb problem *****/
	  *(disp_pixel_pointer + ((j * RGB_BYTES_PER_PIXEL) + v)) = g_rgb3_byte[k][j][v];	/* first page of data */
	  *(disp_pixel_pointer + 1920 + ((j * RGB_BYTES_PER_PIXEL) + v)) = g_rgb3_byte[k][j][v];	/* first page of data */

	  *(disp_pixel_pointer + g_rgb_page_offset + ((j * RGB_BYTES_PER_PIXEL) + v)) = g_rgb3_byte[k][j][v];	/* second page of d */
	  *(disp_pixel_pointer + 1920 + g_rgb_page_offset + ((j * RGB_BYTES_PER_PIXEL) + v)) = g_rgb3_byte[k][j][v];	/* second page of d */
	}
    }
}


void
show_all_display_buffer ()
{
  unsigned int i;
  unsigned int xx;
  unsigned int y;

  for (y = 0; y < 512; y++)
    {
      printf ("\n Y:%03X  ", y);	// vertical scan line numbers
      i = ((ADV_SCREEN_COLS * RGB_BYTES_PER_PIXEL) * y);
      for (xx = 0; xx < 250; xx++)
	{
	  if (g_display_buffer[i + (xx * RGB_BYTES_PER_PIXEL)])	/* sample just one of 3 bytes for x */
	    {
	      printf ("X");
	    }
	  else
	    {
	      printf (" ");
	    }
	}
    }
}

void
get_rgb_pixbuf_data (unsigned int video_scan_line)
{
  BYTE *p_ptr;
  unsigned int i;

  p_ptr =
    (g_display_buffer +
     ((video_scan_line * g_y_dots_per_pixel) *
      (ADV_SCREEN_COLS * RGB_BYTES_PER_PIXEL * g_x_dots_per_pixel)));


  for (i = 0; i < RGB_PB_SIZE; i++)
    {
      *(g_rgb_pixbuf_data + i) = *(p_ptr + i);
    }
}


void
show_rgb_pixbuf_data ()
{
  unsigned int i;
  unsigned int xx;
  unsigned int y;

  printf
    ("\n\n\n--- scanline: %d ---- RGB  PIXBUF DATA (640 x 240 pixels) (Only left side shown) ---\n\n",
     g_scanline);

  for (y = 0; y < ADV_SCREEN_ROWS; y++)
    {
      printf ("\n Y:%03X  ", y);	// vertical scan line numbers

      i = ((ADV_SCREEN_COLS * RGB_BYTES_PER_PIXEL) * y);
      for (xx = 0; xx < 250; xx++)
	{
	  if (g_rgb_pixbuf_data[i + (xx * RGB_BYTES_PER_PIXEL)])
	    {
	      printf ("X");
	    }
	  else
	    {
	      printf (" ");
	    }
	}
    }
}


void
output_vbytes (unsigned int vid_addr, BYTE char_line_byte)
{
  vid_addr = vid_addr & 0x07fff;	//should be less than 0x5000
  if (vid_addr > 0x4fff)
    {
      // disregard any display output that isn't "visible"
      vid_addr = 0x4fff;
    }
  g_display_pixel_ptr = (g_display_buffer + (g_xlt_addr[vid_addr]));	//display_pixel_ptr points to first of 24 bytes in display_buffer
  expand_bits_to_3_bytes (g_display_pixel_ptr, char_line_byte);

}




void
make_24_bit_table ()
{
  int i;

  for (i = 0; i < 256; i++)
    {

      if (i & 0x080)
	{
	  g_rgb3_byte[i][0][0] = RED_LEVEL;
	  g_rgb3_byte[i][0][1] = GREEN_LEVEL;
	  g_rgb3_byte[i][0][2] = BLUE_LEVEL;
	}
      else
	{
	  g_rgb3_byte[i][0][0] = 0x00;
	  g_rgb3_byte[i][0][1] = 0x00;
	  g_rgb3_byte[i][0][2] = 0x00;
	}
      if (i & 0x040)
	{
	  g_rgb3_byte[i][1][0] = RED_LEVEL;
	  g_rgb3_byte[i][1][1] = GREEN_LEVEL;
	  g_rgb3_byte[i][1][2] = BLUE_LEVEL;
	}
      else
	{
	  g_rgb3_byte[i][1][0] = 0x00;
	  g_rgb3_byte[i][1][1] = 0x00;
	  g_rgb3_byte[i][1][2] = 0x00;
	}
      if (i & 0x020)
	{
	  g_rgb3_byte[i][2][0] = RED_LEVEL;
	  g_rgb3_byte[i][2][1] = GREEN_LEVEL;
	  g_rgb3_byte[i][2][2] = BLUE_LEVEL;
	}
      else
	{
	  g_rgb3_byte[i][2][0] = 0x00;
	  g_rgb3_byte[i][2][1] = 0x00;
	  g_rgb3_byte[i][2][2] = 0x00;
	}
      if (i & 0x010)
	{
	  g_rgb3_byte[i][3][0] = RED_LEVEL;
	  g_rgb3_byte[i][3][1] = GREEN_LEVEL;
	  g_rgb3_byte[i][3][2] = BLUE_LEVEL;
	}
      else
	{
	  g_rgb3_byte[i][3][0] = 0x00;
	  g_rgb3_byte[i][3][1] = 0x00;
	  g_rgb3_byte[i][3][2] = 0x00;
	}
      if (i & 0x008)
	{
	  g_rgb3_byte[i][4][0] = RED_LEVEL;
	  g_rgb3_byte[i][4][1] = GREEN_LEVEL;
	  g_rgb3_byte[i][4][2] = BLUE_LEVEL;
	}
      else
	{
	  g_rgb3_byte[i][4][0] = 0x00;
	  g_rgb3_byte[i][4][1] = 0x00;
	  g_rgb3_byte[i][4][2] = 0x00;
	}
      if (i & 0x004)
	{
	  g_rgb3_byte[i][5][0] = RED_LEVEL;
	  g_rgb3_byte[i][5][1] = GREEN_LEVEL;
	  g_rgb3_byte[i][5][2] = BLUE_LEVEL;
	}
      else
	{
	  g_rgb3_byte[i][5][0] = 0x00;
	  g_rgb3_byte[i][5][1] = 0x00;
	  g_rgb3_byte[i][5][2] = 0x00;
	}
      if (i & 0x002)
	{
	  g_rgb3_byte[i][6][0] = RED_LEVEL;
	  g_rgb3_byte[i][6][1] = GREEN_LEVEL;
	  g_rgb3_byte[i][6][2] = BLUE_LEVEL;
	}
      else
	{
	  g_rgb3_byte[i][6][0] = 0x00;
	  g_rgb3_byte[i][6][1] = 0x00;
	  g_rgb3_byte[i][6][2] = 0x00;
	}
      if (i & 0x001)
	{
	  g_rgb3_byte[i][7][0] = RED_LEVEL;
	  g_rgb3_byte[i][7][1] = GREEN_LEVEL;
	  g_rgb3_byte[i][7][2] = BLUE_LEVEL;
	}
      else
	{
	  g_rgb3_byte[i][7][0] = 0x00;
	  g_rgb3_byte[i][7][1] = 0x00;
	  g_rgb3_byte[i][7][2] = 0x00;
	}
    }
}


void
clear_video_ram ()
{
  int i;
  BYTE *i_ptr;

  i_ptr = (&g_ram[0x20000]);
  for (i = 0; i < 0x5000; i++)
    {
      *i_ptr = 0;
      i_ptr++;
    }

}


void
clear_display_buffer ()
{
  BYTE *i_ptr;
  unsigned int i;
  i_ptr = g_display_buffer;
  for (i = 0; i < g_display_len; i++)
    {
      *i_ptr = 0;
      i_ptr++;
    }

}



void
make_x2gchar_table ()
{
  int i;
  int j;
  g_xlt[0x00] = 0x000;		//00 00 00 00
  g_xlt[0x01] = 0x003;		//00 00 00 11
  g_xlt[0x02] = 0x00c;		//00 00 11 00
  g_xlt[0x03] = 0x00f;		//00 00 11 11
  g_xlt[0x04] = 0x030;		//00 11 00 00
  g_xlt[0x05] = 0x033;		//00 11 00 11
  g_xlt[0x06] = 0x03c;		//00 11 11 00
  g_xlt[0x07] = 0x03f;		//00 11 11 11
  g_xlt[0x08] = 0x0c0;		//11 00 00 00
  g_xlt[0x09] = 0x0c3;		//11 00 00 11
  g_xlt[0x0a] = 0x0cc;		//11 00 11 00
  g_xlt[0x0b] = 0x0cf;		//11 00 11 11
  g_xlt[0x0c] = 0x0f0;		//11 11 00 00
  g_xlt[0x0d] = 0x0f3;		//11 11 00 11
  g_xlt[0x0e] = 0x0fc;		//11 11 11 00
  g_xlt[0x0f] = 0x0ff;		//11 11 11 11
  for (j = 0; j < 0x10; j++)
    {
      for (i = 0; i < 0x10; i++)
	{
	  g_x2gchar[((j * 16) + i)][0] = g_xlt[j];
	  g_x2gchar[((j * 16) + i)][1] = g_xlt[i];
	}
    }
}



void
prepare_banner ()
{
  int i;

  g_dots_per_pixel = 2;
  g_x_dots_per_pixel = 1;
  g_y_dots_per_pixel = 2;
  g_display_len =
    (g_dots_per_pixel * ADV_SCREEN_COLS * ADV_VIDRAM_ROWS *
     RGB_BYTES_PER_PIXEL * DISPLAY_PAGES);
  g_rgb_pixbuf_data_len = (g_dots_per_pixel * RGB_PB_SIZE);
  g_rgb_page_offset = g_display_len / 2;

  g_videoram = (&g_ram[0x20000]);
  make_x2gchar_table ();
  sprintf (g_bline, "%s %s.", BANNER_TEXT_1, MAJOR);
  strcat (g_bline, TODAY);
  strcat (g_bline, BANNER_TEXT_2);

  if ((g_bline[0] != '#') && (g_bline[0] != '\n') && (strlen (g_bline) > 3))
    {
      if (g_bline[strlen (g_bline) - 1] == '\n')
	{
	  g_bline[strlen (g_bline) - 1] = '\0';	// clean off eol
	}
      g_xnum = 0;
      g_ynum = 0;

      scan_banner_line ();
    }


  for (i = 0x00; i < ADVANTAGE_VID_RAM_LEN; i++)
    {

      g_vr_byte = *(g_videoram + i);	// get 8 pixels ; 8 bits of videoram[i]
      g_display_pixel_ptr = (g_display_buffer + (g_xlt_addr[i]));
      //display_pixel_ptr points to first of 24 bytes in pb24_data
      expand_bits_to_3_bytes (g_display_pixel_ptr, g_vr_byte);
    }

  get_rgb_pixbuf_data (g_scanline);


  gtk_image_set_from_pixbuf (GTK_IMAGE (g_ade_win), gdk_pixbuf_new_from_data (g_rgb_pixbuf_data, GDK_COLORSPACE_RGB,	// colorspace (must be RGB)
									    0,	// has_alpha (0 for no alpha)
									    8,	// bits-per-sample (must be 8)
									    ADV_SCREEN_COLS * g_x_dots_per_pixel,	// 640 cols
									    ADV_SCREEN_ROWS * g_y_dots_per_pixel,	// 240 rows
									    (ADV_SCREEN_COLS * g_x_dots_per_pixel * RGB_BYTES_PER_PIXEL),	// rowstrid
									    NULL, NULL	// destroy_fn, destroy_fn_data
			     ));
}




void
scan_banner_line ()
{
  g_cptr = g_bline;
  while ((*g_cptr) && (*g_cptr != '\n'))
    {

      if (*g_cptr == '`')
	{
	  parse_banner_line ();
	}
      else
	{
	  g_cptr++;
	}
    }
}


void
parse_banner_line ()
{
  char tbuffer[200];
  int i;
  g_cptr++;
  switch (*g_cptr)
    {
    case 'x':			// next two digits are the character offset of 'x' from start of line
      g_cptr++;
      i = 0;
      while ((*g_cptr != '\n') && (*g_cptr != '`') && (i < 3))
	{
	  tbuffer[i] = *g_cptr;
	  tbuffer[i + 1] = '\0';
	  i++;
	  g_cptr++;
	}
      g_xnum = atoi (tbuffer);
      break;
    case 'y':			// next two digits are the character offset of 'y' from top of page
      g_cptr++;
      i = 0;
      while ((*g_cptr != '\n') && (*g_cptr != '`') && (i < 3))
	{
	  tbuffer[i] = *g_cptr;
	  tbuffer[i + 1] = '\0';
	  i++;
	  g_cptr++;
	}
      g_ynum = atoi (tbuffer);
      break;
    case 'X':			//  string of standard sized characters
      g_cptr++;
      i = 0;
      while ((*g_cptr) && (*g_cptr != '\n') && (*g_cptr != '`'))
	{
	  tbuffer[i] = *g_cptr;
	  tbuffer[i + 1] = '\0';
	  i++;
	  g_cptr++;
	}
      strcpy (g_pstring, tbuffer);
      xstore (g_pstring);
      g_pstring[0] = '\0';
      break;
    case 'Z':			//  string of standard sized characters
      g_cptr++;
      i = 0;
      while ((*g_cptr) && (*g_cptr != '\n') && (*g_cptr != '`'))
	{
	  tbuffer[i] = *g_cptr;
	  tbuffer[i + 1] = '\0';
	  i++;
	  g_cptr++;
	}
      strcpy (g_pstring, tbuffer);
      zstore (g_pstring);
      g_pstring[0] = '\0';
      break;
    default:
      break;
    }




}


void
xstore (char *xstring)
{
  BYTE a;
  int i;
  g_location = (g_ynum * 10) + (g_xnum * 0x100);	//bottom of char
  g_location = g_location - 10;
  g_sptr = xstring;
  while (*g_sptr)
    {
      g_xptr = adv_prom_0000_0800 + 0x0561 + ((*g_sptr - 0x20) * 7);
      *(g_videoram + g_location) = 0;	// top line of char is zeroes - clear
      a = *(g_xptr);		//  check hi bits of first line of char bitmap
      if (a & 0xc0)
	{			// bits high signify char with descenders

	  *(g_videoram + g_location + 1) = 0;	// drop char bitmap by 2 lines
	  *(g_videoram + g_location + 2) = 0;
	  i = 0;
	  *(g_videoram + g_location + 3) = (*(g_xptr + i) & 0x3F);
	  for (i = 1; i < 7; i++)
	    {
	      *(g_videoram + g_location + i + 3) = *(g_xptr + i);
	    }
	}
      else
	{
	  for (i = 0; i < 7; i++)
	    {
	      *(g_videoram + g_location + i + 1) = *(g_xptr + i);
	    }
	  *(g_videoram + g_location + 8) = 0;	// 2nd-last line of char is zeroes - clear
	  *(g_videoram + g_location + 9) = 0;	// last line of char is zeroes - clear
	}
      g_sptr++;
      g_location += 0x100;
      g_xnum += 1;
    }

}



void
zstore (char *zstring)
{
  BYTE dropdown;
  int i;
  BYTE cleft;
  BYTE cright;
// double-height chars - 20 (10x2) duplicated lines
  g_location = (g_ynum * 10) + (g_xnum * 0x100);	//bottom of char
  g_location = g_location - 20;
  g_sptr = zstring;
  while (*g_sptr)
    {

      g_xptr = adv_prom_0000_0800 + 0x0561 + ((*g_sptr - 0x20) * 7);
      dropdown = *(g_xptr);	//  check hi bits of first line of char bitmap
      dropdown &= 0x080;	// bits high signify char with descenders

//chars double width as well as doubleheight
//  get two bytes - each one half width
//cleft=x2gchar[*sptr][0];
//cright=x2gchar[*sptr][1]

      if (dropdown)
	{
	  *(g_videoram + g_location + 0) = 0;	// top L line of char is zeroes - clear
	  *(g_videoram + g_location + 0 + 0x100) = 0;	// top R line of char is zeroes - clear
	  *(g_videoram + g_location + 1) = 0;	// drop char bitmap by 2x2 lines
	  *(g_videoram + g_location + 1 + 0x100) = 0;	// drop char bitmap by 2x2 lines
	  *(g_videoram + g_location + 2) = 0;
	  *(g_videoram + g_location + 2 + 0x100) = 0;
	  *(g_videoram + g_location + 3) = 0;
	  *(g_videoram + g_location + 3 + 0x100) = 0;
	  *(g_videoram + g_location + 4) = 0;
	  *(g_videoram + g_location + 4 + 0x100) = 0;
	  *(g_videoram + g_location + 5) = 0;
	  *(g_videoram + g_location + 5 + 0x100) = 0;
	  *(g_videoram + g_location + 6) = 0;
	  *(g_videoram + g_location + 6 + 0x100) = 0;
	  *(g_videoram + g_location + 7) = 0;
	  *(g_videoram + g_location + 7 + 0x100) = 0;

	  i = 0;
//get two bytes for double-width altered value
	  cleft = g_x2gchar[(*(g_xptr + i) & 0x3F)][0];
	  cright = g_x2gchar[(*(g_xptr + i) & 0x3F)][1];
	  *(g_videoram + g_location + 8) = cleft;
	  *(g_videoram + g_location + 8 + 0x100) = cright;
	  *(g_videoram + g_location + 9) = cleft;
	  *(g_videoram + g_location + 9 + 0x100) = cright;

	  for (i = 1; i < 7; i++)
	    {
	      cleft = g_x2gchar[*(g_xptr + i)][0];
	      cright = g_x2gchar[*(g_xptr + i)][1];
	      *(g_videoram + g_location + (2 * i) + 8) = cleft;
	      *(g_videoram + g_location + (2 * i) + 8 + 0x100) = cright;
	      *(g_videoram + g_location + (2 * i) + 9) = cleft;
	      *(g_videoram + g_location + (2 * i) + 9 + 0x100) = cright;
	    }
	}
      else
	{

	  *(g_videoram + g_location + 0) = 0;	// top L line of char is zeroes - clear
	  *(g_videoram + g_location + 0 + 0x100) = 0;	// top R line of char is zeroes - clear
	  *(g_videoram + g_location + 1) = 0;
	  *(g_videoram + g_location + 1 + 0x100) = 0;
	  *(g_videoram + g_location + 2) = 0;
	  *(g_videoram + g_location + 2 + 0x100) = 0;

	  *(g_videoram + g_location + 3) = 0;
	  *(g_videoram + g_location + 3 + 0x100) = 0;
	  for (i = 0; i < 7; i++)
	    {
	      cleft = g_x2gchar[*(g_xptr + i)][0];
	      cright = g_x2gchar[*(g_xptr + i)][1];

	      *(g_videoram + g_location + (2 * i) + 4) = cleft;
	      *(g_videoram + g_location + (2 * i) + 4 + 0x100) = cright;
	      *(g_videoram + g_location + (2 * i) + 5) = cleft;
	      *(g_videoram + g_location + (2 * i) + 5 + 0x100) = cright;
	    }
	}
      g_sptr++;
      g_location += 0x200;
      g_xnum += 2;
    }
}
