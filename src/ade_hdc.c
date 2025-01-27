/******************************************************/
/**********            ade_hdc.c        ***************/
/******************************************************/
/*    North Star 5-inch Hard Disk Controller Board    */
/*           for North Star Advantage Emulator        */
/*                       (ade)                        */
/*                       GPL V2                       */
/*      Copyright (C) 1996-Present Jack Strangio      */
/*                                                    */
/******************************************************/

#include "ade.h"
#include "ade_extvars.h"




BYTE
HDC_in (BYTE port)
{
  BYTE data;
  BYTE prn;
  int i, j;

  if (g_hd_delay)
    {
      for (i = 0; i < 60; i++)
        {
          for (j = 0; j < 70; j++)
            {
              ;
            }
        }
    }
  g_p_lo = port & 0x07;
  xlog (HDC, "HDC_IN - port = %d\n", g_p_lo);

  if (g_hd5->hdd != NULL)
    {


      switch (g_p_lo)
        {
        case 0:
          /* get data from hard-disk cache RAM */

          data = g_hd5->hd_cache_ram[g_hd5->hd_ram_ptr];
          prn = '.';
          if ((data > 0x1f) && (data < 0x7f))
            {
              prn = data;
            }


          xlog (HDC,
                "HDC_IN_00: Read Data Byte from HD_RAM.  hd_ram_ptr = %03X  byte= %02X (%c)\n",
                g_hd5->hd_ram_ptr, data, prn);
          g_hd5->hd_ram_ptr++;
          break;
          break;
        case 01:
          xlog (HDC, "HDC_IN_01: Get HD Status\n");
          hard_disk_state ();
          data = mk_hd_status ();
          break;
        case 2:
          xlog (HDC, "HDC_IN_02: Clear RAM address, Unit %d\n", g_hd_unit);
          g_hd5->hd_ram_ptr = 0;
          data = 0;
          break;
        case 3:
          xlog (HDC, "HDC_IN_03: Clear Sector Flag, Unit %d\n", g_hd_unit);
          g_hd5->hd_sector_flag = FALSE;
          data = 0;
          break;
        case 4:
          xlog (HDC,
                "HDC_IN_04: Start SYNC, Unit %d  (??? Not Required in NSE)\n",
                g_hd_unit);
      /****************** NI *************/
          xlog (HDC, "DUMMY function\n");
          data = 0;
          break;
        case 5:
          xlog (HDC, "HDC_IN_05: Start READ \n");
          g_hd5->hd_state = READ_0;
          data = 0;
          break;
        case 6:
          xlog (HDC, "HDC_IN_06: Start WRITE\n");
          g_hd5->hd_state = WRITE_0;
          g_hd_sync = 0;
          data = 0;
          break;
        case 7:
          xlog (HDC,
                "HDC_IN_07: FORMAT WRITE, Unit %d (Not Required in NSE)\n",
                g_hd_unit);
          xlog (HDC, "DUMMY function\n");
          data = 0;
          break;
        default:
          xlog (HDC, "ERROR in HDC_in: ------- Command Port: %02X \n", port);
          data = 0xaa;
          break;
        }
    }
  else
    {
      data = 0XFF;
      xlog (HDC, "ERROR! NO HD AVAILABLE. Not Mounted \n");
    }
  return (data);
}


void
HDC_out (BYTE po_lo, BYTE data)
{
  BYTE prn;

  po_lo = po_lo & 0x07;
  xlog (HDC, "HDC_OUT - port(low nybble) = %d    data = %d  %02X\n", po_lo,
        data, data);

  if (g_hd5->hdd != NULL)
    {
      switch (po_lo)
        {
        case 6:
          xlog (HDC, "HDC_OUT_6: LOAD HD DRIVE CONTROL REGISTER\n");
          load_hd_drive_control_register (data);
          break;
        case 7:
          prn = '.';
          if ((data > 0x1f) && (data < 0x7f))
            {
              prn = data;
            }

          xlog (HDC,
                "HDC_OUT_07: Write Data Byte into HD_RAM.  hd_ram_ptr = %03X  byte= %02X (%c)\n",
                g_hd5->hd_ram_ptr, data, prn);

          g_hd5->hd_cache_ram[g_hd5->hd_ram_ptr] = data;
/* if start outputting data and START WRITE not already issued, clear sync, change state */
          if (g_hd5->hd_ram_ptr == 0)
            {
              g_hd_sync = 0;
              g_hd5->hd_state_counter = 1;
              g_hd5->hd_state = WRITE_1;
            }


/* look for start of writable data - position of sync-byte */
          if ((!g_hd_sync) && (g_hd5->hd_ram_ptr > 30) && (g_hd5->hd_ram_ptr < 90))
            {
              if (g_hd5->hd_cache_ram[g_hd5->hd_ram_ptr] == 0x01)
                {
                  g_hd_sync = g_hd5->hd_ram_ptr;
                  g_hd5->hd_read_write_active = TRUE;
                  xlog (HDC, "FOUND SYNC byte at CACHE[%d]\n",
                        g_hd5->hd_ram_ptr);
                }
            }
          g_hd5->hd_ram_ptr++;
          break;
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
          xlog (HDC, "Port Low Nybble = %d ####### DUMMY function #######\n");
          break;
        }
    }
  else
    {
      xlog (HDC, "ERROR No output. Harddrive not available\n");
    }
}


