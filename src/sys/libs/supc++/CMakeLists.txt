#
#  Copyright (C) 2006-2008 by Filip Brcic <brcha@gna.org>
#
#  This file is part of OOMTK
#

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  )

set(supc++_SOURCES
  initialize.cc memory.cc stdexcept.cc finalize.cc purevirtual.cc
  typeinfo.cc guard.cc
  )

add_library(supc++ STATIC ${supc++_SOURCES})