/* This file is part of NSE - North Star Emulator
   Copyright 1995-2009 Jack Strangio.


   NSE is free software; you can redistribute it and/or modify it under
   the terms of the GNU General Public License as published by the Free
   Software Foundation; either version 2 of the License, or (at your
   option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA */

#include "ade.h"
#include "ade_extvars.h"

/* NSE contains two separate threads for the Second Serial Port IN and the    */
/*  Parallel Port IN threads. These threads look for input on those two ports */
/*  and store any chars input on to two large ring-buffers. There they stay   */
/*  until applications look for them.                                         */

#define BC  g_cpux->regs[g_cpux->regs_sel].bc


/*****************************************/
/* S2 */

BYTE
sio_buffstat (void)
{
  unsigned int next_one = 0;
  int c_ready = 0;
  BYTE c;


  if (g_sio_character_buff_ptr == NULL)
    {
      calloc_buffer_pointer ("sio_i", &g_sio_character_buff_ptr);
    }



/* save ocptr value from change, copy into next_one */
  next_one = g_sio_ocptr;

  if (g_sio_ocptr != g_sio_icptr)
    {
      /* return value of char in buffer in C register as signal that buffer */
      /* has char waiting; this can be used as part of a read-ahead function */
      next_one++;
      next_one &= PORT_IN_BUFF_MASK;

      c = *(g_sio_character_buff_ptr + next_one);
      Setlreg (BC, c);
      c_ready = TRUE;

    }
  return c_ready;
}

/* PARALLEL */

BYTE
pio_in_buffstat (void)
{
  unsigned int next_one;
  int c_ready = 0;
  BYTE c;

  if (g_pio_character_buff_ptr == NULL)
    {
      calloc_buffer_pointer ("pio_i", &g_pio_character_buff_ptr);
    }


  if (g_pio_ocptr != g_pio_icptr)
    {
      next_one = g_pio_ocptr;

      /* return value of char in buffer as signal that buffer has char waiting */
      /* this can be used as part of a read-ahead function */
      next_one++;
      next_one &= PORT_IN_BUFF_MASK;
      c = *(g_pio_character_buff_ptr + next_one);
      Setlreg (BC, c);
      c_ready = TRUE;
    }
  return (c_ready);
}

/*******************************************/

/* S2 */

BYTE
sio_buff_in (void)
{
  unsigned char c;

  while (!sio_buffstat ())
    { /* do nuthin' */ ;
    }                           /*wait for char available */

  g_sio_ocptr++;
  g_sio_ocptr &= PORT_IN_BUFF_MASK;
  c = *(g_sio_character_buff_ptr + g_sio_ocptr);
  return (c);
}

/* PARALLEL_IN */

BYTE
pio_buff_in (void)
{
  unsigned char c;

  while (!pio_in_buffstat ())
    { /* do nuthin' */ ;
    }                           /*wait for char available */

  g_pio_ocptr++;
  g_pio_ocptr &= PORT_IN_BUFF_MASK;
  c = *(g_pio_character_buff_ptr + g_pio_ocptr);
  return (c);
}

/**************************************************/

/* PARALLEL */

void
pio_input_thread (void)
{
  struct sio *s = &g_siotab[PIO_CARD_IN];
  BYTE cc = 0;
  int diff;
  int count;
  int fd;

  if (g_pio_character_buff_ptr == NULL)
    {
      calloc_buffer_pointer ("pio_in", &g_pio_character_buff_ptr);
    }

  if (g_pio_ocptr > g_pio_icptr)
    {
      diff = (g_pio_icptr + PORT_IN_BUFF_SIZE) - g_pio_ocptr;
    }
  else
    {
      diff = g_pio_icptr - g_pio_ocptr;
    }


  if (diff > (PORT_IN_BUFF_SIZE - 10))
    {
      xlog (ALL,
            "STALLING TILL CATCH-UP:   icptr = %4d    ocptr = %4d \n",
            g_pio_icptr, g_pio_ocptr);
    }
  else
    {
      if (s->fp == NULL)        /* no file */
        {
          xlog (ALL,
                "pio_thread:  s->fp is not valid, no file. No char input.\n");
        }
      else
        {
          fd = fileno (s->fp);
          count = 0;
          count = read (fd, &cc, 1);
          if (count > 0)
            {
              g_pio_icptr++;
              g_pio_icptr &= PORT_IN_BUFF_MASK;

              *(g_pio_character_buff_ptr + g_pio_icptr) = cc;

              Setlreg (BC, cc);
            }
        }

    }
}


/*****************************************/
/* SIO HARWARE INPUT */

void
sio_input_thread (void)
{
  struct sio *s = &g_siotab[SIO_CARD_IN];
  BYTE cc = 0;
  int diff;
  int count;
  int fd;



  if (g_sio_character_buff_ptr == NULL)
    {
      calloc_buffer_pointer ("sio_in", &g_sio_character_buff_ptr);
    }

  if (g_sio_ocptr > g_sio_icptr)
    {
      diff = (g_sio_icptr + PORT_IN_BUFF_SIZE) - g_sio_ocptr;
    }
  else
    {
      diff = g_sio_icptr - g_sio_ocptr;
    }
//   xlog (ALL, "sio_input_thread: diff = %d\n");



  if (diff > (PORT_IN_BUFF_SIZE - 10))
    {
      xlog (ALL,
            "STALLING TILL CATCH-UP:   icptr = %4d    ocptr = %4d \n",
            g_sio_icptr, g_sio_ocptr);
    }
  else
    {
      if (s->fp == NULL)        /* no file */
        {
          xlog (ALL,
                "sio_thread:  s->fp is not valid, no file. No char input.\n");
        }
      else
        {
          fd = fileno (s->fp);
          count = 0;
          count = read (fd, &cc, 1);

          if (count > 0)        // IE NO ERROR, AND MORE THAN ZERO
            {
              g_sio_icptr++;
              g_sio_icptr &= PORT_IN_BUFF_MASK;

              *(g_sio_character_buff_ptr + g_sio_icptr) = cc;


              xlog (ALL,
                    "Char on SIO Hardware Input:  %02X  <%c>   icptr = %4d    ocptr = %4d \n",
                    cc, prn (cc), g_sio_icptr, g_sio_ocptr);
//            Setlreg (BC, cc);
            }
        }

    }
}

/*****************************************/
