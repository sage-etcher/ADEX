/**********************************************************************************/
/* This file 'ade_extvars.h' produced by 'g2extvars' from the   */
/*  global-variable file 'ade_gvars.h'                                  */
/* Do not edit, as your changes  can be overwritten at any time.         210314   */
/**********************************************************************************/


/*****************************************************/
/************      ade_gvar.h      *******************/
/*****************************************************/
/*  Global variables for Z80 Computer Emulator  */
/*                       (ADE)                       */
/*                       GPL V2                      */
/*      Copyright (C) 1995  Frank D. Cringle.        */
/*      Copyright (C) 1996-Present Jack Strangio     */
/*                                                   */
/*****************************************************/
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

extern struct sio g_siotab[MAXPSTR];


extern unsigned char g_adv_prom_0000_0800[0x800];


////    GLOBAL VARIABLES

extern BYTE *g_base;
extern BYTE g_blanking_flag;
extern BYTE g_cmd_ack_counter;
extern BYTE g_ddt_mem_page;
extern BYTE *g_display_buffer;
extern BYTE *g_display_pixel_ptr;
extern BYTE g_display_ram[ADVANTAGE_VID_RAM_LEN];        /* ram xlated from adv video RAM format to display-RAM format */
extern BYTE g_four_key_reset_enable_flag;
extern BYTE g_io_control_reg;
extern BYTE g_kbd_char;
extern BYTE *g_machine_prom_code;
extern BYTE g_motherboard_status;
extern BYTE g_parm[MAX_PARAMS][PARM_LEN];
extern BYTE g_pb24_data[0xF0000];        // (640 * 256 * 3) * 2
extern BYTE *g_pb24_ptr;
extern BYTE g_pio_interrupt_mask;
extern BYTE g_p_lo;
extern BYTE g_p_hi;