void
load_hd_drive_control_register (BYTE data)
{
  BYTE header_read_enable;
  BYTE temp;
  BYTE bit_s_hi;
  temp = ~data;
  g_hd5->hd_surface = temp & 0x03;
  bit_s_hi = data & 0x04;
  xlog (HDC, "    Bits 0,1:   Surface selected = %d\n", g_hd5->hd_surface);
  xlog (HDC, "    Bit  2  : =RESERVED=  extra heads bit = %d\n",
        (data & 0x04) / 4);
  if (bit_s_hi)
    {
      g_hd5->hd_surface = g_hd5->hd_surface | bit_s_hi;
      xlog (HDC,
            "NOTE: hd_surface is also using the =RESERVED= bit, so hd_surface is now %d\n",
            g_hd5->hd_surface);
    }


  xlog (HDC, "    Bit  3  :   Low Write Current  [NI]   = %d\n",
        (data & 0x08) / 8);
  g_hd5->hd_step_direction = (data & 0x20) / 32;
  g_hd5->hd_step_pulse = (data & 0x10) / 16;
  xlog (HDC, "    Bit  4  :   Head Step Pulse           = %d\n",
        g_hd5->hd_step_pulse);
  xlog (HDC,
        "    Bit  5  :   Head Step Direction       = %d   (0 = IN, 1 = OUT\n",
        g_hd5->hd_step_direction);
  xlog (HDC,
        "                (Also Write Precompensation, but Not Implemented)\n");
  if ((!g_hd5->hd_step_pulse) && (g_hd5->hd_step_pulse_prev))
    {
      xlog (HDC, "STEP \n");
      cylinder_step ();
      g_hd5->hd_seek_complete = FALSE;
    }
  g_hd5->hd_step_pulse_prev = g_hd5->hd_step_pulse;
  xlog (HDC, "    Bit  6  :   Unused\n");
  header_read_enable = (data & (BYTE) 0x80) / 128;
  xlog (HDC, "    Bit  7  :   Read Sector Header Enable = %d \n",
        header_read_enable);
  if (header_read_enable)
    {
      xlog (HDC, "Header Read Enable\n");
      copy_cache_header ();
    }

}


/*********************************************************************************/
/* CYLINDER STEP                                                                 */
/*Steps cylinder-in-use inwards (increasing track-number) or outwards (decreasing*/
/* track-number). Actual stepping occurs when step-cmd is reset (Signal goes LOW */
/* from HIGH) so we remember what previous step-cmd value was (in last_step_cmd) */
/*********************************************************************************/
void
cylinder_step (void)
{

  if (g_hd5->hd_step_direction) /* Stepping OUTWARDS */
    {
      if (g_hd5->cylinder)      /* decrease ONLY if track-number > 0 */
        {
          g_hd5->cylinder--;
          if (!g_hd5->cylinder)
            g_hd5->hd_track_zero = TRUE;        /* when get to Track 0, set Track_Zero flag */
        }
    }
  else                          /*step_dir==0 , so stepping INWARDS */
    {
      g_hd5->cylinder++;
      if (g_hd5->cylinder > g_hd5->hd_max_cylinders)
        {                       /* cylinder can't step in further than max_cylinders */
          g_hd5->cylinder = g_hd5->hd_max_cylinders;
        }
      g_hd5->hd_track_zero = FALSE;     /* if stepped IN, Track 0 flag MUST be off */
    }

  xlog (HDC, " STEP-CMD  - CYLINDER :%3d   -- SURFACE %d\n", g_hd5->cylinder,
        g_hd5->hd_surface);
  xlog (HDC, "   STEPPED TO  cylinder %d\n", g_hd5->cylinder);
  g_hd5->hd_seek_complete = FALSE;      /* delay the seek period */
  g_hdseek_delay = 4;
}




BYTE
mk_hd_status (void)
{

  BYTE hd_status = 0;           /* clear previous value of hd_status */
  if (g_hd5->hd_sector_flag)
    {
      hd_status = hd_status | (BYTE) 0x80;
    }

  if (!g_hd5->hd_index_flag)
    {
      hd_status = hd_status | 0x40;
    }

  if ((g_hd5->hd_read_write_active) || (g_hdseek_delay))
    {
      hd_status = hd_status | 0x20;
      g_hd5->hd_seek_complete = FALSE;

    }


/* HD drive ALWAYS ready */

/* bit 4 always zero */
  g_hd5->hd_drive_ready = TRUE;
  if (!g_hd5->hd_drive_ready)
    {
      hd_status = hd_status | 0x10;
    }


  if (!g_hd5->hd_drive_selected)
    {
      hd_status = hd_status | 0x08;
    }

  if (!g_hd5->hd_track_zero)
    {
      hd_status = hd_status | 0x04;
    }
  if (g_hdseek_delay)
    {
      g_hd5->hd_seek_complete = FALSE;
      g_hdseek_delay--;
    }
  else
    {
      g_hd5->hd_seek_complete = TRUE;
    }

  if ((g_hd5->hd_read_write_active) || (g_hdseek_delay))
    {
      g_hd5->hd_seek_complete = FALSE;
    }
  if (!g_hd5->hd_seek_complete)
    {
      hd_status = hd_status | 0x02;
    }

  if (!g_hd5->hd_write_fault)
    {
      hd_status = hd_status | 0x01;
    }

  show_status (hd_status);      /* temporary */
  return (hd_status);
}


