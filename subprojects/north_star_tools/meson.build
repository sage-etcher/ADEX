
project('NorthStar-Unix Tools', 'c', 
  license : 'GPL2',
  default_options : [
      'c_std=c89',
      'warning_level=3',
  ],
)

add_project_arguments([ '-D_POSIX_SOURCE' ], language : 'c')

chk_sector = executable('chk_sector', 'chk_sector.c', install : true)
compact  = executable('compact', 'compact.c',   install : true)
jdz80    = executable('jdz80',    'jdz80.c',    install : true)
mkfs_ns  = executable('mkfs.ns',  'mkfs.ns.c',  install : true)
mkhd     = executable('mkhd',     'mkhd.c',     install : true)
nsfd2u   = executable('nsfd2u',   'nsfd2u.c',   install : true)
nsfdls   = executable('nsfdls',   'nsfdls.c',   install : true)
nsfilecalc = executable('nsfilecalc', 'nsfilecalc.c', install : true)
nshdbm   = executable('nshdbm',   'nshdbm.c',   install : true)
nshdcp   = executable('nshdcp',   'nshdcp.c',   install : true)
nshdinfo = executable('nshdinfo', 'nshdinfo.c', install : true)
nshdls   = executable('nshdls',   'nshdls.c',   install : true)
printnsb = executable('printnsb', 'printnsb.c', install : true)
u2nsfd   = executable('u2nsfd',   'u2nsfd.c',   install : true)
unskew_hd_image = executable('unskew-hd-image', 'unskew-hd-image.c', install : true)

# vim ts=2 sts=2 sw=2 et
# end of file
