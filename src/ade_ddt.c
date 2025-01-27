/******************************************************************/
/*             memlook.c - ddt-like memory examiner               */
/*                copyright 2008 - jack strangio                  */
/*                                                                */
/* This file is part of ADE - North Star Horizon Emulator         */
/*                                                                */
/******************************************************************/


/********************************************************************************/
/*   This file is part of nse  - North Star Advantage emulator.                   */
/*                                                                              */
/*  This is free software; you can redistribute it and/or modify it under       */
/*   the terms of the GNU General Public License as published by the Free       */
/*   Software Foundation; either version 2 of the License, or (at your          */
/*   option) any later version.                                                 */
/*                                                                              */
/*   This program is distributed in the hope that it will be useful, but        */
/*   WITHOUT ANY WARRANTY; without even the implied warranty of                 */
/*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU          */
/*   General Public License for more details.                                   */
/*                                                                              */
/*   You should have received a copy of the GNU General Public License          */
/*   along with this program; if not, write to the Free Software                */
/*   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA. */
/*                                                                              */
/********************************************************************************/

#include "ade.h"
#include "ade_extvars.h"


/* ---------------------------------------------------------------- */
/* Display memory. If no parameters given, will continue from where */
/* last memory display terminated. If end parameter given is less   */
/* than the start parameter, the end parameter is disregarded and   */
/* the display defaults back to 0x100 bytes. Initial display default */
/* start is at 0x00000.                                             */
/* ---------------------------------------------------------------- */

int
memmon (void)
{
  g_msize = 0x4000;
  gtk_text_buffer_place_cursor (g_mem_buffer, &g_mem_end_iter);
  g_line_start_iter = &g_mem_end_iter;
  getparms ();
  switch (g_mcmd[0])
    {
    case 'p':
    case 'P':
      page_select ();
      break;
    case 'c':
    case 'C':
      compare ();
      break;
    case 'e':
    case 'E':
      g_examine_flag = 1;
      if ((strcmp ((char *) g_parm[0], "")) == 0)
        {
          sprintf (g_line_out, "Error. No address given.\n");
          mem_print (g_line_out);
          break;
        }
      examine ();
      g_examine_flag = 0;
      break;

    case 'd':
    case 'D':
      vdisplay ();
      break;

    case 'x':
    case 'X':
      display ();
      break;

    case 'f':
    case 'F':
      fill ();
      break;
    case 'h':
    case 'H':
      subadd ();
      break;
    case 'l':
    case 'L':
      load ();
      break;
    case 'm':
    case 'M':
      mmmove ();
      break;

    case 'w':
    case 'W':
      if ((strlen (g_filenamebuff) == 0))
        {
          sprintf (g_line_out, "No filename for  writing out.\n");
          mem_print (g_line_out);
        }
      else
        {
          writeout ();
        }
      break;

      /* ------------------------------------------------------------ */
      /* Name. Will prepare a new filename  for saving the memory     */
      /* buffer. If new name is the same as the original name it will */
      /* be disregarded.                                              */
      /* ------------------------------------------------------------ */

    case 'n':
    case 'N':
      if ((!strlen ((char *) g_parm[0])) && (!strlen (g_filenamebuff)))
        {
          sprintf (g_line_out, "No filename for  writing out.\n");
          mem_print (g_line_out);
        }
      else
        {
          if ((strlen (g_filenamebuff)) && (!strlen ((char *) g_parm[0])))
            {
              sprintf (g_line_out,
                       "Current filename for  writing out is '%s'.\n",
                       g_filenamebuff);
              mem_print (g_line_out);
            }
          else
            {
              strcpy (g_filenamebuff, (char *) g_parm[0]);
            }
        }
      break;
      /* -------------------------------------------------------- */
      /* Help. Self-explanatory.                                  */
      /* -------------------------------------------------------- */
    case 's':
    case 'S':
      search ();
      break;
    case 'q':
    case 'Q':
      mem_hide ();
      break;
    case '?':
      help ();
      break;
      /* -------------------------------------------------------- */
      /* Command lines that don't begin with one of the acceptable */
      /*  commands are disregarded.                               */
      /* -------------------------------------------------------- */
    default:
      sprintf (g_line_out, "Error. %c is unrecognised command.\n", g_mcmd[0]);
      mem_print (g_line_out);
      break;
    }
  sprintf (g_line_out, "\n-");
  mem_print (g_line_out);
  g_mcmd[0] = '\0';
  g_q = 0;
  return (1);
}