void
show_status (BYTE hd_status)
{

  xlog (HDC, "HD Cyl:Surf:Sector is %d:%d:%d  - hd_status is %02X : \n",
        g_hd5->cylinder, g_hd5->hd_surface, g_hd5->hd_sector_num, hd_status);
  xlog (HDC, "+---- ---- ---- ----  ---- ---- ---- ----+\n");
  if (hd_status & (BYTE) 0x80)  /*bit 7 */
    {
      xlog (HDC, "| SF |");
    }
  else
    {
      xlog (HDC, "| sf |");
    }

  if (hd_status & 0x40)         /*bit 6 */
    {
      xlog (HDC, " if |");
    }
  else
    {
      xlog (HDC, " IF |");
    }

  if (hd_status & 0x20)
    {
      xlog (HDC, " BZ |");
    }
  else
    {
      xlog (HDC, " bz |");
    }

  if (hd_status & 0x10)
    {
      xlog (HDC, " nr |");
    }
  else
    {
      xlog (HDC, " RDY|");
    }

  if (hd_status & 0x08)
    {
      xlog (HDC, "  ds |");
    }
  else
    {
      xlog (HDC, "  DS |");
    }

  if (hd_status & 0x04)
    {
      xlog (HDC, " t0 |");
    }
  else
    {
      xlog (HDC, " TK0|");
    }

  if (hd_status & 0x02)
    {
      xlog (HDC, " sk |");
    }
  else
    {
      xlog (HDC, " SK |");
    }

  if (hd_status & 0x01)
    {
      xlog (HDC, " OK |    UNIT No. %d\n", g_hd_unit);
    }
  else
    {
      xlog (HDC, " Nok|    UNIT No. %d\n", g_hd_unit);
    }
  show_status_bin (hd_status);
  xlog (HDC, "+ 7--- 6--- 5--- 4---  3--- 2--- 1--- 0--+ \n");
}

/***************************************************************/
/* show_status_bin                                             */
/* Converts the contents of one unsigned char (BYTE) into two  */
/* 4-bit binary strings for hd-status display logging.         */
/***************************************************************/

void
show_status_bin (unsigned char a)
{
  const char *table[16] = {
    "0  | 0  | 0  | 0  |", "0  | 0  | 0  | 1  |",
    "0  | 0  | 1  | 0  |", "0  | 0  | 1  | 1  |",
    "0  | 1  | 0  | 0  |", "0  | 1  | 0  | 1  |",
    "0  | 1  | 1  | 0  |", "0  | 1  | 1  | 1  |",
    "1  | 0  | 0  | 0  |", "1  | 0  | 0  | 1  |",
    "1  | 0  | 1  | 0  |", "1  | 0  | 1  | 1  |",
    "1  | 1  | 0  | 0  |", "1  | 1  | 0  | 1  |",
    "1  | 1  | 1  | 0  |", "1  | 1  | 1  | 1  |"
  };
  xlog (HDC, "| %s  %s\n", table[a / 0x10], table[a % 0x10]);
}


 /*********************************************************************/
  /* WRITE CACHED SECTOR                                               */
  /* Parses data in the HD Controller's cache RAM, by taking the sync  */
  /* byte, then extracting the Label Header information which specifies */
  /* which disk sector the following sector data is to be written.     */
  /* Finally, the sector data is written to the specified disk sector. */
 /*********************************************************************/
void
write_cached_sector (void)
{
  int cyl_factor;
  unsigned int psector_address;
  unsigned int logical_sector, track_num;
  long file_offset;
  BYTE psector;
  unsigned int pcylinder;
  BYTE psurface;
  int error;

  display_header ("CACHED WRITE");
  psurface = g_hd5->hd_cache_ram[g_hd_sync + HD_HDR_SURFACE];
  psurface = psurface & 0x07;
  psector = g_hd5->hd_cache_ram[g_hd_sync + HD_HDR_PHYS_SECTOR] & 0x0f;
  cyl_factor = (g_hd5->hd_cache_ram[g_hd_sync + HD_HDR_PHYS_SECTOR] & 0x30) * 16;
  pcylinder = g_hd5->hd_cache_ram[g_hd_sync + HD_HDR_CYLINDER] | cyl_factor;
  track_num = (pcylinder * (g_hd5->hd_max_heads + 1)) + psurface;

/* The logical_sector is the absolute numbered sector (disk address) which */
/* disregards things like surfaces, cylinders, and disk skew.              */
/* The track_sector is the logical sector number on each cylinder-surface, */
/* (0-15), it doesn't take skew into account. The psector (physical sector)*/
/* is the actual position on the track when we take skew into account.     */
/*                                                                         */
/* logical sectors:   00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15      */
/* physical sectors:  00 09 02 11 04 13 06 15 08 01 10 03 12 05 14 07      */
  psector_address = (track_num * 16) + psector;
  copy_write_cache (psector_address);
  xlog (HDC, "psector_address = %5d  ", psector_address);
  if (psector % 2)
    {
      logical_sector = (psector + 8) % 16;
    }
  else
    {
      logical_sector = psector;
    }


  xlog (HDC,
        "WRITE_CACHED_SECTOR:wrirpcylinder: %d   cyl_factor: %04x  psurface: %d  psector: %5d , track_num=%4d log_sect= %5d\n",
        pcylinder, cyl_factor, psurface, psector, track_num, logical_sector);
  logical_sector += track_num;
  file_offset = ((long) (psector_address) * (long) DSECTOR);
  xlog (HDC, "write_cached_sector: file_offset= %08lx    =sect %5d\n",
        file_offset, (int) (file_offset / 0x200L));
  error = fseek (g_hd5->hdd, file_offset, SEEK_SET);
  if (error == -1)
    {
      xlog (HDC,
            "Write Cached Sector: Can't seek to %ld on hard-disk image %s \n",
            file_offset, g_hd5->hdfn);
    }
  error = (int) fwrite (&g_hd5->hd_cache_ram[g_hd_sync + 10], 512, 1, g_hd5->hdd);
  if (error == 0)
    {
      xlog (HDC,
            "hdc: Write Sector: Can'twrite at %ld on hard-disk image %s \n",
            file_offset, g_hd5->hdfn);
    }


}


void
copy_sector_to_ram (void)
{
  int physical_sector_num;
  int shifted_track;
  int i;
  int k;
  shifted_track =
    (((g_hd5->cylinder * (g_hd5->hd_max_heads + 1)) + g_hd5->hd_surface) * 16);
  physical_sector_num = g_hd5->hd_sector_num;
  physical_sector_num += shifted_track;
  i = physical_sector_num;
  for (k = 0; k < 0x20f; k++)
    {
      g_hd5->hd_cache_ram[k] = g_hd5->hd_sectorc[i][k];
    }

  g_hd5->hd_ram_ptr = 0x20f;    /* we've entered 512+14 bytes into cache RAM, so set pointer to next */
}



