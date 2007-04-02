#
#  Copyright (C) 2006 by Filip Brcic <brcha@users.sourceforge.net>
#
#  This file is part of OOMTK
#

# $Id$

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(boot_ia32_SOURCES
      start.S
      init.cc
      ctors.cc
      CPUID.cc
      Paging.cc
      PagingLegacy.cc
      PagingPAE.cc
      GDT.cc
   )

add_executable(oomtk ${boot_ia32_SOURCES})
set_target_properties(oomtk PROPERTIES LINKER_LANGUAGE CXX)

# Make custom commands for start.S & for linker.ia32.ld.S

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/linker.ia32.ld
                   COMMAND ${CMAKE_C_COMPILER} -E -P ${CMAKE_C_FLAGS} -c ${CMAKE_CURRENT_SOURCE_DIR}/linker.ia32.ld.S -o ${CMAKE_CURRENT_BINARY_DIR}/linker.ia32.ld
                   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/linker.ia32.ld.S
                  )

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
