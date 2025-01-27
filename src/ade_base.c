/********************************************************************************/
/************                      ade_base.c                 *******************/
/********************************************************************************/
/*          Basic-Framework source file for Z80 S100 Computer Emulator          */
/*                                     (ade)                                    */
/*                                     GPL V2                                   */
/*                       Copyright (C) 1995  Frank D. Cringle.                  */
/*                       Copyright (C) 1996-Present Jack Strangio               */
/*                                                                              */
/*                         nse consists of two parts:                           */
/*                                                                              */
/* EMU, the basic Z80 emulator which is built upon Frank Cringle's YAZE is the  */
/* basic framework simulating a Z80 S100 computer of the 80s. It contains the   */
/* Z80 CPU, the terminals, and a monitor which controls all administrative      */
/* actions, such as mounting and unmounting disk images, setting break points,  */
/* debug levels, and other functions such as CapsLock, etc.                     */
/*                                                                              */
/* Also there is NSE itself emulates the specific S100 machine which was the    */
/* North Star Horizon, which had single and double-density North Star floppy    */
/* disk controllers and a 5-inch North Star hard disk, together with the        */
/* motherboard substrate which contained two serial i/o ports and one parallel  */
/* i/o port.                                                                    */
/*                                                                              */
/********************************************************************************/

/********************************************************************************/
/*   Parts of this file are from yaze - yet another Z80 emulator.               */
/*   This file is part of nse  - North Star Horizon emulator.                   */
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


#ifndef LIBDIR
#define LIBDIR "/usr/local/lib/"
#endif

#include "ade.h"
#include "ade_extvars.h"
#include <time.h>



/* getopt declarations */
//extern int getopt ();
//extern char *optarg;
//extern int optind, opterr, optopt;
//extern int nanosleep ();


void
get_environment ()
{
  strcpy (g_env_home, getenv ("HOME"));
  strcpy (g_env_logname, getenv ("LOGNAME"));
  strcpy (g_env_pwd, getenv ("PWD"));
  strcpy (g_env_shell, getenv ("SHELL"));
}


int
prepare_emulator (void)
{
  int error = 0;

  g_coldboot_flag = TRUE;

  if (g_machine_floppy_max)
    {
      (void) disk_manager (INITIALISE, 0, NULL, 0, 0);
    }

  g_ascii_in = NULL;		/* ascii_input file pointer. not in use at startup */
  g_native_flags = 0;
  g_loadadr = -1;

  g_row = 0;
  g_col = 0;
  if (error)
    {
      usage ();
      exit (22);
    }
  return (0);
}

void
initialise_cpu_structure ()
{
  int j;
  g_cpux = (&g_cpu);
  g_logfile = NULL;
  g_cpux->break_address = 0x1ffff;	/* set break address to roll-over point at top of memory */
  g_cpux->trap_address = 0x0ffff;	/* DEFAULT TRAP ADDRESS RIGHT AT TOP OF MEMORY */
  for (j = 0; j < 2; j++)
    {
      g_cpux->af[j] = 0x044;	/* accumulator with 00H has Zero and Parity flags set */
      g_cpux->regs[j].bc = 0;
      g_cpux->regs[j].de = 0;
      g_cpux->regs[j].hl = 0;
    }
  g_cpux->ix = 0;
  g_cpux->iy = 0;
  g_cpux->prom_base = 0;
  g_cpux->prom_end = 0;
  g_cpux->rom_base = 0;
  g_cpux->rom_end = 0;
  g_cpux->pc = g_cpux->prom_base;
}

int job;


int
run (gint * alpha)
{
  int s_ctr = 1000;
  int i;

  UNUSED (alpha);
  if (!g_stopsim)
    {
      s_ctr = RUN_CYCLES;
      while (s_ctr)
	{
	  loop_z80 (g_cpux->pc);
	  s_ctr--;
	}

    }
  else
    {
      for (i = 0x00; i < ADVANTAGE_VID_RAM_LEN; i++)
	{

	  g_vr_byte = *(g_videoram + i);	// get 8 pixels ; 8 bits of videoram[i]
	  g_display_pixel_ptr = (g_display_buffer + (g_xlt_addr[i]));
	  //display_pixel_ptr points to first of 24 bytes in pb24_data
	  expand_bits_to_3_bytes (g_display_pixel_ptr, g_vr_byte);
	}

    }

  return (TRUE);
}