void
copy_write_cache (int diskadd)
{
  int j;
  g_hd5->hd_sectorc[diskadd][0] = 0;
  for (j = 1; j < 0x020e; j++)
    {
      g_hd5->hd_sectorc[diskadd][j] = g_hd5->hd_cache_ram[g_hd_sync + j];
      if (j < 12)
        {
          xlog (HDC, "Copying WRITE Cache  %03X   data %02X\n", j,
                g_hd5->hd_cache_ram[g_hd_sync + j]);
        }
    }

  xlog (HDC, "AFTER WRITE_SECTOR to hd5->hd_sectorc cache %d     [ %04X ]\n",
        diskadd, diskadd);
  xlog (HDC, " st  phy  cyl  hed  lsl  lsh  stl  sth  crc  crc~\n");
  xlog (HDC,
        " %02X   %02X   %02X   %02X   %02X   %02X   %02X   %02X   %02X   %02X  \n\n",
        g_hd5->hd_sectorc[diskadd][0], g_hd5->hd_sectorc[diskadd][1],
        g_hd5->hd_sectorc[diskadd][2], g_hd5->hd_sectorc[diskadd][3],
        g_hd5->hd_sectorc[diskadd][4], g_hd5->hd_sectorc[diskadd][5],
        g_hd5->hd_sectorc[diskadd][6], g_hd5->hd_sectorc[diskadd][7],
        g_hd5->hd_sectorc[diskadd][8], g_hd5->hd_sectorc[diskadd][9]);
}




void
copy_cache_header (void)
{
  unsigned int physical_sector_num;
  unsigned int shifted_track;
  unsigned int i;
  unsigned int k;

  shifted_track =
    (((g_hd5->cylinder * (g_hd5->hd_max_heads + 1)) + g_hd5->hd_surface) * 16);
  xlog (HDC,
        "copy_cache_header: cylinder=%d  max_heads=%d  surface=%d  shifted-track=%04X\n",
        g_hd5->cylinder, (g_hd5->hd_max_heads + 1), g_hd5->hd_surface,
        shifted_track);
  if (g_hd5->cylinder > g_hd5->hd_max_cylinders)
    {
      g_hd5->cylinder = g_hd5->hd_max_cylinders;
    }
  physical_sector_num = g_hd5->hd_sector_num;
  physical_sector_num += shifted_track;
  i = physical_sector_num;
  /* we can't try to xfer a header which has a sector-number and track number  greater than */
  /* the maximum sector number and maximum track number on the disk */
  /* so restrict header xfer to max-sector-number?? */

  xlog (HDC,
        " we are looking to xfer our cached premade header for sector %04X\n",
        physical_sector_num);
  xlog (HDC, " st  phy  cyl  hed  lsl  lsh  stl  sth  crc  crc~\n");
  xlog (HDC,
        " %02X   %02X   %02X   %02X   %02X   %02X   %02X   %02X   %02X   %02X  \n",
        g_hd5->hd_sectorc[i][0], g_hd5->hd_sectorc[i][1], g_hd5->hd_sectorc[i][2],
        g_hd5->hd_sectorc[i][3], g_hd5->hd_sectorc[i][4], g_hd5->hd_sectorc[i][5],
        g_hd5->hd_sectorc[i][6], g_hd5->hd_sectorc[i][7], g_hd5->hd_sectorc[i][8],
        g_hd5->hd_sectorc[i][9]);
  for (k = 0; k < 11; k++)
    {
      g_hd5->hd_cache_ram[k] = g_hd5->hd_sectorc[i][k];
    }

  g_hd5->hd_ram_ptr = 10;               /* we've entered 10 bytes into cache RAM, so set pointer to next */
  if (g_ade_debug & HDC)
    {
      display_header ("READ");
    }
}


void
display_header (const char *info)
{

  xlog (HDC, "              %s HEADER\n", info);
  xlog (HDC, "  ST   PHY  CYL  HED  SNl  SNh  SNl  SNh  CRC  CRC~\n");
  xlog (HDC,
        "  %02X   %02X   %02X   %02X   %02X   %02X   %02X   %02X   %02X   %02X \n",
        g_hd5->hd_cache_ram[g_hd_sync + HD_HDR_START],
        g_hd5->hd_cache_ram[g_hd_sync + HD_HDR_PHYS_SECTOR],
        g_hd5->hd_cache_ram[g_hd_sync + HD_HDR_CYLINDER],
        g_hd5->hd_cache_ram[g_hd_sync + HD_HDR_SURFACE]
        /* or 'head number' */ ,
        g_hd5->hd_cache_ram[g_hd_sync + HD_HDR_LOG_SECTOR_LO],
        g_hd5->hd_cache_ram[g_hd_sync + HD_HDR_LOG_SECTOR_HI],
        g_hd5->hd_cache_ram[g_hd_sync + HD_HDR_SHIFT_TRACK_LO],
        g_hd5->hd_cache_ram[g_hd_sync + HD_HDR_SHIFT_TRACK_HI],
        g_hd5->hd_cache_ram[g_hd_sync + HD_HDR_CRC],
        g_hd5->hd_cache_ram[g_hd_sync + HD_HDR_CRC_CHECK]);
}




/************************************************************************/
/**********************Hard-Drive Disk-Image Management******************/
/************************************************************************/

/* mount North Star Hard Disk image-file into ade */

