#
#  Copyright (C) 2006-2007 by Filip Brcic <brcha@users.sourceforge.net>
#
#  This file is part of OOMTK (http://launchpad.net/oomtk)
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

set(SYS_SOURCE_DIR "${OOMTK_SOURCE_DIR}/src/sys")
set(SYS_BINARY_DIR "${OOMTK_BINARY_DIR}/src/sys")

# OOMTK Version
#set(OOMTK_BASE_VERSION "0.80.1")
set(OOMTK_VERSION "0.80.1")
#set(OOMTK_REVISION "`bzr revno ${OOMTK_SOURCE_DIR}`")
#set(OOMTK_VERSION "${OOMTK_BASE_VERSION}-r${OOMTK_REVISION}")

# OOMTK Architecture (set in the main CMakeLists.txt file)
# supported archs = ia32
#set(OOMTK_ARCH "ia32")

# include stddef.h & similar files
include_directories(${CMAKE_INSTALL_PREFIX}/host/lib/gcc/i386-unknown-oomtk/4.1.1/include/)
include_directories(${CMAKE_INSTALL_PREFIX}/host/include/c++/4.1.1)

# Cross-gcc prefix
set(CROSS_BIN_PREFIX "${CMAKE_INSTALL_PREFIX}/host/bin")
if(OOMTK_ARCH STREQUAL "ia32")
  set(GCCPREFIX "${CROSS_BIN_PREFIX}/i386-unknown-oomtk-")
# elseif(OOMTK_ARCH STREQUAL "amd64")
#   set(GCCPREFIX "${CROSS_BIN_PREFIX}/x86_64-unknown-oomtk-")
# elseif(OOMTK_ARCH STREQUAL "arm")
#   set(GCCPREFIX "${CROSS_BIN_PREFIX}/arm-unknown-oomtk-")
# elseif(OOMTK_ARCH STREQUAL "ppc")
#   set(GCCPREFIX "${CROSS_BIN_PREFIX}/powerpc-unknown-oomtk-")
else(OOMTK_ARCH STREQUAL "ia32")
  MESSAGE(FATAL_ERROR "Unsupported architecture :(")
endif(OOMTK_ARCH STREQUAL "ia32")

# Cross-gcc tools
set(CMAKE_C_COMPILER "${GCCPREFIX}gcc")
set(CMAKE_COMPILER_IS_GNUCC 1)
set(CMAKE_CXX_COMPILER "${GCCPREFIX}g++")
set(CMAKE_COMPILER_IS_GNUCXX 1)
set(CMAKE_GnuAssembler_COMPILER "${GCCPREFIX}gcc")
set(CMAKE_GnuAssembler_IS_GNUGAS 1)
set(CMAKE_AR "${GCCPREFIX}ar")
set(CMAKE_RANLIB "${GCCPREFIX}ranlib")

# Find out the date
#execute_process(COMMAND "date -R"
#  OUTPUT_VARIABLE OOMTK_BUILD_DATE
#  OUTPUT_STRIP_TRAILING_WHITESPACE
#)
#execute_process(COMMAND "whoami"
#  OUTPUT_VARIABLE OOMTK_BUILD_USER
#  OUTPUT_STRIP_TRAILING_WHITESPACE
#)

# Standard defines for OOMTK::Sys
#set(OOMTK_DEFINES "-DOOMTK -DOOMTK_VERSION=\"${OOMTK_VERSION}\" -DOOMTK_BUILD_DATE=\"${OOMTK_BUILD_DATE}\" -DOOMTK_BUILD_USER=\"${OOMTK_BUILD_USER}\"")
set(OOMTK_DEFINES "-DOOMTK -DOOMTK_VERSION=\"${OOMTK_VERSION}\" -DOOMTK_BUILD_USER=\"${OOMTK_BUILD_USER}\"")

# Flags

# Generic flags