int
xscreen (gint * alpha)
{
  UNUSED (alpha);
  get_rgb_pixbuf_data (g_scanline);
  gtk_image_set_from_pixbuf (GTK_IMAGE (g_ade_win), gdk_pixbuf_new_from_data (g_rgb_pixbuf_data, GDK_COLORSPACE_RGB,	// colorspace (must be RGB)
									    0,	// has_alpha (0 for no alpha)
									    8,	// bits-per-sample (must be 8)
									    ADV_SCREEN_COLS * g_x_dots_per_pixel,	// 640 *1 cols
									    ADV_SCREEN_ROWS * g_y_dots_per_pixel,	// 240 * 2 rows
									    (ADV_SCREEN_COLS * g_x_dots_per_pixel * RGB_BYTES_PER_PIXEL),	// rowstride
									    NULL, NULL	// destroy_fn, destroy_fn_data
			     ));
  return (TRUE);
}


gint
input_idle (int *alpha)
{
/* limit number of input loops*/
  int j;

  UNUSED (alpha);

  if (siotab[SIO_CARD_IN].fp != NULL)
    {
      for (j = 0; j < 32; j++)
	{
	  sio_input_thread ();
	}
    }

  if (siotab[PIO_CARD_IN].fp != NULL)
    {
      for (j = 0; j < 32; j++)
	{
	  pio_input_thread ();
	}
    }
  return (TRUE);
}









/* Usage - displayed on aborted startup*/
void
usage (void)
{
  fprintf (stderr,
	   "Usage: %s {flags} {commands}\n"
	   "           -b <file>       boot using disk image file\n"
	   "           -c <file>       rc_config configuration file (default= '.ade_rc')\n"
	   "           -v              verbose startup information\n",
	   g_eprogname);
  exit (1);
}


/* Enable/Disable timer_interrupt */
void
set_timer_interrupt_active (int active)
{
  if (active)
    {
      g_timer_interrupt_active = TRUE;
    }
  else
    {
      g_timer_interrupt_active = FALSE;
    }
}


BYTE
GetBYTE (WORD ram_addr)
{
  int xmem;

  ram_addr &= 0xffff;
  xmem = extended_ram_address (ram_addr);
  return (g_ram[xmem]);
}


unsigned int
extended_ram_address (WORD ram_addr)
{
  int xmem;
  int ram_page;

  ram_page = ram_addr / 0x4000;
  xmem = g_memory_mapping_register[ram_page];
  xmem = xmem + (ram_addr % 0x4000);
  return (xmem);
}


void
PutBYTE (WORD ram_addr, BYTE v)
{
  int xmem;
  int memslice;

  xmem = extended_ram_address (ram_addr);
  memslice = xmem / 0x4000;

  switch (memslice)
    {
    case 0xc:
    case 0xd:			/* prom - can't be written on */
    case 0xe:
    case 0xf:
      xlog (MEM, "Can't Write to PROM memory %05X\n", xmem);
      break;

    case 0xa:			/* unused video ram can't be written on */
    case 0xb:
      break;
    case 0x04:			/* Second 64K of Advantage RAM not used */
    case 0x05:
    case 0x06:
    case 0x07:
      break;
    case 0x08:			/* Normal Video RAM */
    case 0x09:
      g_ram[xmem] = v;
      output_vbytes (xmem, v);
      break;
    case 0:			/* First 64K of Advantage RAM Memory */
    case 1:
    case 2:
    case 3:
      g_ram[xmem] = v;
      break;
    default:
      xlog (MEM, "ERROR. Bad 4K page of RAM selected\n");
      break;
    }
}


void
PutWORD (WORD ram_addr, WORD v)
{
  PutBYTE (ram_addr, v & 0xff);
  PutBYTE (((ram_addr + 1) & 0x0ffff), v / 0x100);
}


/***************************************************************************/
/******** Z80-OS-INTERFACE     SECTION *************************************/
/***************************************************************************/