void
display (void)
{
  int dx;
  static unsigned int dptr = 0;
  static unsigned int dfptr = 0;
  unsigned int tmp;
  /* start is at 0000 and then to follow    on block by block */
  dx = strlen ((char *) g_parm[0]);
  if (dx)
    {                           /* if parm1 , use this */
      tmp = (unsigned int) htoi ((char *) g_parm[0]);   /* value instead of default */
      tmp = tmp & 0x3fff;
      dptr = tmp;
    }
  if (dptr >= g_msize)
    dptr = 0;                   /* truncate error values */
  /* default end is 0x100 bytes to display */
  dfptr = dptr + 0xffl;
  dx = strlen ((char *) g_parm[1]);
  if (dx)
    {                           /* if specify end, */
      tmp = (unsigned int) htoi ((char *) g_parm[1]);   /* use that instead */
      if ((tmp > 0) && (tmp >= dptr))
        {
          tmp = tmp & 0x3fff;
          dfptr = tmp;          /* don't use if less than start */
        }
      if (dfptr > g_msize)
        dfptr = dptr + 0xffl;   /* or bigger */
    }                           /* than buffer */
  sprintf (g_line_out,
           "16K Display (0-3FFF) based at RAM Page %d  (= Absolute Address %06X)\n\n",
           g_ddt_mem_page, (g_ddt_mem_page * 0x4000));
  mem_print (g_line_out);
  hex_display (dptr, dfptr);
  dptr = dfptr + 1l;            /* be ready to continue from end */
}

/* ---------------------------------------------------------------- */
/* Set memory page. If no parameters given, will set active page to */
/* page zero.                                                       */
/* ---------------------------------------------------------------- */
void
page_select (void)
{
  int psx;
  psx = strlen ((char *) g_parm[0]);
  if (psx)
    {                           /* if parm1 , use this */
      g_ddt_mem_page = (unsigned int) asc2hex ((char *) g_parm[0]);     /* value instead of default */
      g_ddt_mem_page = g_ddt_mem_page & 0x0f;
    }
  else
    {
      g_ddt_mem_page = 0;
    }


  g_base = &(g_ram[g_ddt_mem_page * 0x4000]);
  sprintf (g_line_out,
           "Memory Page selected in DDT is %1X hex (%d), absolute address is %06X\n",
           g_ddt_mem_page, g_ddt_mem_page, (g_ddt_mem_page * 0x4000));
  mem_print (g_line_out);
  xlog (INFO,
        "Memory Page selected in DDT is %1X (%d), absolute address is %06X\n",
        g_ddt_mem_page, g_ddt_mem_page, (g_ddt_mem_page * 0x4000));
}

/* ---------------------------------------------------------------- */
/* VDisplay memory. If no parameters given, will continue from where */
/* last memory display terminated. If end parameter given is less   */
/* than the start parameter, the end parameter is disregarded and   */
/* the display defaults back to 0x100 bytes. Initial display default */
/* start is at 0x00000.                                             */
/* ---------------------------------------------------------------- */
void
vdisplay (void)
{
  int vdx;
  static unsigned int dptr = 0;
  static unsigned int dfptr = 0;
  unsigned int tmp;
  g_msize64 = 0x10000;
  /* start is at 0000 and then to follow    on block by block */
  vdx = strlen ((char *) g_parm[0]);
  if (vdx)
    {                           /* if parm1 , use this */
      tmp = (unsigned int) htoi ((char *) g_parm[0]);   /* value instead of default */
      tmp = tmp & 0xffff;
      dptr = tmp;
    }
  if (dptr >= g_msize64)
    dptr = 0;                   /* truncate error values */
  /* default end is 0x100 bytes to display */
  dfptr = dptr + 0xffl;
  vdx = strlen ((char *) g_parm[1]);
  if (vdx)
    {                           /* if specify end, */
      tmp = (unsigned int) htoi ((char *) g_parm[1]);   /* use that instead */
      if ((tmp > 0) && (tmp >= dptr))
        {
          tmp = tmp & 0xffff;
          dfptr = tmp;          /* don't use if less than start */
        }
      if (dfptr > g_msize64)
        dfptr = dptr + 0xffl;   /* or bigger */
    }                           /* than buffer */

  vhex_display (dptr, dfptr);
  dptr = dfptr + 1l;            /* be ready to continue from end */
}