void
hdmount (const char *filename)
{
  int flen;
  int fatal;
  char filemode[4];
  struct stat st;
  char hdfilename[70];
  BYTE hdlabel[128];


  xlog (HDC, "we want to mount hard drive\n");
  g_hd5 = (&g_nshd);
  *hdfilename = '\0';           /* make it an empty string */
  fatal = 0;
  if (*filename != '/')
    {                           /* Not an absolute F/N, must be relative */
      /* so need to include where we are in absolute terms */
      strcpy (hdfilename, g_env_pwd);
      if ((strlen (hdfilename)) > 1)
        {
          strcat (hdfilename, "/");
        }
    }
  strcat (hdfilename, filename);
  xlog (HDC, "hdfilename is %s\n", hdfilename);
  stat (hdfilename, &st);
  if (!S_ISREG(st.st_mode))
    {
      printf ("   No File '%s'\n", hdfilename);
      xlog (HDC, "  No File '%s'\n", hdfilename);
    }



  strcpy (filemode, "rb+");
  if (g_hd5->hdd != NULL)
    {
      printf ("   *** Hard Drive is already mounted. Unmount first! ***\n");
    }
  else
    {

      if ((g_hd5->hdd = fopen (hdfilename, filemode)) == NULL)
        {
          printf ("   *** Can't Mount hard Drive, File < %s > ***\n",
                  hdfilename);
        }
      else
        {
          strcpy (g_hd5->hdfn, hdfilename);
          fseek (g_hd5->hdd, 0L, SEEK_SET);
          (void) fread (hdlabel, 128, 1, g_hd5->hdd);   /* get hd label area: contains disk validation */
          /* North Star hard Disks begin with 2 bytes which are always 00h and FFh */
          if ((hdlabel[0] != 0x00) || (hdlabel[1] != 0xff))
            {
              fatal = 1;
            }
          else
            {
              fatal = 0;
            }


          if (fatal)
            {
              /* We could leave the invalid disk image mounted and then format  */
              /* it. While this would make life easier, it's safer to refuse to */
              /* mount a file which could be something other than a N* hard-disk */
              /* image file, and to insist on a new image-file manufactured     */
              /* with 'mkhd'.                                                   */

              printf ("   ERROR! %s may not be a NSE Hard-Disk!\n",
                      hdfilename);
              hdumount ();      /* refuse to mount invalid image-file */
            }
          else
            {

              fseek (g_hd5->hdd, 0L, SEEK_END); /*set file ptr to end of disk image */
              flen = ftell (g_hd5->hdd);
              fseek (g_hd5->hdd, 0L, SEEK_SET); /*reset file ptr to start if disk image */
              get_hd_info (hdlabel);    /* then place them into nshd structure   */
              sprintf (g_vstring, "\nMounted  HARD DISK <%s> %ldK", hdfilename,
                       (long) (flen / 1024));
              status_print (g_vstring, 0);
              strcpy (g_hd5->hdfn, hdfilename);
              /* set up hdc disk properties */
              xlog (HDC, "Mounted Hard Disk <%s>  size = %ldK\n",
                    g_hd5->hdfn, (long) (flen / 1024));
              g_hd5->hd_max_sectors = (hdlabel[40] * 256) + hdlabel[39];
              if ((hdlabel[0] != 0) || (hdlabel[1] != 0xff))
                {
                  xlog (HDC, "\nHard Disk <%s> has  no valid Label,\n",
                        g_hd5->hdfn);
                  xlog (HDC, " so \'hd_max_sectors\' size is incorrect.\n");
                }
              else
                {
                  xlog (HDC, ", hd_max_sectors = %d \n", g_hd5->hd_max_sectors);
                }

              g_hd5->hd_sector_num = 14;
              g_hd5->hd_state = SECTOR_0;
              g_hd5->hd_ram_ptr = 0;
              g_hd5->hd_step_direction = 0;
              g_hdseek_delay = 0;
              g_hd5->hd_step_pulse = 0;
              /* initial (before use) hard-disk status bit values */
              g_hd5->hd_sector_flag = FALSE;    /* status-bit 7 is OFF */
              g_hd5->hd_index_flag = FALSE;     /* status-bit 6 is ON  */
              g_hd5->hd_read_write_active = FALSE;      /* status-bit 5 is OFF */
              g_hd5->hd_drive_ready = TRUE;     /* status_bit 4 is OFF */
              g_hd5->hd_drive_selected = TRUE;  /*status-bit 3 is OFF  */
              {
                g_hd5->cylinder = 0;
                g_hd5->hd_track_zero = TRUE;    /* status-bit 2 is OFF  */
              }
              g_hd5->hd_seek_complete = TRUE;   /* status-bit 1 is OFF */
              g_hd5->hd_write_fault = FALSE;    /* status-bit 0 is ON  */
            }
        }
    }
}


/********************************************************/
/* HDUMOUNT                                             */
/* Unmounts a hard-disk image file. The same or another */
/* hard-disk image-file can be remounted afterwards if  */
/* the 'hdmount' command is used.                       */
/********************************************************/
void
/* unmount a HD disk-image from ade */
hdumount (void)
{

  g_hd5 = (&g_nshd);
  if (g_hd5->hdd != NULL)
    {
      fclose (g_hd5->hdd);
    }
  g_hd5->hdd = NULL;
  /* finishing values for hard disk status bits */
  g_hd5->hd_sector_flag = OFF;  /*status-bit 7 */
  g_hd5->hd_index_flag = OFF;   /*status-bit 6 */
  g_hd5->hd_read_write_active = FALSE;  /*status-bit 5 */
  g_hd5->hd_drive_ready = FALSE;        /*status-bit 4 */
  g_hd5->hd_drive_selected = FALSE;     /*status-bit 3 */
  g_hd5->hd_track_zero = FALSE; /*status-bit 2 */
  g_hd5->hd_seek_complete = FALSE;      /*SPEED_NOT_READY *//*status-bit 1 */
  g_hd5->hd_write_fault = TRUE; /*status-bit 0 */
  g_hd5->hdfn[0] = '\0';
/*  free_sector_cache (hd); */
}