void
z80_os_interface (WORD * xaf, WORD * xbc, WORD * xde, WORD * xhl,
		  WORD * xix, WORD * xiy, U_INT * xpc, WORD * xsp)
{
/* Interface between emulated machine and the host OS. The type of command */
/* specified by the value in the 'C' register, with the other registers    */
/* except AF being used as arguments.                                      */
/* Return values are zero in A and the zero flag set for success, or an    */
/* error value in A with the zero flag cleared and the carry flag set.     */
  BYTE *ramptr;
  int qty;
  int offset;
/*  WORD bc; */
  BYTE b_arg;
  BYTE d_arg;
  BYTE e_arg;
  BYTE zcmd;
  int file_no;
  int file_op;
  unsigned int status;
  int error;

  UNUSED (xiy);
  UNUSED (xsp);


  /* bc = (WORD) * xbc; *//*variable bc not yet in use */
  zcmd = (WORD) * xbc & 0x0ff;
  b_arg = (WORD) (*xbc & 0x0ff00) / 256;
  e_arg = (WORD) * xde & 0x0ff;
  d_arg = (WORD) (*xde & 0x0ff00) / 256;

  switch (zcmd)
    {

    case 0:			/* Initial Load */
      /*de has number of bytes to load, hl has boot address in ram to load at */
      initial_load (xde, xhl);
      *xpc = *xhl;
      *xaf = OP_OK;
      break;
    case 1:
				  /**** FILE OPERATIONS ****/
      file_no = (b_arg & 0x07);
      file_op = (b_arg & 0x0f8) / 16;

      switch (file_op)
	{
	case 0:		/* open file */

	  ramptr = &(g_ram[(*xhl & 0x0ffff)]);
	  status = open_zo_file (file_no, ramptr);
	  if (status == 0)
	    {
	      *xaf = OP_OK;
	    }
	  else
	    {
	      *xaf = (OP_BAD | 0x0ff00);
	    }
	  break;
	case 1:		/*read from file */
	  /* hl points to ram address */
	  /* de points to number of bytes to load */

	  ramptr = &(g_ram[*xhl & 0x0ffff]);
	  qty = *xde & 0x0ffff;
	  status = fread (ramptr, 1, qty, g_zo_f[file_no]);

	  if (status)		/* chunk of qty size read */
	    {
	      *xaf = OP_OK;
	    }
	  else
	    {
	      *xaf = (OP_BAD | 0x0ff00);
	    }

/* fill last sector with CPM EOF chars */
	  for (status = status; status < (unsigned int) qty; status++)
	    {
	      *(ramptr + status) = 0x1a;
	    }
	  break;

	case 2:		/* write to file */
	  /* hl points to ram address */
	  /* de points to number of bytes to write to disk */

	  ramptr = &(g_ram[*xhl & 0x0ffff]);
	  qty = *xde & 0x0ffff;
	  status = fwrite (ramptr, qty, 1, g_zo_f[file_no]);
	  fflush (g_zo_f[file_no]);

	  if (status == 1)	/* CPM uses sector-size chunks */
	    {
	      *xaf = OP_OK;
	    }
	  else
	    {
	      *xaf = (OP_BAD | 0x0ff00);
	    }
	  break;

	case 5:		/* set file ptr at offset from start of file */

	  offset = (*xde * 0x10000) + *xhl;
	  status = fseek (g_zo_f[file_no], (long) offset, SEEK_SET);
	  if (!status)
	    {
	      *xaf = OP_OK;
	    }
	  else
	    {
	      *xaf = (OP_BAD | 0x0ff00);
	    }
	  break;

	case 6:		/* set file ptr at offset from end of file */

	  offset = (*xde * 0x10000) + *xhl;
	  status = fseek (g_zo_f[file_no], (long) offset, SEEK_END);
	  if (!status)
	    {
	      *xaf = OP_OK;
	    }
	  else
	    {
	      *xaf = (OP_BAD | 0x0ff00);
	    }
	  break;
	case 7:		/* report size of file */
	  if (g_zo_f[file_no] != NULL)
	    {

	      *xde = (g_zo_flen[file_no] / 0x10000) & 0x0ffff;	/* DE has filesize upper 16 bits */
	      *xhl = (g_zo_flen[file_no] % 0x10000) & 0x0ffff;	/* HL has filesize lower 16 bits */
	      *xaf = OP_OK;
	    }
	  else
	    {
	      *xaf = (0xff00 | OP_BAD);
	    }
	  break;

	case 0x0f:		/* close file */
	  if (g_zo_f[file_no] != NULL)
	    {
	      error = (fclose (g_zo_f[file_no]));
	    }
	  if (!error)
	    {
	      *xaf = OP_OK;
	    }
	  else
	    {
	      *xaf = (0xff00 | OP_BAD);
	    }
	  break;

	  /***************Floppy ops***************/
	case 0x41:		/*read from floppy image-file */
	  xlog (INFO,
		"z80_os_interface: read data from floppy unit %d TRACK %d SECTOR %d  TO RAM at %04X \n",
		b_arg, d_arg, e_arg, xix);
	  xlog (INFO, " ======== not yet implemented\n");
	  break;
	default:		/*file-ops */
	  xlog (INFO, "z80_os_interface: BAD file/floppy_op %d\n", file_op);
	  *xaf = (0x0ff00 | OP_BAD);
	  break;
	}
      break;
    default:			/* zo-interface */
      xlog (INFO,
	    "z80_os_iinterface: UNKNOWN interface operation: %d    [ %02X ]\n",
	    zcmd, zcmd);
      xlog (INFO, "*xaf = %08X \n", *xaf);
      *xaf = (0x0ff00 | OP_BAD);
      break;
    }

}