/* -------------------------------------------------------------------- */
/* hex_display() - Displays memory contents 'a la CP/M DDT'  as hex and */
/* text in lines, 16 chars to a line.                                   */
/* -------------------------------------------------------------------- */
void
vhex_display (WORD start, WORD end)
{
  BYTE c;
  char txt[18];
  char minibuff[20];
  int k = 0;
  int i;
  unsigned int ptrpos;
  int map_reg;
  int ram_page;
  if (end >= g_msize64)
    end = g_msize64 - 1;
  ptrpos = start;
  c = GetBYTE (ptrpos);
  map_reg = ptrpos / 0x4000;
  ram_page = g_memory_mapping_register[map_reg] / 0x4000;
  sprintf (g_line_out, "%X %04X ", ram_page, ptrpos);   //address at start of line
  while (ptrpos <= end)
    {

      c = GetBYTE (ptrpos);
      map_reg = ptrpos / 0x4000;
      ram_page = g_memory_mapping_register[map_reg] / 0x4000;
      c = c & 0xff;
      minibuff[0] = '\0';       //clear minibuff
      sprintf (minibuff, "%02X ", c);
      strcat (g_line_out, minibuff);
      c = c & 0x7f;             /* convert to 7-bit ASCII */
      if (c < 0x20)
        c = '.';
      if (c == 0x7f)
        c = '.';
      txt[k] = c;
      txt[k + 1] = '\0';
      k++;
      ptrpos++;
      if ((((int) ptrpos % 16) == 0) || (ptrpos > end))
        {
          if (ptrpos > 0)
            {
              if ((strlen (g_line_out)) < 55)
                {
                  for (i = strlen (g_line_out); i < 55; i++)
                    {
                      strcat (g_line_out, " ");
                    }
                }
              sprintf (minibuff, " %s\n", txt);
              strcat (g_line_out, minibuff);
              mem_print (g_line_out);
            }
          k = 0;
          if (ptrpos <= end)
            {
              sprintf (g_line_out, "%X %04X ", ram_page, ptrpos);
              // more stuff to be appended to line_out, don't print yet??
            }
        }
    }
  sprintf (g_line_out, "\n");
  mem_print (g_line_out);
}



/* ---------------------------------------------------------------- */
/* Write contents of buffer back to file. If a file size parameter  */
/* is given, the file will be written back to that length, being    */
/* either lengthened or truncated in the process.                   */
/* ---------------------------------------------------------------- */
void
writeout (void)
{
  int error;
  int writeflag;
  unsigned int wmflen;
  int bytes_to_write;
  int i;
  if ((strlen (g_filenamebuff)) > 0)
    strcpy (g_outbuff, g_filenamebuff);
  if ((strlen (g_outbuff)) > 0)
    {
      writeflag = 1;
      if ((g_fp = fopen (g_outbuff, "rb+")) == NULL)
        {
          perror ("");
          writeflag = 0;
          if ((g_fp = fopen (g_outbuff, "rb")) == NULL)
            {
              perror ("");
              if ((g_fp = fopen (g_outbuff, "wb+")) != NULL)
                {
                  perror ("");
                  sprintf (g_line_out, "\n   Creating %s", g_outbuff);
                  mem_print (g_line_out);
                  writeflag = 1;
                }
            }
        }

    }


  if ((g_fp == 0) && ((strlen (g_filenamebuff)) == 0))
    {
      sprintf (g_line_out, "   No file open\n");
      mem_print (g_line_out);
    }

  if ((g_fp != 0) && (writeflag == 0))
    {
      sprintf (g_line_out, "   %s is read-only file\n", g_outbuff);
      mem_print (g_line_out);
    }

  if ((g_fp == 0) && (writeflag != 0))
    {

      sprintf (g_line_out, "   Unable to create file %s\n", g_outbuff);
      mem_print (g_line_out);
    }


  if ((g_fp) && (writeflag))
    {

      error = fseek (g_fp, 0l, SEEK_END);
      if (!error)
        {
          wmflen = ftell (g_fp);
          fseek (g_fp, 0l, SEEK_SET);
        }
      else
        {
          wmflen = 0;
        }



      if ((strcmp ((char *) g_parm[0], "")) != 0)
        {
          bytes_to_write = (unsigned int) htoi ((char *) g_parm[0]);
          if (bytes_to_write > 0)
            {
              bytes_to_write = bytes_to_write & 0xffff;
              wmflen = bytes_to_write;
              if (wmflen > g_msize64)
                {
                  wmflen = g_msize64;
                  sprintf (g_line_out,
                           "   Size greater than buffer. Reduced to %04X\n",
                           wmflen);
                  mem_print (g_line_out);
                }
            }
        }
/*transfer the bytes to be written out into our temporary buffer*/
      for (i = 0; i < (bytes_to_write + 1); i++)
        {
          g_tmpbuff[i] = GetBYTE (i);
        }


      error = fseek (g_fp, 0l, SEEK_SET);       /* set file ptr to start. */
      if (!error)
        {
          wmflen = fwrite (g_tmpbuff, 1, bytes_to_write, g_fp);
          fflush (g_fp);
          sprintf (g_line_out, " %d Bytes written. [%04X h]", wmflen, wmflen);
          mem_print (g_line_out);
        }
      else
        {
          sprintf (g_line_out, "\n Error: NO Bytes written.");
          mem_print (g_line_out);
        }
    }
}


