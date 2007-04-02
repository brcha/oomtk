#
#  Copyright (C) 2007 by Filip Brcic <brcha@users.sourceforge.net>
#
#  This file is part of OOMTK
#

# $ Id: $

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(kernel_SOURCES
    fatal.cc
   )

add_library(Kernel STATIC ${kernel_SOURCES})