int
open_zo_file (int fnum, BYTE * fname)
{
  int error;
  g_zo_filename[fnum][0] = '\0';
  strcpy (g_zo_filename[fnum], (char *) fname);	/* save filename string */
  if ((g_zo_f[fnum] = fopen (g_zo_filename[fnum], "r+")) == NULL)
    {

      if ((g_zo_f[fnum] = fopen (g_zo_filename[fnum], "w")) == NULL)
	{
	  error = 2;
	  return (error);
	}
      else
	{
	  fclose (g_zo_f[fnum]);
	  if ((g_zo_f[fnum] = fopen (g_zo_filename[fnum], "rb+")) == NULL)
	    {
	      error = 2;
	      return (error);
	    }
	  else
	    {
	      error = 0;
	      return (error);
	    }
	}


    }
  else
    {
      fseek (g_zo_f[fnum], 0L, SEEK_END);
      g_zo_flen[fnum] = ftell (g_zo_f[fnum]);
      fseek (g_zo_f[fnum], 0L, SEEK_SET);
      error = 0;
    }
  return (error);
}



void
initial_load (WORD * numbytes, WORD * ramaddress)
{
  int loaded;
  xlog (INFO,
	"z80_os_interface: INITIAL LOAD: loads first %X bytes of disk 1\n into memory at %04XH",
	*numbytes, *ramaddress);
  xlog (INFO, " then jumps to instructions at %04XH.\n", *ramaddress);
  if (!disk_manager (MOUNTED, 1, NULL, 0, 0))
    {				/*NO boot disk mounted!! */
      g_interrupt_newpc = g_prom_base;	/* restart the prom (if one) */
    }
  else
    {
      loaded =
	disk_manager (BOOT, 1, NULL, (*numbytes & 0x0ffff),
		      (*ramaddress & 0x0ffff));
      if (!loaded)
	{
	  xlog (INFO, "initial_load:  Error? Not loaded?\n");
	}
      else
	{
/*	  machine_initialiser ();*/
	  g_cpux->ir = 0;		/* reset interrupt register to 0000 */
	  g_interrupt_newpc = (*ramaddress & 0x0ffff);
	  g_interrupt_req_flag = TRUE;
	  g_rom_end = 0x0ffff;
	  g_prom_base = 0x0ffff;
	}
    }
}



/****************/
/* END FILE-OPS */
/****************/


BYTE
RAM (WORD addr)
{
  BYTE data;
  data = GetBYTE (addr) & 0x0ff;
  return (data);
}

/****************************************************************/


void
set_rtc_interrupt (int a)
{

  xlog (INFO, "REAL TIME CLOCK INTERRUPT");
  if (a)
    {
      xlog (INFO, " ENABLED\n");
    }
  else
    {
      xlog (INFO, " DISABLED\n");
    }

  g_rtc_int = a;
}

void
set_keyboardin_interrupt (int a)
{

  xlog (KEYB, "KEYBOARD-IN INTERRUPT");
  if (a)
    {
      xlog (KEYB, " ENABLED\n");
    }
  else
    {
      xlog (KEYB, " DISABLED\n");
    }

  g_keybrdin_int = a;
}


void
set_hardwareint (int *hwint, int vector_num)
{
  *hwint = vector_num;
}



void
load_advantage_prom ()
{
  int i;
  int j;
  BYTE *romptr;

  romptr = g_machine_prom_code;	/* start of binary code */

  for (j = 0; j < g_machine_prom_length; j++)
    {

      *(&(g_ram[0x30000 + j])) = *romptr++;
    }
/*we have the first 0x800 bytes of prom. loaded. Now duplicate that for the full*/
/* 0x10000 bytes of RAM on the 30000-3ffff space. */

  for (i = 1; i < 32; i++)
    {
      for (j = 0; j < 0x800; j++)
	{
	  g_ram[(0x30000 + (0x0800 * i) + j)] = g_ram[0x30000 + j];
	}
    }

/* reset sends advantage address PC here*/
  g_cpu.pc = 0x08066;

  xlog (MEM, "load_advantage_rom:  64K LOADED!\n");
}