/* -------------------------------------------------------------------- */
/* hex_display() - Displays memory contents 'a la CP/M DDT'  as hex and */
/* text in lines, 16 chars to a line.                                   */
/* -------------------------------------------------------------------- */
void
hex_display (WORD start, WORD end)
{
  BYTE c;
  int bottom;
  char txt[21];
  char minibuff[25];
  int k = 0;
  unsigned int ptrpos;
  bottom = g_base - (&(g_ram[0]));
  if (end >= g_msize)
    end = g_msize - 1;
  ptrpos = start;
  sprintf (g_line_out, "%05X ", bottom + ptrpos);
  while (ptrpos <= end)
    {

      c = *(g_base + ptrpos);
      c = c & 0xff;
      sprintf (minibuff, "%02X ", c);
      strcat (g_line_out, minibuff);
      if (k == 7)
        {
          sprintf (minibuff, " ");
          strcat (g_line_out, minibuff);
        }
      c = c & 0x7f;             /* convert to 7-bit ASCII */
      if (c < 0x20)
        c = '.';
      if (c == 0x7f)
        c = '.';
      txt[k] = c;
      txt[k + 1] = '\0';
      k++;
      ptrpos++;
      if ((((int) ptrpos % 16) == 0) || (ptrpos > end))
        {
          if (ptrpos > 0L)
            {
              sprintf (minibuff, " %s", txt);
              strcat (g_line_out, minibuff);
              mem_print (g_line_out);
            }
          k = 0;
          if (ptrpos <= end)
            {
              sprintf (g_line_out, "\n%05X ", bottom + ptrpos);
              // more stuff to be added to this beginning of line
            }
        }
    }
}


/* ---------------------------------------------------------------- */
/* Examine byte values in sequential memory locations. Input is     */
/* terminated by entering a null string ( just a carriage return).  */
/* ---------------------------------------------------------------- */
void
examine (void)
{
  int c;
//  int end;
  char c1buff[2];
  char *c1ptr;
//  end = FALSE;
  g_vector = (unsigned int) htoi ((char *) g_parm[0]) & 0xffff;
  if (g_vector > g_msize64)
    {
      sprintf (g_line_out, "Address not in Buffer. Using Zero\n");
      mem_print (g_line_out);
      g_vector = 0;
    }
  sprintf (g_line_out, "\n");
  mem_print (g_line_out);
  //while (!end)
  //  {
  sprintf (g_line_out, "%04X  %02X=", g_vector, (GetBYTE (g_vector) & 0x0ff));
  c1ptr = g_line_out;
  while (*c1ptr)
    {
      c1buff[0] = *c1ptr;
      printf ("exc1ptr: [%c]\n", *c1ptr);
      c1buff[1] = '\0';
      mem_print (c1buff);
      c1ptr++;
      sleep (1);
    }
  sleep (10);
//  build_string (); NI
  strcat (g_line_out, g_sbuf);
  strcat (g_line_out, "\n");
  mem_print (g_line_out);
  if ((strlen (g_sbuf)) > 0)
    {
      c = (unsigned int) htoi (g_sbuf);
      c = c & 0x00ff;
      PutBYTE (g_vector, c);
      g_vector++;
    }
  //  else{
//      end = TRUE;
//      }
  // }
}

int
verify_hex (char c)
{

  switch (c)
    {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    case 'A':
    case 'B':
    case 'C':
    case 'D':
    case 'E':
    case 'F':
      break;
    default:
      g_ok = 0;
      break;
    }
  return (g_ok);
}



/* -------------------------------------------------------------------- */
/* load(). will load the latest filename given with the 'N' command into*/
/*  memory. If no load address given, the file will be loaded at 000000.*/
/* -------------------------------------------------------------------- */
void
load (void)
{
  unsigned int error, dest_addr, lrlen, truelen;
  unsigned int i;
  unsigned int file_len;
  char up_p0[128];
  char *lcptr;
  lrlen = 0x10000;              /* nominal figure of 64K max len */
  strcpy (up_p0, (char *) g_parm[0]);   //get spare copy of load address
  lcptr = up_p0;
  while ((*lcptr) && ((lcptr - (char *) (&up_p0)) < 64))
    {
      *lcptr = toupper (*lcptr);
      lcptr++;
    }
  *lcptr = '\0';                // truncate load address ancd convert to upper-case
  g_ok = 1;
  lcptr = up_p0;
  while (*lcptr)                // check all chars are true hex chars
    {
      verify_hex (*lcptr);
      lcptr++;
    }


  if (!g_ok)
    {
      sprintf (g_line_out, "Invalid Hexadecimal Load Address: '%s'\n",
               (char *) g_parm[0]);
      mem_print (g_line_out);
    }
  else
    {

      if ((strlen (g_filenamebuff)) == 0)
        {
          sprintf (g_line_out,
                   "No File Name previously supplied with 'n' command.\n");
          mem_print (g_line_out);
        }

      else
        {
          if ((g_fp = fopen (g_filenamebuff, "rb")) == NULL)
            {
              sprintf (g_line_out, "Can't load '%s'. No such file?\n",
                       g_filenamebuff);
              mem_print (g_line_out);
            }

          else
            {

              if ((strcmp ((char *) g_parm[0], "")) != 0)
                {
                  dest_addr = (unsigned int) htoi ((char *) g_parm[0]);
                  if (dest_addr > 0)
                    {
                      dest_addr = dest_addr & 0xffff;
                      lrlen = 0x10000 - dest_addr;      /* new longest length allowed */
                    }
                }
              else
                {
                  dest_addr = 0;
                }

              fseek (g_fp, 0L, SEEK_END);       /* find end of file */
              file_len = (unsigned int) (ftell (g_fp)); /* how long? */
              error = fseek (g_fp, 0l, SEEK_SET);       /* set file ptr to start. */
              if (file_len > lrlen)
                {
                  file_len = lrlen;     /* truncate allowed load length */
                }

              if (!error)
                {
                  truelen = fread (g_tmpbuff, 1, file_len, g_fp);
                  sprintf (g_line_out, "\n %d  Bytes read. [%04X h]", truelen,
                           truelen);
                  mem_print (g_line_out);
                }
              if (truelen == lrlen)
                {
                  sprintf (g_line_out,
                           "File read reaches end of buffer, file probably truncated!");
                  mem_print (g_line_out);
                }
              if (g_fp != NULL)
                fclose (g_fp);
              /* loaded data now sitting in tmpbuff */
              for (i = 0; i < (file_len + 1); i++)
                {
                  PutBYTE (dest_addr + i, g_tmpbuff[i]);
                }

            }
        }
    }
}

