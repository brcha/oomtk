#
#  Copyright (C) 2007 by Filip Brcic <brcha@users.sourceforge.net>
#
#  This file is part of OOMTK
#

# $ Id: $

set(libglue_SOURCES
      free.c
      malloc.c
      memset.c
      abort.c
   )

add_library(glue STATIC ${libglue_SOURCES})