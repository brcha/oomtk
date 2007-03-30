#
#  Copyright (C) 2006 by Filip Brcic <brcha@users.sourceforge.net>
#
#  This file is part of OOMTK
#

# $Id$

include(UsePreprocess)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

set(boot__SOURCES
      start.S
      init.cc
   )
set(boot_PREP
      ctors.cpp
   )
# set(oomtk_LIBRARIES
#        Display
#    )

prep_wrap_cpp(boot_PREPROCESSED ${boot_PREP})

#set(CMAKE_CXX_LINK_EXECUTABLE
#  "<CMAKE_CXX_COMPILER> <FLAGS> -nostdlib -nostartfiles -nodefaultlibs -fno-rtti -fno-builtin -fno-exceptions -fomit-frame-pointer -Xlinker -T -Xlinker ${CMAKE_CURRENT_SOURCE_DIR}/linker.ld <OBJECTS> -o <TARGET> <LINK_LIBRARIES>"
#)

# add_executable(oomtk ${boot_SOURCES} ${boot_PREPROCESSED} ${boot_NASM_OBJECTS})
# target_link_libraries(oomtk ${oomtk_LIBRARIES})
#
# set_target_properties(oomtk
#   PROPERTIES LINK_FLAGS "-Xlinker -T -Xlinker ${CMAKE_CURRENT_SOURCE_DIR}/linker.ld"
#              LINKER_LANGUAGE  CXX
#   )

set(boot_SOURCES
      ${boot__SOURCES}
      ${boot_PREPROCESSED}
   )