/****************************************************************/
/* GET HD INFO                                                  */
/* Picks out values from the N* Hard-Disk Label area at the     */
/* start of the N* hard-disk image-file and uses them to obtain */
/* the working parameters of the hard-disk image. The image-file*/
/* should have been manufactured by the 'mkhd' utility which    */
/* can provide image-files of many of the standard N* hard disks*/
/****************************************************************/
void
get_hd_info (BYTE * hdlabel)
{
  unsigned int msectors;
  unsigned int mheads;
  unsigned int mcyls;
  unsigned int csectors;
  unsigned int flen;
  unsigned int fsectors;
  unsigned int tsectors;
  unsigned int xsectors;        /*largest number of possible sectors */

  g_hd5 = (&g_nshd);
  msectors = hdlabel[DLDSZE] + (256 * hdlabel[DLDSZE + 1]);
  mheads = hdlabel[DLMXH];
  if (!mheads)
    mheads = 3;                 /* default is 4 heads (disk = SG5A) if no info */
  mcyls = hdlabel[DLMXC] + (256 * hdlabel[DLMXC + 1]);
  if (!mcyls)
    mcyls = 152;                /* default is 152 (disk=SG5A) if no info */
  g_hd5->hd_max_sectors = msectors;
  g_hd5->hd_max_heads = mheads;
  g_hd5->hd_max_cylinders = mcyls;
  g_hd5->hd_total_cylinders = hdlabel[DLOFC] + (256 * hdlabel[DLOFC + 1]);
/* N* HD always has 16 sectors/track */
  fseek (g_hd5->hdd, 0L, SEEK_END);
  flen = (int) ftell (g_hd5->hdd);
  fseek (g_hd5->hdd, 0L, SEEK_SET);
  csectors = (mcyls + 1) * (mheads + 1) * 16;
  tsectors = g_hd5->hd_total_cylinders * (mheads + 1) * 16;
  fsectors = flen / 512;        /*max physical sectors capable on diskfile */
  xlog (HDC,
        "Hard_disk: USABLE  size = %d cylinders,       %d heads, %d max sectors \"per label\" %d max calc sectors  %d maxsectors/filesize\n",
        mcyls + 1, mheads + 1, msectors, csectors, fsectors);
  xlog (HDC,
        "Hard_disk: TOTAL   size = %d total cylinders, %d heads, %d  TOTAL sectors  \" per safe shipping cylinders\"\n",
        g_hd5->hd_total_cylinders + 1, mheads + 1, tsectors);

/*find largest sectors-number available*/
  xsectors = msectors;
  if (tsectors > xsectors)
    {
      xsectors = tsectors;
    }
  if (fsectors > xsectors)
    {
      xsectors = fsectors;
    }
/* use largest number for pre-made sector headers */
  load_hd_cache (xsectors);
}




void
load_hd_cache (int num_sectors)
{
  int i;
  int heads;
  int cylinders;
  g_hd5 = &(g_nshd);
  heads = g_hd5->hd_max_heads + 1;
  cylinders = g_hd5->hd_max_cylinders + 1;
  xlog (HDC, "HD  has %d heads and %d cylinders\n", heads, cylinders);
  g_hd5->hd_sectorc = malloc (num_sectors * sizeof (char *));
  if (g_hd5->hd_sectorc == NULL)
    {
      xlog (HDC, "out of memory: sector-pointers\n");
    }
  else
    {
      xlog (HDC, "Allocated %d sector-pointers\n", num_sectors);
    }

  for (i = 0; i < num_sectors; i++)
    {
      g_hd5->hd_sectorc[i] = malloc (CACHE_SIZE * sizeof (char));
      mk_header (i, heads);
      ld_sectors (i);
      calc_cache_crc (i);
      if (g_hd5->hd_sectorc[i] == NULL)
        {
          xlog (HDC, "out of memory: sector-cache\n");
        }
    }
}


void
mk_header (int sector_number, int heads)
{

  int ccyl;
  BYTE cylfactor;
  BYTE cyllo;
  BYTE phys;
  int logical;
  WORD shftrk;
  BYTE chead;
  phys = sector_number % 16;
  shftrk = sector_number & 0x0fff0;
  logical = phys;               /*anticipate evens */
  if (phys % 2)
    {
      logical = (logical + 8) % 16;
    }
  logical = logical + shftrk;
  chead = (sector_number % (16 * heads)) / 16;
  ccyl = sector_number / (16 * heads);  /*current cylinder */
  cylfactor = (BYTE) ((ccyl & 0x0300) / 16);
  cyllo = ccyl & 0x0ff;
  g_hd5->hd_sectorc[sector_number][0] = 0;
  g_hd5->hd_sectorc[sector_number][1] = phys | cylfactor;
  g_hd5->hd_sectorc[sector_number][2] = cyllo;
  g_hd5->hd_sectorc[sector_number][3] = chead;
  if ((sector_number < 16) || (sector_number > g_hd5->hd_max_sectors))
    {
      g_hd5->hd_sectorc[sector_number][3] = chead | 0x080;
    }
  g_hd5->hd_sectorc[sector_number][4] = (BYTE) (logical & 0x0ff);
  g_hd5->hd_sectorc[sector_number][5] = (BYTE) (logical / 256);
  g_hd5->hd_sectorc[sector_number][6] = (BYTE) (shftrk & 0x0ff);
  g_hd5->hd_sectorc[sector_number][7] = (BYTE) (shftrk / 256);
  g_hd5->hd_sectorc[sector_number][8] =
    (BYTE) ((g_hd5->hd_sectorc[sector_number][1] +
             g_hd5->hd_sectorc[sector_number][2] +
             g_hd5->hd_sectorc[sector_number][3] +
             g_hd5->hd_sectorc[sector_number][4] +
             g_hd5->hd_sectorc[sector_number][5] +
             g_hd5->hd_sectorc[sector_number][6] +
             g_hd5->hd_sectorc[sector_number][7]) & 0x0ff);
  g_hd5->hd_sectorc[sector_number][9] =
    (BYTE) (0x0ff - g_hd5->hd_sectorc[sector_number][8]);


/*     ONLY DISPLAY IF HAVING A PROBLEM PREALLOCATIING SECTOR HEADER INFORMATION - very bulky info - */

/*  xlog (HDC, " st  phy  cyl  hed  lsl  lsh  stl  sth  crc  crc~  unit  Sector %d  [ %04X ]\n", sector_number, sector_number);
  xlog (HDC,
        " %02X   %02X   %02X   %02X   %02X   %02X   %02X   %02X   %02X   %02X    %d\n",
        hd5->hd_sectorc[sector_number][0], hd5->hd_sectorc[sector_number][1], hd5->hd_sectorc[sector_number][2],
        hd5->hd_sectorc[sector_number][3], hd5->hd_sectorc[sector_number][4], hd5->hd_sectorc[sector_number][5],
        hd5->hd_sectorc[sector_number][6], hd5->hd_sectorc[sector_number][7], hd5->hd_sectorc[sector_number][8], hd5->hd_sectorc[sector_number][9], unsector_numbert);
*/
}


