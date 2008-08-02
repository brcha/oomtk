#
#  Copyright (C) 2008 by Filip Brcic <brcha@gna.org>
#
#  This file is part of OOMTK
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

# This should be included only if OOMTK_ARCH is "ia32"
if(NOT OOMTK_ARCH STREQUAL "ia32")
	message(FATAL_ERROR "arch/ia32 included for non-ia32 arch!!!")
endif(NOT OOMTK_ARCH STREQUAL "ia32")

set(arch_ia32_SOURCES
	start.S
	init.cpp
	otss.cpp
	ogdt.cpp
	opaging-ia32.cpp
	opaginglegacy.cpp
	opagingpae.cpp
)

set(oomtk_LIBRARIES
    Kernel
    Devices
    supc++
    minilibc
#     libc++
    glue
    gcc
   )

add_executable(oomtk ${arch_ia32_SOURCES})
target_link_libraries(oomtk ${oomtk_LIBRARIES})
set_target_properties(oomtk
  PROPERTIES LINK_FLAGS "-Xlinker -T -Xlinker ${CMAKE_CURRENT_BINARY_DIR}/ldscript"
  LINKER_LANGUAGE  CXX
  )

add_custom_command(TARGET oomtk
                   PRE_LINK
                   COMMAND ${CMAKE_C_COMPILER} -E -P -I ${sys_SOURCE_DIR} -I ${sys_SOURCE_DIR}/include -DOOMTK_ARCH=ia32 -DOOMTK_ARCH_IA32 -c ${CMAKE_CURRENT_SOURCE_DIR}/ldscript.S -o ${CMAKE_CURRENT_BINARY_DIR}/ldscript
                  )