/* -------------------------------------------------------------------- */
/* move. moves memory block from 1 place in the buffer to another.      */
/* -------------------------------------------------------------------- */
void
mmmove (void)
{
  int fault = FALSE;
  unsigned int vectord, vectoro;
  if ((strlen ((char *) g_parm[0]) == 0) ||
      (strlen ((char *) g_parm[1]) == 0) || (strlen ((char *) g_parm[2]) == 0))
    {
      sprintf (g_line_out, "Must be three parameters given.\n");
      mem_print (g_line_out);
    }
  else
    {
      g_vector = (unsigned int) htoi ((char *) g_parm[0]);
      vectord = (unsigned int) htoi ((char *) g_parm[1]);
      vectoro = (unsigned int) htoi ((char *) g_parm[2]);
      g_vector = g_vector & 0xffff;     /* source addr        */
      vectord = vectord & 0xffff;       /* end-of-source addr */
      vectoro = vectoro & 0xffff;       /* destination addr   */
      if (!fault)
        {
          if (g_vector > g_msize64)
            {
              sprintf (g_line_out, "Start  address out of buffer.\n");
              mem_print (g_line_out);
              fault = TRUE;
            }
          if (vectord > g_msize64)
            {
              sprintf (g_line_out, "End    address out of buffer.\n");
              mem_print (g_line_out);
              fault = TRUE;
            }
          if (g_vector > g_msize64)
            {
              sprintf (g_line_out, "Target address out of buffer.\n");
              mem_print (g_line_out);
              fault = TRUE;
            }
          if ((vectoro + (vectord - g_vector)) > g_msize64)
            {
              sprintf (g_line_out, "Target end     out of buffer.\n");
              mem_print (g_line_out);
              fault = TRUE;
            }
        }

      if (!fault)
        {
          xfer_via_tmpbuff (g_vector, vectord, vectoro,
                            (vectord - g_vector + vectoro));
        }
    }
}


void
xfer_via_tmpbuff (WORD s_start, WORD s_end, WORD d_start, WORD d_end)
{
  unsigned int i;
  unsigned int xferlength;

  UNUSED (d_end);

  xferlength = (s_end - s_start + 1);   /* include both start and end BYTES */
/* put source contents into buffer */
  for (i = 0; i < (xferlength + 1); i++)
    {
      g_tmpbuff[i] = GetBYTE (s_start + i);
    }

/* put contents from buffer into destination */

  for (i = 0; i < (xferlength + 1); i++)
    {
      PutBYTE (d_start + i, g_tmpbuff[i]);
    }
}



