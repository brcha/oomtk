#
#  Copyright (C) 2007 by Filip Brčić <brcha@users.sourceforge.net>
#
#  This file is part of OOMTK
#

# $ Id: $

INCLUDE(FindCygwin)

FIND_PROGRAM(SED
  sed
  ${CYGWIN_INSTALL_PATH}/bin
  /bin
  /usr/bin 
  /usr/local/bin
  /sbin
)
MARK_AS_ADVANCED(
  SED
)