set(CFLAGS_GENERIC "-static -nostdinc -nostdlib -fno-builtin -fomit-frame-pointer -Wall -Wno-format -nostartfiles -nodefaultlibs -include ${SYS_SOURCE_DIR}/include/macros.h -include ${SYS_SOURCE_DIR}/include/config.h ${OOMTK_DEFINES} -L ${CMAKE_INSTALL_PREFIX}/host/lib -L ${CMAKE_INSTALL_PREFIX}/host/lib/gcc/i386-unknown-oomtk/4.1.1")
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

set(CFLAGS_ia32 "-DIA32 -D__IA32__ -D__ARCH__=ia32 -mpreferred-stack-boundary=2 -falign-functions=4 ${CFLAGS_ia32_subarch}")

endif(OOMTK_ARCH STREQUAL "ia32")
# End of ia32 flags

# Now generate the real C/C++ flags used by CMake
set(CMAKE_C_FLAGS "${CFLAGS_GENERIC} ${CFLAGS_DEBUG} ${CFLAGS_${OOMTK_ARCH}}")
set(CMAKE_CXX_FLAGS "${CFLAGS_GENERIC} ${CFLAGS_DEBUG} ${CFLAGS_${OOMTK_ARCH}} ${CXXFLAGS_ADDONS}")
set(CMAKE_GnuAssembler_FLAGS "${CFLAGS_GENERIC} ${CFLAGS_DEBUG} ${CFLAGS_${OOMTK_ARCH}} ${ASMFLAGS_ADDONS}")

# Release:
set(CMAKE_C_FLAGS_RELEASE "${CFLAGS_GENERIC} ${CFLAGS_RELEASE} ${CFLAGS_${OOMTK_ARCH}}")
set(CMAKE_CXX_FLAGS_RELEASE "${CFLAGS_GENERIC} ${CFLAGS_RELEASE} ${CFLAGS_${OOMTK_ARCH}} ${CXXFLAGS_ADDONS}")
set(CMAKE_GnuAssembler_FLAGS_RELEASE "${CFLAGS_GENERIC} ${CFLAGS_RELEASE} ${CFLAGS_${OOMTK_ARCH}} ${ASMFLAGS_ADDONS}")

# Debug:
set(CMAKE_C_FLAGS_DEBUG "${CFLAGS_GENERIC} ${CFLAGS_DEBUG} ${CFLAGS_${OOMTK_ARCH}}")
set(CMAKE_CXX_FLAGS_DEBUG "${CFLAGS_GENERIC} ${CFLAGS_DEBUG} ${CFLAGS_${OOMTK_ARCH}} ${CXXFLAGS_ADDONS}")
set(CMAKE_GnuAssembler_FLAGS_DEBUG "${CFLAGS_GENERIC} ${CFLAGS_DEBUG} ${CFLAGS_${OOMTK_ARCH}} ${ASMFLAGS_ADDONS}")

# Ok, finished configuring, go build!
include_directories( ${SYS_SOURCE_DIR} ${SYS_BINARY_DIR} ${SYS_SOURCE_DIR}/include )

# minilibc
include_directories( ${SYS_SOURCE_DIR}/libs/minilibc/include
                     ${SYS_SOURCE_DIR}/libs/minilibc/${OOMTK_ARCH}/include
                   )

# libglue
include_directories( ${SYS_SOURCE_DIR}/libs/libglue )

# libc++
# include_directories( ${SYS_SOURCE_DIR}/libs/libc++/include )

# supc++
# include_directories( ${SYS_SOURCE_DIR}/libs/supc++/include )

# Kernel headers
include_directories( ${SYS_SOURCE_DIR}/kernel )

# Devices headers
include_directories( ${SYS_SOURCE_DIR}/devices )

add_subdirectory(boot) # this doesn't work if you don't compile the libraries first!!!
add_subdirectory(devices)
add_subdirectory(kernel)
add_subdirectory(libs)

 add_custom_target(VersionInfo bzr version-info ${OOMTK_SOURCE_DIR} | awk -f ${OOMTK_SOURCE_DIR}/tools/versionInfoProc.awk > ${SYS_BINARY_DIR}/version.h
                   VERBATIM)