/* -------------------------------------------------------------------- */
/* compare. Compares 2 blocks of memory. Displays addresses ofdifferent    */
/*          values and the values themselves. */
/* -------------------------------------------------------------------- */
void
compare (void)
{
  int fault = FALSE;
  unsigned int vectord, vectoro;
  if ((strlen ((char *) g_parm[0]) == 0) ||
      (strlen ((char *) g_parm[1]) == 0) || (strlen ((char *) g_parm[2]) == 0))
    {
      sprintf (g_line_out, "Must be three parameters given.\n");
    }
  else
    {
      g_vector = (unsigned int) htoi ((char *) g_parm[0]);
      vectord = (unsigned int) htoi ((char *) g_parm[1]);
      vectoro = (unsigned int) htoi ((char *) g_parm[2]);
      g_vector = g_vector & 0xffff;
      vectord = vectord & 0xffff;
      vectoro = vectoro & 0xffff;
      if (!fault)
        {
          if (g_vector > g_msize64)
            {
              sprintf (g_line_out, "\nStart  address out of buffer.");
              mem_print (g_line_out);
              fault = TRUE;
            }
          if (vectord > g_msize64)
            {
              sprintf (g_line_out, "End    address out of buffer.\n");
              mem_print (g_line_out);
              fault = TRUE;
            }
          if (g_vector > g_msize64)
            {
              sprintf (g_line_out, "Target address out of buffer.\n");
              mem_print (g_line_out);
              fault = TRUE;
            }
          if ((vectoro + (vectord - g_vector)) > g_msize64)
            {
              sprintf (g_line_out, "Target end     out of buffer.\n");
              mem_print (g_line_out);
              fault = TRUE;
            }
        }

      if (!fault)
        {
          for (; g_vector <= vectord; g_vector++)
            {
              if (GetBYTE (g_vector) != GetBYTE (vectoro))
                {
                  sprintf (g_line_out, "\n%04X = %02X          %04X = %02X",
                           g_vector, GetBYTE (g_vector), vectoro,
                           GetBYTE (vectoro));
                  mem_print (g_line_out);
                }
              vectoro++;
            }
        }
    }
}


/* ------------------------------------------------------------------ */
/* Fill memory.                                                       */
/* ------------------------------------------------------------------ */
void
fill (void)
{
  unsigned int vectord;
  int fx;
  if ((strlen ((char *) g_parm[0]) == 0) ||
      (strlen ((char *) g_parm[1]) == 0) || (strlen ((char *) g_parm[2]) == 0))
    {
      sprintf (g_line_out, "Must be three parameters given.\n");
      mem_print (g_line_out);
    }
  else
    {
      g_vector = (unsigned int) htoi ((char *) g_parm[0]) & 0xffff;
      vectord = (unsigned int) htoi ((char *) g_parm[1]) & 0xffff;
      fx = (unsigned int) htoi ((char *) g_parm[2]);
      fx = fx & 0xff;
      while (g_vector <= vectord)
        {
          PutBYTE (g_vector, fx);
          g_vector++;
        }
    }
}


/* ------------------------------------------------------------------ */
/* Search memory. Accepts 'string' of byte values separated by a '.'  */
/*                or an ascii string contained defined thus: "abc"    */
/* ------------------------------------------------------------------ */
void
search (void)
{
  int fault = FALSE;
  int match, i, nbytes;
  unsigned int vectord;
  BYTE sbuff[PARM_LEN - 1];
  unsigned int sx;
  nbytes = str_parse (sbuff);
  if ((strlen ((char *) g_parm[0]) == 0) || (strlen ((char *) g_parm[1]) == 0)
      || (nbytes == -1))
    {
      sprintf (g_line_out, "Must be three valid parameters given.\n");
      mem_print (g_line_out);
    }
  else
    {
      g_vector = (unsigned int) htoi ((char *) g_parm[0]);
      vectord = (unsigned int) htoi ((char *) g_parm[1]);
      if (fault)
        sprintf (g_line_out, "Faulty parameters given.\n");
      mem_print (g_line_out);
      g_vector = g_vector & 0xffff;
      vectord = vectord & 0xffff;
      if (!fault)
        {
          if (g_vector > g_msize64)
            {
              sprintf (g_line_out, "Start  address out of buffer.\n");
              mem_print (g_line_out);
              fault = TRUE;
            }
          if (vectord > g_msize64)
            {
              sprintf (g_line_out, "End    address out of buffer.\n");
              mem_print (g_line_out);
              fault = TRUE;
            }
        }
      if (!fault)
        {
          for (; g_vector <= vectord; g_vector++)
            {
              if (GetBYTE (g_vector) == sbuff[0])
                {
                  sx = g_vector;
                  match = 1;
                  for (i = 0; i < nbytes; i++)
                    {
                      if (GetBYTE (sx + i) != sbuff[i])
                        {
                          match = FALSE;
                          i = ENDLOOP;
                        }
                    }
                  if (match)
                    {
                      sprintf (g_line_out, "%04X\n", g_vector);
                      mem_print (g_line_out);
                    }
                }
            }
        }
    }
}