void
ld_sectors (int i)
{
  FILE *hd;
  int hok, error;
  hd = g_nshd.hdd;
  error = (int) fseek (hd, (long) (i * 0x200), SEEK_SET);
  if (error)
    {
      xlog (HDC, "READ ERROR seeking on disk %s at sector %d\n", g_nshd.hdfn,
            i);
    }

  hok = fread (&(g_hd5->hd_sectorc[i][10]), 0x200, 1, hd);
  if (!hok)
    {
      xlog (HDC, "READ ERROR loading disk %s at sector %d\n", g_nshd.hdfn, i);
    }

}



void
calc_cache_crc (int i)
{

  int j;
  int ccrc = 0;
  BYTE ccrc_lo;
  BYTE ccrc_hi;
  for (j = 10; j < 0x20a; j++)
    {

      ccrc += g_hd5->hd_sectorc[i][j];
    }
  ccrc_lo = ccrc % 0x100;
  ccrc_hi = ccrc / 0x100;
  g_hd5->hd_sectorc[i][0x20a] = ccrc_hi;
  g_hd5->hd_sectorc[i][0x20b] = ccrc_lo;
  g_hd5->hd_sectorc[i][0x20c] = (BYTE) (0x0ff - ccrc_hi);
  g_hd5->hd_sectorc[i][0x20d] = (BYTE) (0x0ff - ccrc_lo);
}



/* display a mount table entry */
void
hdshowdisk (void)
{
#ifdef HD
  if (g_nshd.hdd == NULL)
    {
      printf ("  Hard Disk   is   ** not mounted. **\n");
    }
  else
    {
      printf ("  Hard Disk   is <%s>\n", g_nshd.hdfn);
    }
#endif
}



void
initialise_hard_disk_structure (void)
{
/* Only ONE Hard Drive, not yet anything mounted on it*/
  g_hd5 = (&g_nshd);
  g_hd5->hdd = NULL;
  /* finishing values for hard disk status bits */
  g_hd5->hd_sector_flag = OFF;  /*status-bit 7 */
  g_hd5->hd_index_flag = OFF;   /*status-bit 6 */
  g_hd5->hd_read_write_active = FALSE;  /*status-bit 5 */
  g_hd5->hd_drive_ready = H_4_NOT_READY;        /*status-bit 4 */
  g_hd5->hd_drive_selected = FALSE;     /*status-bit 3 */
  g_hd5->hd_track_zero = FALSE; /*status-bit 2 */
  g_hd5->hd_seek_complete = FALSE;      /*status-bit 1 */
  g_hd5->hd_write_fault = TRUE; /*status-bit 0 */
}



