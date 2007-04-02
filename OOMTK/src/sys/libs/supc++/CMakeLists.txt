#
#  Copyright (C) 2007 by Filip Brcic <brcha@users.sourceforge.net>
#
#  This file is part of OOMTK
#

# $ Id: $

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

set(supc++_SOURCES
    initialize.cc memory.cc stdexcept.cc finalize.cc
    purevirtual.cc typeinfo.cc guard.cc
   )

add_library(supc++ STATIC ${supc++_SOURCES})