/* -------------------------------------------------------------------- */
/* Subadd. Hexadecimal arithmetic.                                      */
/* -------------------------------------------------------------------- */
void
subadd (void)
{
  long xx, y;
  if ((strlen ((char *) g_parm[0]) == 0) || (strlen ((char *) g_parm[1]) == 0))
    {
      sprintf (g_line_out, "Must be two parameters given.\n");
      mem_print (g_line_out);
    }
  else
    {
      xx = ahtol (g_parm[0]);
      y = ahtol (g_parm[1]);
      sprintf (g_line_out, " ADDITION AND SUBTRACTION: SIGNED, 4 Digits \n");
      mem_print (g_line_out);
      sprintf (g_line_out, " %04X      Plus  %04X      Equals    %04X \n",
               (WORD) xx, (WORD) y, (WORD) (xx + y));
      mem_print (g_line_out);
      sprintf (g_line_out, " %04X      Minus %04X      Equals    %04X \n",
               (WORD) xx, (WORD) y, (WORD) (xx - y));
      mem_print (g_line_out);
      sprintf (g_line_out, "\n\n");
      mem_print (g_line_out);
      sprintf (g_line_out, " ADDITION AND SUBTRACTION: SIGNED, 2 Digits \n");
      mem_print (g_line_out);
      sprintf (g_line_out, " %02X      Plus  %02X      Equals    %02X \n",
               (BYTE) xx, (BYTE) y, (BYTE) (xx + y));
      mem_print (g_line_out);
      sprintf (g_line_out, " %02X      Minus %02X      Equals    %02X \n",
               (BYTE) xx, (BYTE) y, (BYTE) (xx - y));
      mem_print (g_line_out);
    }
}


/* -------------------------------------------------------------------- */
/* getparms()   -- Parses UDDT command line obtaining the parameters for */
/*                 the various submodules - present maximum of 3        */
/*                 parameters looked for and stored in the parms array. */
/* -------------------------------------------------------------------- */

/************************************************/
/**       This function is faulty             **/
/**   String parameters MUST NOT contain any  **/
/**   embedded SPACES or they will be truncated**/
/***********************************************/


void
getparms (void)
{
  char *xx, *y;
  int a, end;
  xx = &g_mcmd[1];
  g_parm[0][0] = '\0';          /* initialise all parms to "" */
  g_parm[1][0] = '\0';
  g_parm[2][0] = '\0';
  end = FALSE;
  for (a = 0; a < MAX_PARAMS; a++)
    {                           /* get start of parm */
      if (*xx != '\0')
        {
          while ((*xx == ' ') || (*xx == ','))
            {
              xx++;
              if (xx > &g_mcmd[PARM_LEN - 3])
                *xx = '\0';     /* kill if gross */
            }

          /* get end of parm */
          y = xx;
          while ((*y != ' ') && (*y != ',') && (*y != '\n') && (*y != '\0')
                 && (y < &g_mcmd[PARM_LEN - 3]))
            y++;
          if (*y == '\0')
            end = TRUE;         /* if end of parms, abort 'for' loop */
          *y = '\0';
          strcpy ((char *) g_parm[a], xx);
          xx = y + 1;           /* set start pointer in readiness for next parm */
          if (end)
            a = MAX_PARAMS;     /* abort by maximising the loop index */
        }
    }
}


/* ---------------------------------------------------------------------- */
/* str_parse. parse search string according to whether it is a '"'-delimited */
/*         ascii string or a set of '.'-separated ascii hex bytes.        */
/*         e.g. search string could be "ABC" or 41.42.43                  */
/* ---------------------------------------------------------------------- */

int
str_parse (BYTE * buff)
{
  int fault;
  int a, xx, end, nbytes;
  char sbyte[MAXBYTES][3];
  BYTE *qq, *r;
  nbytes = -1;                  /* init number of bytes to error value */
  fault = TRUE;
  if (g_parm[2][0] == '"')
    {                           /* Starts with " therefore ascii string */
      xx = 1;
      while (xx < (int) strlen ((char *) g_parm[2]))
        {
          if (g_parm[2][xx] == '"')
            {                   /* No end with " , faulty string */
              g_parm[2][xx] = '\0';
              fault = FALSE;
            }
          xx++;
        }

      if (!fault)
        {
          strcpy ((char *) buff, (char *) &g_parm[2][1]);
          nbytes = strlen ((char *) buff);
        }
    }

  else
    {
      if (strlen ((char *) g_parm[2]) != 0)
        fault = FALSE;
      for (xx = 0; xx < MAXBYTES; xx++)
        {
          sbyte[xx][0] = '\0';  /* initialise all parms to "" */
        }

      end = FALSE;
      qq = &g_parm[2][0];
      for (a = 0; a < MAXBYTES; a++)
        {                       /* get start of byte parm */
          if (*qq != '\0')
            {
              while ((*qq == ' ') || (*qq == ','))
                {
                  qq++;
                  if (qq > &g_parm[2][PARM_LEN - 2])
                    *qq = '\0';
                  /* kill if gross */
                }

              /* get end of parm */
              r = qq;
              while ((*r != '.') && (*r != '\n') && (*r != '\0')
                     && (r < &g_parm[2][PARM_LEN - 3]))
                r++;
              if (*r == '\0')
                end = TRUE;     /* if end of parms, abort 'for'loop */
              *r = '\0';
              strcpy (sbyte[a], (char *) qq);
              qq = r + 1;       /* set start pointer in readiness for next parm */
              if ((end) && (!fault))
                nbytes = a + 1;
              /* return number of search bytes */
              if (end)
                a = MAXBYTES;   /* abort by maximising the loop index */
            }
          for (xx = 0; xx < nbytes; xx++)
            buff[xx] = (char) (htoi (sbyte[xx]));
        }

    }
  return (nbytes);              /* number of bytes to search for */
}