extern BYTE g_ram[0x40000];
extern BYTE g_rgb3_byte[256][8][RGB_BYTES_PER_PIXEL];    /* table converting 1bpp byte-value to 24bpp rgb byte-value */
extern BYTE g_rgb_pb_data[0x70800];      /* rgb pixbuf data - 640 * 240 * 3bytes(24bits) per pixel */
extern BYTE *g_rgb_pixbuf_data;
extern BYTE g_sio_test;
extern BYTE g_tmpbuff[0x10000];
extern BYTE *g_videoram;
extern BYTE g_vr_byte;
extern char g_work_dir[30];
extern char g_aread_name[70];
extern char *g_bdata;
extern char g_bline[1024];
extern char g_bootdisk[TERM_WIDTH_STD];
extern char g_break_string[20];
extern char g_cfg_arg[30][128];
extern char g_cfg_key[30][20];
extern char g_cline[512];
extern char *g_cline_ptr;
extern char *g_confname;
extern char g_confnamebuff[128];
extern char *g_cptr;
extern char g_datestring[10];
extern char g_dbuffer_data[DBUFF_WIDTH * (DBUFF_HEIGHT * 2) / BITS_PER_BYTE];
extern char g_env_home[TERM_WIDTH_STD];
extern char g_env_logname[TERM_WIDTH_STD];
extern char g_env_pwd[128];
extern char g_env_shell[TERM_WIDTH_STD];
extern char g_eprogname[TERM_WIDTH_STD];
extern char *g_file_choice_name;
extern char g_filenamebuff[CMD_LEN];
extern char g_flags[9];
extern char g_hexstring[10];
extern char g_line_out[256];
extern char g_logfilename[TERM_WIDTH_STD];
extern char g_machine_name[TERM_WIDTH_STD];
extern char g_machine_prom_name_string[TERM_WIDTH_STD];
extern char g_machine_version[TERM_WIDTH_STD];
extern char g_mcmd[CMD_LEN];
extern char g_namebuff[CMD_LEN];
extern char g_outbuff[CMD_LEN];
extern char g_pstring[128];
extern char g_qchar;
extern char g_qqstr[512];
extern char g_rc_config[TERM_WIDTH_STD];
extern char g_sbuf[80];
extern char g_slogfilename[TERM_WIDTH_STD];
extern char *g_sptr;
extern char g_this_year[10];
extern char g_trap_string[20];
extern char g_zo_filename[8][128];
extern const char *g_white;
extern const GtkTextIter *g_line_end_iter;
extern const GtkTextIter *g_line_start_iter;
extern const struct timespec g_pulse;
extern const struct timeval g_immediate;
extern FILE *g_ascii_in;
extern FILE *g_cmd;
extern FILE *g_conf;
extern FILE *g_fp;
extern FILE *g_logfile;
extern FILE *g_slog;
extern FILE *g_zo_f[8];
extern gchar *g_fdname;
extern gchar *g_port_name;
extern gchar g_vstring[128];
extern GdkDisplay *g_gdkdisplay;
extern GdkPixbuf *g_bannerpb;
extern gint g_aread;
extern gint g_aread_result;
extern gint g_choose_mode;
extern gint g_clix;
extern gint g_ade_debug;
extern gint g_fc_disk;
extern gint g_fc_result;    /* file chooser result*/
extern gint g_file_choice_val;
extern gint g_io_port;
extern gint g_io_result;
extern gint g_xsize;
extern GtkBuilder *g_builder;
extern GtkButton *g_debugexit;
extern GtkButton *g_exit_button;
extern GtkButton *g_fd1_change;
extern GtkButton *g_fd1_current;
extern GtkButton *g_fd1_eject;
extern GtkButton *g_fd2_change;
extern GtkButton *g_fd2_current;
extern GtkButton *g_fd2_eject;
extern GtkButton *g_h1button;
extern GtkButton *g_h2button;
extern GtkButton *g_h3button;
extern GtkButton *g_h4button;
extern GtkButton *g_h5button;
extern GtkButton *g_h6button;
extern GtkButton *g_hdd_change;
extern GtkButton *g_hdd_current;
extern GtkButton *g_hdd_eject;
extern GtkButton *g_hunused;
extern GtkButton *g_memhidebutton;
extern GtkButton *g_new_floppy_button;
extern GtkButton *g_p1button;
extern GtkButton *g_p2button;
extern GtkButton *g_p3button;
extern GtkButton *g_p4button;
extern GtkButton *g_p5button;
extern GtkButton *g_p6button;
extern GtkButton *g_pio_in_change;
extern GtkButton *g_pio_in_current;
extern GtkButton *g_pio_in_detach;
extern GtkButton *g_pio_out_change;
extern GtkButton *g_pio_out_current;
extern GtkButton *g_pio_out_detach;
extern GtkButton *g_punused;
extern GtkButton *g_s1button;
extern GtkButton *g_s2button;
extern GtkButton *g_s3button;
extern GtkButton *g_s4button;
extern GtkButton *g_s5button;
extern GtkButton *g_s6button;
extern GtkButton *g_sio_in_change;
extern GtkButton *g_sio_in_current;
extern GtkButton *g_sio_in_detach;
extern GtkButton *g_sio_out_change;
extern GtkButton *g_sio_out_current;
extern GtkButton *g_sio_out_detach;
extern GtkButton *g_size_end;
extern GtkButton *g_sunused;
extern GtkCheckButton *g_break_enable;
extern GtkCheckButton *g_dcb001;
extern GtkCheckButton *g_dcb002;
extern GtkCheckButton *g_dcb004;
extern GtkCheckButton *g_dcb008;
extern GtkCheckButton *g_dcb010;
extern GtkCheckButton *g_dcb020;
extern GtkCheckButton *g_dcb040;
extern GtkCheckButton *g_dcb080;
extern GtkCheckButton *g_dcb100;
extern GtkCheckButton *g_dcb1000;
extern GtkCheckButton *g_dcb200;
extern GtkCheckButton *g_dcb2000;
extern GtkCheckButton *g_dcb400;
extern GtkCheckButton *g_dcb4000;
extern GtkCheckButton *g_dcb800;
extern GtkCheckButton *g_dcb8000;
extern GtkCheckButton *g_trap_enable;
extern GtkEntry *g_break_entry;
extern GtkEntry *g_debugvalue;
extern GtkEntry *g_new_floppy_text;
extern GtkEntry *g_trap_entry;
extern GtkImage *g_ade_win;
extern GtkLabel *g_break_label;
extern GtkLabel *g_fd1fn;
extern GtkLabel *g_fd2fn;
extern GtkLabel *g_hdc_label;
extern GtkLabel *g_hdfn;
extern GtkLabel *g_pio_dev_fn;
extern GtkLabel *g_pio_in_fn;
extern GtkLabel *g_pio_label;
extern GtkLabel *g_pio_out_fn;
extern GtkLabel *g_sio_dev_fn;
extern GtkLabel *g_sio_in_fn;
extern GtkLabel *g_sio_label;
extern GtkLabel *g_sio_out_fn;
extern GtkLabel *g_trap_label;
extern GtkMenuBar *g_settings;
extern GtkRadioButton *g_screensize1button;
extern GtkRadioButton *g_screensize2button;
extern GtkRadioButton *g_screensize3button;
extern GtkScrolledWindow *g_memscrl;
extern GtkScrolledWindow *g_winstatusscroll;
extern GtkTextBuffer *g_mem_buffer;
extern GtkTextBuffer *g_status_buffer;
extern GtkTextIter g_mem_end_iter;
extern GtkTextIter g_status_end_iter;
extern GtkTextView *g_memtext;
extern GtkTextView *g_statusinfo;
extern GtkWidget *g_Wade_win;
extern GtkWidget *g_Wbreak_enable;
extern GtkWidget *g_Wbreak_entry;
extern GtkWidget *g_Wbreak_label;
extern GtkWidget *g_Wbreak_top;
extern GtkWidget *g_Wdcb001;
extern GtkWidget *g_Wdcb002;
extern GtkWidget *g_Wdcb004;
extern GtkWidget *g_Wdcb008;
extern GtkWidget *g_Wdcb010;
extern GtkWidget *g_Wdcb020;
extern GtkWidget *g_Wdcb040;
extern GtkWidget *g_Wdcb080;
extern GtkWidget *g_Wdcb100;
extern GtkWidget *g_Wdcb1000;
extern GtkWidget *g_Wdcb200;
extern GtkWidget *g_Wdcb2000;
extern GtkWidget *g_Wdcb400;
extern GtkWidget *g_Wdcb4000;
extern GtkWidget *g_Wdcb800;
extern GtkWidget *g_Wdcb8000;
extern GtkWidget *g_Wdebugexit;
extern GtkWidget *g_Wdebugtop;
extern GtkWidget *g_Wdebugvalue;
extern GtkWidget *g_Wdisks_top;
extern GtkWidget *g_Wexit_button;
extern GtkWidget *g_Wfd1_change;
extern GtkWidget *g_Wfd1_current;
extern GtkWidget *g_Wfd1_eject;
extern GtkWidget *g_Wfd1fn;
extern GtkWidget *g_Wfd2_change;
extern GtkWidget *g_Wfd2_current;
extern GtkWidget *g_Wfd2_eject;
extern GtkWidget *g_Wfd2fn;
extern GtkWidget *g_Wh1button;
extern GtkWidget *g_Wh2button;
extern GtkWidget *g_Wh3button;
extern GtkWidget *g_Wh4button;
extern GtkWidget *g_Wh5button;
extern GtkWidget *g_Wh6button;
extern GtkWidget *g_Whdc_label;
extern GtkWidget *g_Whdd_change;
extern GtkWidget *g_Whdd_current;
extern GtkWidget *g_Whdd_eject;
extern GtkWidget *g_Whdfn;
extern GtkWidget *g_Whunused;
extern GtkWidget *g_Wioports_top;
extern GtkWidget *g_Wmemhidebutton;
extern GtkWidget *g_Wmemscrl;
extern GtkWidget *g_Wmemtext;
extern GtkWidget *g_Wmemtw;
extern GtkWidget *g_Wnew_floppy_button;
extern GtkWidget *g_Wnew_floppy_text;
extern GtkWidget *g_Wp1button;
extern GtkWidget *g_Wp2button;
extern GtkWidget *g_Wp3button;
extern GtkWidget *g_Wp4button;
extern GtkWidget *g_Wp5button;
extern GtkWidget *g_Wp6button;
extern GtkWidget *g_Wpio_dev_change;
extern GtkWidget *g_Wsio_dev_change;
extern GtkWidget *g_Wpio_in_change;
extern GtkWidget *g_Wpio_in_current;
extern GtkWidget *g_Wsio_dev_detach;
extern GtkWidget *g_Wpio_dev_detach;
extern GtkWidget *g_Wpio_in_detach;
extern GtkWidget *g_Wpio_in_fn;
extern GtkWidget *g_Wpio_label;
extern GtkWidget *g_Wpio_out_change;
extern GtkWidget *g_Wpio_out_current;
extern GtkWidget *g_Wpio_out_detach;
extern GtkWidget *g_Wpio_out_fn;
extern GtkWidget *g_Wpunused;
extern GtkWidget *g_Ws1button;
extern GtkWidget *g_Ws2button;
extern GtkWidget *g_Ws3button;
extern GtkWidget *g_Ws4button;
extern GtkWidget *g_Ws5button;
extern GtkWidget *g_Ws6button;
extern GtkWidget *g_Wscreensize1button;
extern GtkWidget *g_Wscreensize2button;
extern GtkWidget *g_Wscreensize3button;
extern GtkWidget *g_Wsettings;
extern GtkWidget *g_Wsio_in_change;
extern GtkWidget *g_Wsio_in_current;
extern GtkWidget *g_Wsio_in_detach;
extern GtkWidget *g_Wsio_in_fn;
extern GtkWidget *g_Wsio_dev_fn;
extern GtkWidget *g_Wpio_dev_fn;
extern GtkWidget *g_Wsio_label;
extern GtkWidget *g_Wsio_out_change;
extern GtkWidget *g_Wsio_out_current;
extern GtkWidget *g_Wsio_out_detach;
extern GtkWidget *g_Wsio_out_fn;
extern GtkWidget *g_Wsize_end;
extern GtkWidget *g_Wsize_top;
extern GtkWidget *g_Wslottop;
extern GtkWidget *g_Wstatusinfo;
extern GtkWidget *g_Wsunused;
extern GtkWidget *g_Wtrap_enable;
extern GtkWidget *g_Wtrap_entry;
extern GtkWidget *g_Wtrap_label;
extern GtkWidget *g_Wwindow;
extern GtkWidget *g_Wwinstatusscroll;
extern GtkWindow *g_break_top;
extern GtkWindow *g_disks_top;
extern GtkWindow *g_ioports_top;
extern GtkWindow *g_memtw;
extern GtkWindow *g_window;
extern int g_ascii;
extern int g_auto_repeat;
extern int g_blank_display;
extern int g_break_active;
extern int g_break_wait;
extern int g_ade_shift_flag;
extern int g_ade_control_flag;
extern int g_ade_cmd_flag;
extern int g_capslock;
extern int g_capslock_start;
extern int g_capslock_end;
extern int g_cursor_lock;
extern int g_char_overrun;
extern int g_charwait;
extern int g_cmd_ack;
extern int g_cmd_end_flag;
extern int g_col;
extern int g_coldboot_flag;
extern int g_current_disk;
extern int g_display_flag;
extern int g_ade_meta_flag;
extern int g_examine_flag;
extern int g_fdc_card;
extern int g_floppy_pulse_flag;
extern int g_hdcslot;
extern gboolean g_hd_delay;
extern int g_hd_sync;
extern int g_hd_unit;
extern int g_interrupt_mode;
extern int g_interrupt_req_flag;
extern int g_io_interrupt;
extern int g_ascii_eol;
extern int g_kbd_data_flag;
extern int g_kbd_interrupt;
extern int g_keyboard_active;
extern int g_keybrdin_int;
extern int g_location;
extern int g_machine_floppy_max;
extern int g_machine_hd_max;
extern int g_machine_reset_flag;
extern int g_max_cfg_key;
extern int g_memory_mapping_register[4];
extern int g_microtick;
extern int g_native_flags;
extern int g_no_boot_disk;
extern int g_non_mask_interrupt;
extern int g_noprefix_flag;
extern int g_numlock_start;
extern int g_numlock_end;
extern int g_ok;
extern int g_pio_in_ack;
extern int g_pio_input_flag;
extern int g_pio_out_ack;
extern int g_pio_output_flag;
extern int g_pioslot;
extern int g_prom_active;
extern int g_q;
extern int g_row;
extern int g_rtc_int;
extern int g_rtc_interval;
extern int g_rtclock_int_enabled;
extern int g_rtclock_tick_flag;
extern int g_scanline;
extern int g_show_windows;
extern int g_sioslot;
extern int g_started;
extern int g_stopsim;
extern int g_term_slow;
extern int g_term_type;
extern int g_term_width;
extern int g_timer_interrupt_active;
extern int g_trap_active;
extern int g_warn_boot_floppy;
extern int g_x;
extern int g_xnum;
extern int g_ynum;
extern int g_zo_flen[8];
extern long g_loadadr;
extern PangoFontDescription *g_mono_font;
extern struct CPU g_cpu;
extern struct CPU *g_cpux;
extern struct fixed_disk *g_hd5;
extern struct fixed_disk g_nshd;
extern struct kbdq *g_advq;
extern struct kbdq g_kqueue[2];
extern struct kbdq *g_memq;
extern struct peripheral g_slot[7];
extern struct peripheral *g_slotx;
extern struct removable_disk *g_floppy;
extern struct removable_disk g_nsd[MACHINE_FLOPPY_MAX];
extern struct timespec g_rem;
extern struct termios g_sio_raw;
extern struct termios g_sio_cooked;
extern struct termios g_pio_raw;
extern struct termios g_pio_cooked;
extern unsigned char g_x2gchar[MAXCHARS][2];
extern unsigned char *g_xptr;
extern unsigned int g_aread_clock;
extern unsigned int g_break_address;
extern unsigned int g_display_len;
extern unsigned int g_dots_per_pixel;
extern unsigned int g_mem_state;
extern unsigned int g_msize;
extern unsigned int g_msize64;
extern unsigned int g_rgb_page_offset;
extern unsigned int g_rgb_pixbuf_data_len;
extern unsigned int g_trap_address;
extern unsigned int g_x_dots_per_pixel;
extern unsigned int g_xlt[16];
extern unsigned int g_xlt_addr[ADVANTAGE_VID_RAM_LEN];   /* table converting RAM address to display address (still 1 bpp) */
extern unsigned int g_y_dots_per_pixel;
extern WORD g_break_dbg;
extern WORD g_display_flag_counter;
extern WORD g_floppy_controller_clock;
extern WORD g_hdseek_delay;
extern WORD g_IFF;
extern WORD g_interrupt_newpc;
extern WORD g_machine_prom_address;
extern WORD g_machine_prom_length;
extern WORD g_prom_base;
extern WORD g_rom_end;
extern WORD g_vector;

extern char g_nc_logfilename[64];
extern FILE *g_nc_log;

extern char g_xconfigfilename[64];
extern int g_configfileflag;

extern BYTE *g_pio_character_buff_ptr;
extern BYTE *g_sio_character_buff_ptr;
extern unsigned int g_pio_icptr;
extern unsigned int g_sio_icptr;
extern unsigned int g_pio_ocptr;
extern unsigned int g_sio_ocptr;
extern int g_z80_active;
extern BYTE g_diskname[25];
extern BYTE g_fnbuff[128];
extern BYTE g_portname[25];
extern char *g_portwholename;
extern char *g_portbasename;

extern unsigned char g_adv_kbd_scancodes[0xA0][0x07];

