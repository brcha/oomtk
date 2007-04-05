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
#       ctors.cc
      CPUID.cc
      Paging.cc
      PagingLegacy.cc
      PagingPAE.cc
      GDT.cc
   )


set(oomtk_LIBRARIES
    minilibc
    Devices
    Kernel
    supc++
    gcc
    glue
#     libc++
#     supc++
   )

add_executable(oomtk ${boot_ia32_SOURCES})
target_link_libraries(oomtk ${oomtk_LIBRARIES})
add_dependencies(oomtk ${SYS_BINARY_DIR}/boot/${OOMTK_ARCH}/linker.${OOMTK_ARCH}.ld)

set_target_properties(oomtk
  PROPERTIES LINK_FLAGS "-Xlinker -T -Xlinker ${SYS_BINARY_DIR}/boot/${OOMTK_ARCH}/linker.${OOMTK_ARCH}.ld"
             LINKER_LANGUAGE  CXX
  )

add_custom_command(TARGET oomtk
                   PRE_LINK
                   COMMAND ${CMAKE_C_COMPILER} -E -P -DIA32 -D__IA32__ -D__ARCH__=ia32 -I ${SYS_SOURCE_DIR}/include -include ${SYS_SOURCE_DIR}/include/macros.h -include ${SYS_SOURCE_DIR}/include/config.h -c ${CMAKE_CURRENT_SOURCE_DIR}/linker.ia32.ld.S -o ${CMAKE_CURRENT_BINARY_DIR}/linker.ia32.ld
                  )


# add_executable(oomtk ${boot_ia32_SOURCES})
# set_target_properties(oomtk PROPERTIES LINKER_LANGUAGE CXX)

# add_library(oomtkBoot STATIC ${boot_ia32_SOURCES})
# set_target_properties(oomtkBoot PROPERTIES LINKER_LANGUAGE CXX)

# Make custom commands for start.S & for linker.ia32.ld.S

# add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/linker.ia32.ld
#                    COMMAND ${CMAKE_C_COMPILER} -E -P ${CMAKE_C_FLAGS} -c ${CMAKE_CURRENT_SOURCE_DIR}/linker.ia32.ld.S -o ${CMAKE_CURRENT_BINARY_DIR}/linker.ia32.ld
#                    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/linker.ia32.ld.S
#                   )

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
