#
#  Copyright (C) 2006-2008 by Filip Brcic <brcha@gna.org>
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

project(sys C CXX ASM-GCC)

cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

# OOMTK Version
set(OOMTK_VERSION_MAJOR 00)
set(OOMTK_VERSION_MINOR 80)
set(OOMTK_VERSION_RELEASE 10) 
set(OOMTK_VERSION "${OOMTK_VERSION_MAJOR}.${OOMTK_VERSION_MINOR}.${OOMTK_VERSION_RELEASE}")
set(OOMTK_VERSION_STRING "${OOMTK_VERSION} (OOMTK 0.80.10 >= 20080721) codename: Horus")

# Flags

# Generic flags

set(CFLAGS_GENERIC "-static -nostdinc -nostdlib -fno-builtin -fomit-frame-pointer -Wall -Wno-format -nostartfiles -nodefaultlibs")
set(CXXFLAGS_ADDONS "-fno-exceptions")
set(ASMFLAGS_ADDONS "-D__ASSEMBLER__")

set(CFLAGS_RELEASE "-O2")
set(CFLAGS_DEBUG "-O2")

# ia32 flags
if(OOMTK_ARCH STREQUAL "ia32")

# Check the subarchitecture
set(IA32_386 ON CACHE BOOL "Generic ia32 processor (386+)")
set(IA32_486 OFF CACHE BOOL "Intel 486+ processor")
set(IA32_586 OFF CACHE BOOL "Intel Pentium processor")
set(IA32_686 OFF CACHE BOOL "Intel PentiumPro processor")
set(IA32_p2 OFF CACHE BOOL "Intel Pentium2 processor")
set(IA32_p3 OFF CACHE BOOL "Intel Pentium3 processor")
set(IA32_p4 OFF CACHE BOOL "Intel Pentium4 processor")
set(IA32_sse3 OFF CACHE BOOL "SSE3 instruction set support")

if(IA32_386)
  set(CFLAGS_ia32_subarch "-march=i386 -D__SUBARCH__=386")
endif(IA32_386)
if(IA32_486)
  set(CFLAGS_ia32_subarch "-march=i486 -D__SUBARCH__=486")
endif(IA32_486)
if(IA32_586)
  set(CFLAGS_ia32_subarch "-march=i586 -D__SUBARCH__=586")
endif(IA32_586)
if(IA32_686)
  set(CFLAGS_ia32_subarch "-march=i686 -D__SUBARCH__=686")
endif(IA32_686)
if(IA32_p2)
  set(CFLAGS_ia32_subarch "-march=pentium2 -D__SUBARCH__=Pentium2")
endif(IA32_p2)
if(IA32_p3)
  set(CFLAGS_ia32_subarch "-march=pentium3 -D__SUBARCH__=Pentium3")
endif(IA32_p3)
if(IA32_p4)
  set(CFLAGS_ia32_subarch "-march=pentium4 -D__SUBARCH__=Pentium4")
endif(IA32_p4)
if(IA32_sse3)
  set(CFLAGS_ia32_subarch "-march=pentium4 -msse3 -D__SUBARCH__=Pentium4 -D__SSE3__")
endif(IA32_sse3)

set(CFLAGS_ia32 "-DOOMTK_ARCH_IA32 -DOOMTK_ARCH=ia32 -mpreferred-stack-boundary=2 -falign-functions=4 ${CFLAGS_ia32_subarch}")

endif(OOMTK_ARCH STREQUAL "ia32")
# End of ia32 flags

# Now generate the real C/C++ flags used by CMake
set(CMAKE_C_FLAGS "${CFLAGS_GENERIC} ${CFLAGS_DEBUG} ${CFLAGS_${OOMTK_ARCH}}")
set(CMAKE_CXX_FLAGS "${CFLAGS_GENERIC} ${CFLAGS_DEBUG} ${CFLAGS_${OOMTK_ARCH}} ${CXXFLAGS_ADDONS}")
set(CMAKE_ASM-GCC_FLAGS "${CFLAGS_GENERIC} ${CFLAGS_DEBUG} ${CFLAGS_${OOMTK_ARCH}} ${ASMFLAGS_ADDONS}")

# Release:
set(CMAKE_C_FLAGS_RELEASE "${CFLAGS_GENERIC} ${CFLAGS_RELEASE} ${CFLAGS_${OOMTK_ARCH}}")
set(CMAKE_CXX_FLAGS_RELEASE "${CFLAGS_GENERIC} ${CFLAGS_RELEASE} ${CFLAGS_${OOMTK_ARCH}} ${CXXFLAGS_ADDONS}")
set(CMAKE_ASM-GCC_FLAGS_RELEASE "${CFLAGS_GENERIC} ${CFLAGS_RELEASE} ${CFLAGS_${OOMTK_ARCH}} ${ASMFLAGS_ADDONS}")

# Debug:
set(CMAKE_C_FLAGS_DEBUG "${CFLAGS_GENERIC} ${CFLAGS_DEBUG} ${CFLAGS_${OOMTK_ARCH}}")
set(CMAKE_CXX_FLAGS_DEBUG "${CFLAGS_GENERIC} ${CFLAGS_DEBUG} ${CFLAGS_${OOMTK_ARCH}} ${CXXFLAGS_ADDONS}")
set(CMAKE_ASM-GCC_FLAGS_DEBUG "${CFLAGS_GENERIC} ${CFLAGS_DEBUG} ${CFLAGS_${OOMTK_ARCH}} ${ASMFLAGS_ADDONS}")

# Ok, finished configuring, go build!
# Include gcc standard definitions
# TODO: Find a way to put ${TARGET} and ${GCC_VERSION} in these directories...
include_directories(
  ${CMAKE_INSTALL_PREFIX}/host/lib/gcc/i386-unknown-oomtk-gnu/4.1.2/include
)

include_directories( ${sys_SOURCE_DIR} ${sys_BINARY_DIR} ${sys_SOURCE_DIR}/include )

# minilibc
include_directories( ${sys_SOURCE_DIR}/libs/minilibc/include
                     ${sys_SOURCE_DIR}/libs/minilibc/${OOMTK_ARCH}/include
                   )

# libglue
include_directories( ${sys_SOURCE_DIR}/libs/libglue )

# libc++
include_directories( ${sys_SOURCE_DIR}/libs/libc++/include )

# supc++
include_directories( ${sys_SOURCE_DIR}/libs/supc++/include )

# Kernel headers
include_directories( ${sys_SOURCE_DIR}/kernel 
  ${sys_BINARY_DIR}/kernel
  )

add_subdirectory(arch)    # This directory contains architecture dependant files
add_subdirectory(kernel)  # This directory contains mostly architecture independant kernel classes
add_subdirectory(libs)    # This directory contains 3rd party libraries
