#
#  Copyright (C) 2007 by Filip Brcic <brcha@users.sourceforge.net>
#
#  This file is part of OOMTK
#

# $ Id: $

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(devices_SOURCES
    Console.cc
    PortIO.cc
    ${OOMTK_ARCH}/PortIO-${OOMTK_ARCH}.cc
    VgaConsole.cc
    libc_backend.cc
   )

add_library(Devices STATIC ${devices_SOURCES})
