
project('ADE', 'c',
  version : '0.67',
  license : 'GPL2',
  default_options : [
    'c_std=c99',
    'warning_level=3',
  ],
  meson_version : '>= 1.1'
)

if get_option('northstartools')
  subproject('north_star_tools')
endif

if get_option('prefix') == '/usr'
  etcdir = get_option('sysconfdir')
else 
  etcdir = get_option('prefix') / get_option('sysconfdir')
endif
add_project_arguments([ 
    '-D_POSIX_SOURCE',
    '-DETCDIR="' + etcdir + '"'
  ], 
  language : 'c',
)

add_project_link_arguments([ '-rdynamic' ], language : 'c')

gnome = import('gnome')

gtk3_dep = dependency('gtk+-3.0', required : true)
X11_dep = dependency('X11', required : true)

ade_resources_c = gnome.compile_resources(
  'ade_resources', 'ade_gresource.xml',
  source_dir : 'gresources',
  c_name : 'ade',
)

ade_bin = executable(
  'ade', [
    'src/ade_base.c', 
    'src/ade_breaktrap.c', 
    'src/ade_config.c',
    'src/ade_ddt.c',
    'src/ade_debug.c',
    'src/ade_disks.c',
    'src/ade_dz80.c',
    'src/ade_fdc.c',
    'src/ade_file_choose.c',
    'src/ade_hdc.c',
    'src/ade_io.c',
    'src/ade_inputs.c',
    'src/ade_ioports.c',
    'src/ade_launch_ade.c',
    'src/ade_launch_gui.c',
    'src/ade_main.c',
    'src/ade_mem.c',
    'src/ade_slot_card.c',
    'src/ade_trap.c',
    'src/ade_display.c',
    'src/ade_z80.c',
    'src/jfuncs.c',
    ade_resources_c,
  ],
  dependencies : [
    gtk3_dep,
    X11_dep,
  ],
  install : true,
)

# install data
if get_option('disks')
  subproject('ade-disks')
endif

if get_option('documentation')
  subproject('ade-documentation')
endif

# install desktop file
if get_option('desktop')
  install_data(
    'src/.star_7point_red.png', 
    rename : 'ade-logo.png', 
    install_dir : get_option('datadir') / 'icons/hicolor/64x64',
  )
  
  desktop_conf = configuration_data({
    'VERSION' : meson.project_version(),
    'EXEC'    : get_option('prefix') / get_option('bindir') / 'ade',
    'ICON'    : get_option('prefix') / get_option('datadir') / 'icons/hicolor/64x64/ade-logo.png',
  })
  ade_desktop = configure_file(
    input  : 'advantage.desktop.in',
    output : 'advantage.desktop',
    configuration : desktop_conf,
  )
  install_data(
    ade_desktop,
    install_dir : get_option('datadir') / 'applications',
  )
endif

# install example ade.conf
rc_conf = configuration_data({
  'HDD_DISK' : get_option('prefix') / get_option('datadir') / 'advantage/disks/ADV_SG5A.NHD',
  'FD1_DISK' : get_option('prefix') / get_option('datadir') / 'advantage/disks/CPMBASIC_120A1.NSI',
  'FD2_DISK' : '',
  'DISK_DIR' : get_option('prefix') / get_option('datadir') / 'advantage/disks',
})
ade_conf = configure_file(
  input  : 'ade.conf.in',
  output : 'ade.conf',
  configuration : rc_conf,
)
install_data(
  ade_conf,
  install_dir : get_option('sysconfdir') / 'advantage',
)

# vim: ts=2 sts=2 sw=2 et
# end of file