/****************************************************************/
/* HARD DISK STATE                                              */
/* Finite state machine which determines where in a single disk */
/* rotation the hard disk is, whether it's just spinning, or    */
/* whether it's in a write or read operation. The finite state  */
/* machine is held back by setting delay-values which are       */
/* counted down before allowing the machine to move along.      */
/* These values can be adjusted so that the sector-timing code  */
/* in the Level 1 Test comes out correctly, but I have set the  */
/* values to give a faster hard drive.                          */
/****************************************************************/
void
hard_disk_state (void)
{
  int logical_sector_num;
  int track_sector;
  switch (g_hd5->hd_state)
    {
/******************************/
/* Start of track at sector 0 */
/******************************/
    case INDEX_0:


      g_hd5->hd_index_flag = TRUE;      /*index flag  ACTIVE LOW */
      copy_cache_header ();
      /*read_cache_header ();      *//* tells which sector we're on */
      g_hd5->hd_read_write_active = FALSE;      /*start of sector, rwbusy goes low */
      g_hd5->hd_state = INDEX_1;
      g_hd5->hd_state_counter = 2;      /*two */
/****************** this stuff here is just for debug logging info */
      track_sector = g_hd5->hd_sector_num;
      if (track_sector % 2)
        {                       /* if odd sector need to add skew */
          track_sector = ((track_sector + 8) % 16);
        }
      logical_sector_num =
        (g_hd5->cylinder * (g_hd5->hd_max_heads + 1) * 16) +
        (g_hd5->hd_surface * 16) + track_sector;
      xlog (HDC,
            "########  Cylinder:%4d   Head: %1d   Sector:%2d  DiskAddress:%5d   ####\n",
            g_hd5->cylinder, g_hd5->hd_surface, g_hd5->hd_sector_num,
            logical_sector_num);
/*****************/
      break;
    case INDEX_1:
      g_hd5->hd_state_counter--;
      if (!g_hd5->hd_state_counter)
        {
          g_hd5->hd_index_flag = FALSE; /* set index flag INACTIVE low */
          g_hd5->hd_sector_flag = TRUE; /* sector flag ACTIVE HIGH */
          g_hd5->hd_state = INDEX_2;
          g_hd5->hd_state_counter = 2;
          break;
        }
      break;
    case INDEX_2:
      g_hd5->hd_state_counter--;
      if (!g_hd5->hd_state_counter)
        {
          g_hd5->hd_read_write_active = FALSE;  /*start of sector, rwbusy goes low */
          g_hd5->hd_state = INDEX_3;
          break;
        }
      break;
    case INDEX_3:
      g_hd5->hd_state = SECTOR_1;
      g_hd5->hd_state_counter = 2;
      break;
/***************************************/
/* Start of sectors 1 to 15 begins here - main sequence*/
/***************************************/
    case SECTOR_0:
      g_hd5->hd_sector_flag = TRUE;     /* sector flag is active HIGH */
      g_hd5->hd_read_write_active = FALSE;      /*start of sector, rwbusy goes low */
      copy_cache_header ();
      g_hd5->hd_state_counter = 2;
      g_hd5->hd_state = SECTOR_1;
/*********************  calculations for debug logging info */
      track_sector = g_hd5->hd_sector_num;
      if (track_sector % 2)
        {                       /* if odd sector need to add skew */
          track_sector = ((track_sector + 8) % 16);
        }
      logical_sector_num =
        (g_hd5->cylinder * (g_hd5->hd_max_heads + 1) * 16) +
        (g_hd5->hd_surface * 16) + track_sector;
      xlog (HDC,
            "########  Cylinder:%4d   Head: %1d   Sector:%2d  DiskAddress:%5d   ####\n",
            g_hd5->cylinder, g_hd5->hd_surface, g_hd5->hd_sector_num,
            logical_sector_num);
/**********************/
      break;
    case SECTOR_1:
      g_hd5->hd_state_counter--;
      if (!g_hd5->hd_state_counter)
        {
          g_hd5->hd_read_write_active = FALSE;  /* was TRUE; */
          /*hd5->hd_state_counter = 2; */
          g_hd5->hd_state = SECTOR_2;
          break;
        }
      break;
/****************************************************************/
/* READ SECTOR SEQUENCE HERE                                    */
/****************************************************************/
    case READ_0:
      xlog (HDC, "READ-SECTOR:     HD STATE READ_0: \n");
      copy_cache_header ();
      copy_sector_to_ram ();
      g_hd5->hd_state_counter = 2;
      g_hd5->hd_state = READ_1;
      break;
    case READ_1:
      xlog (HDC, "READ-SECTOR:     HD STATE READ_1: \n");
      g_hd5->hd_state_counter--;
      if (!g_hd5->hd_state_counter)
        {
          g_hd5->hd_state = READ_2;
          g_hd5->hd_read_write_active = TRUE;
          g_hd5->hd_state_counter = 8;  //*eight *//*
          break;
        }
      break;
    case READ_2:
      xlog (HDC, "READ-SECTOR:     HD STATE READ_2: \n");
      g_hd5->hd_state_counter--;
      if (!g_hd5->hd_state_counter)
        {
          g_hd5->hd_state = SECTOR_2;   /*sector ends */
          g_hd5->hd_read_write_active = FALSE;
          /*hd5->hd_state_counter = 2; */
          break;
        }
      break;
/****************************************************************/
/* WRITE SECTOR SEQUENCE HERE                                   */
/****************************************************************/
    case WRITE_0:
      xlog (HDC, "WRITE-SECTOR:     HD STATE WRITE_0: \n");
      g_hd5->hd_state_counter = 1;
      g_hd5->hd_state = WRITE_1;
      g_hd_sync = 0;
      break;
    case WRITE_1:
      g_hd5->hd_state_counter--;
      if (!g_hd5->hd_state_counter)
        {
          if (g_hd_sync)
            {
              write_cached_sector ();
            }
          g_hd5->hd_state = WRITE_2;
          g_hd5->hd_state_counter = 2;
          break;
        }
      break;
    case WRITE_2:
      g_hd5->hd_state_counter--;
      if (!g_hd5->hd_state_counter)
        {
          g_hd5->hd_state = SECTOR_2;   /*sector ends */
          g_hd5->hd_read_write_active = FALSE;
          /*hd5->hd_state_counter = 1; */
          break;
        }
      break;
/***************************************************************/
/*          END OF SECTOR -> NEXT SECTOR                       */
/***************************************************************/
    case SECTOR_2:
      if (g_hd_delay)
        {
          g_hd5->hd_state_counter = 115;        /* vary hard drive speed between */
        }                       /* 'fast' and 'normal'           */
      else
        {
          g_hd5->hd_state_counter = 1;
        }
      g_hd5->hd_state = SECTOR_3;

      break;
    case SECTOR_3:
      xlog (HDC, "SECTOR END:     HD STATE SECTOR_2: \n");
      g_hd5->hd_state_counter--;
      if (!g_hd5->hd_state_counter)
        {
          g_hd5->hd_state = SECTOR_4;   /*sector ends */
          break;
        }
      break;
    case SECTOR_4:
      g_hd5->hd_sector_num++;
      if (g_hd5->hd_sector_num >= 16)
        {                       /* NS HD has 16 sectors (0-15), back to sector 0 */
          g_hd5->hd_sector_num = 0;
          g_hd5->hd_state = INDEX_0;
          break;
        }
      g_hd5->hd_state = SECTOR_0;       /* not sector 0, back to main-sequence (1-15) */
      break;
/***************************************************************/
/*           BAD STATE NUMBER - COMPLAIN                       */
/***************************************************************/
    default:
      xlog (HDC, "Strange state value: %d \n", g_hd5->hd_state);
      g_hd5->hd_state = SECTOR_0;
      break;
    }                           /* end switch */
}
