#
#  Copyright (C) 2007,2008 by Filip Brcic <brcha@gna.org>
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

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

# Generate oomtkversion.h
configure_file(oomtkversion.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/oomtkversion.h)

set(kernel_SOURCES
  oomtkversion.cpp
  oprocess.cpp
  ocpu.cpp
  ${OOMTK_ARCH}/ocpu-${OOMTK_ARCH}.cpp
  ophysicalmemorymanagement.cpp
  oatomic.cpp
  ${OOMTK_ARCH}/oatomic-${OOMTK_ARCH}.cpp
  ${OOMTK_ARCH}/oatomicptr-${OOMTK_ARCH}.cpp
  omutex.cpp
  ospinlock.cpp
  )

add_library(Kernel STATIC ${kernel_SOURCES})

add_subdirectory(devices)