/* -------------------------------------------------------------------- */
/* ahtol()    --    converts ascii string in hex to a long              */
/* -------------------------------------------------------------------- */
long
ahtol (BYTE * str)
{
  int c;
  int a, b;
  long xx;
  int fault;
  xx = 0;
  fault = FALSE;
  /* First, do a check for non-hex chars. Just in case. */
  if ((strlen ((char *) str)) > 6)
    fault = TRUE;               /* hex string too big */
  for (a = 0; a < (int) strlen ((char *) str); a++)
    {
      c = toupper (*(str + a));
      if ((c < '0') || (c > 'F'))
        fault = TRUE;
      else if ((c > '9') && (c < 'A'))
        fault = TRUE;
    }


  if (fault)
    xx = -1l;
  else
    {
      for (a = 0; a < (int) strlen ((char *) str); a++)
        {
          xx = xx * 16l;
          b = toupper (*(str + a)) - '0';
          if (b > 10)
            {
              b = b - 7;
            }
          xx = xx + (long) b;
          /*            xx=xx & 0x3fffff; */
        }
    }
  return (xx);
}

/* -------------------------------------------------------------------- */
/* htoi()    --    converts ascii string in hex to an integer           */
/* -------------------------------------------------------------------- */
int
htoi (char *str)
{
  char c;
  int a, b;
  int xx;
  int i;
  int fault;
  for (i = 0; i < (int) strlen (str); i++)
    {
      str[i] = toupper (str[i]);
    }
  xx = 0;
  fault = FALSE;
  /* First, do a check for non-hex chars. Just in case. */
  if (strlen (str) > 4)
    fault = TRUE;               /* hex string too big */
  for (a = 0; a < (int) strlen (str); a++)
    {
      c = toupper (*(str + a));
      if ((c < '0') || (c > 'F'))
        fault = TRUE;
      else if ((c > '9') && (c < 'A'))
        fault = TRUE;
    }

  if (fault)
    xx = -1;
  else
    {
      for (a = 0; a < (int) strlen (str); a++)
        {
          xx = xx * 16;
          b = toupper (*(str + a)) - '0';
          if (b > 10)
            {
              b = b - 7;
            }
          xx = xx + (int) b;
          xx = xx & 0xffff;
        }
    }
  return (xx);
}

/* --------------------------------------------------------------- */
/*  help text. Displays commands and required parameters.          */
/* --------------------------------------------------------------- */
void
help (void)
{
  sprintf (g_line_out, " Help : Command and Parameter List\n\n");
  mem_print (g_line_out);
  sprintf (g_line_out,
           " '?' Displays this 'help' page. Upper or lower case commands are accepted\n");
  mem_print (g_line_out);
  sprintf (g_line_out, " <xxx> is a required parameter\n");
  mem_print (g_line_out);
  sprintf (g_line_out, " [xxx] is an optional parameter\n\n");
  mem_print (g_line_out);
  sprintf (g_line_out,
           "C <start address>  <finish address> <start of compared block>        :Compare\n");
  mem_print (g_line_out);
  sprintf (g_line_out,
           "D [start address] [finish address]        :Display Memory-Mapped RAM Contents\n");
  mem_print (g_line_out);
//  sprintf (line_out, "E <start address>                                      :Examine/change memory\n");
//  mem_print (line_out);
  sprintf (g_line_out,
           "F <start address>  <finish address> <fill byte>        :Fill memory with byte\n");
  mem_print (g_line_out);
  sprintf (g_line_out,
           "H <value> <value>                                             :Hex arithmetic\n");
  mem_print (g_line_out);
  sprintf (g_line_out,
           "L [load address]                                       :Load file into memory\n");
  mem_print (g_line_out);
  sprintf (g_line_out,
           "M <source start address> <source end> <destination>              :Move memory\n");
  mem_print (g_line_out);
  sprintf (g_line_out,
           "N <file name>                                        :Change active file name\n");
  mem_print (g_line_out);
  sprintf (g_line_out,
           "P [RAM Page 0-F hex]      :Select 16K ram-page (0000-3FFF) for display by 'X'\n");
  mem_print (g_line_out);
  sprintf (g_line_out,
           "S <start address> <end address> <search byte.byte.. | \"string\">       :Search\n");
  mem_print (g_line_out);
  sprintf (g_line_out,
           "W [number of bytes]                                            :Write to disk\n");
  mem_print (g_line_out);
  sprintf (g_line_out,
           "X [start address] [finish address]    :Display Selected 16K ram-page Contents\n\n");
  mem_print (g_line_out);
}
