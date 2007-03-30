#
#  Copyright (C) 2006-2007 by Filip Brcic <brcha@users.sourceforge.net>
#
#  This file is part of OOMTK
#

set(SYS_SOURCE_DIR "${OOMTK_SOURCE_DIR}/src/sys")
set(SYS_BINARY_DIR "${OOMTK_BINARY_DIR}/src/sys")

# OOMTK Version
set(OOMTK_VERSION "0.80.1")

# OOMTK Architecture (set in the main CMakeLists.txt file)
# supported archs = ia32
#set(ARCH "ia32")

# Cross-gcc prefix
# This will be changed when the xenv is built with cmake
#set(GCCPREFIX "/oomtk/xenv/bin/i386-pc-oomtk0.2-")
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
set(CMAKE_AR "${GCCPREFIX}ar")
set(CMAKE_RANLIB "${GCCPREFIX}ranlib")

# Find out the date
execute_process(COMMAND "date"
  OUTPUT_VARIABLE OOMTK_BUILD_DATE
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
execute_process(COMMAND "whoami"
  OUTPUT_VARIABLE OOMTK_BUILD_USER
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Standard defines for OOMTK::Sys
set(OOMTK_DEFINES "-DOOMTK -DOOMTK_VERSION=\"${OOMTK_VERSION}\" -DOOMTK_BUILD_DATE=\"${OOMTK_BUILD_DATE}\" -DOOMTK_BUILD_USER=\"${OOMTK_BUILD_USER}\"")

# Flags

# Generic flags

set(CFLAGS_GENERIC "-static -nostdinc -nostdlib -fno-builtin -fomit-frame-pointer -Wall -Wno-format -nostartfiles -nodefaultlibs -include ${SYS_SOURCE_DIR}/include/macros.h -include ${SYS_SOURCE_DIR}/include/config.h")
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
  set(CFLAGS_ia32_subarch "-march=pentium4 -msse3 -D__SUBARCH__=Pentium4 -DSSE3 -D__SSE3__")
endif(IA32_sse3)

set(CFLAGS_ia32 "-DIA32 -D__IA32__ -D__ARCH__=ia32 -mprefered-stack-boundary=2 -falign-functions=4 ${CFLAGS_ia32_subarch}")

endif(OOMTK_ARCH STREQUAL "ia32")
# End of ia32 flags

# Now generate the real C/C++ flags used by CMake
set(CMAKE_C_FLAGS "${CFLAGS_GENERIC} ${CFLAGS_DEBUG} ${CFLAGS_${OOMTK_ARCH}}")
set(CMAKE_CXX_FLAGS "${CFLAGS_GENERIC} ${CFLAGS_DEBUG} ${CFLAGS_${OOMTK_ARCH}} ${CXXFLAGS_ADDONS}")
set(CMAKE_ASM_FLAGS "${CFLAGS_GENERIC} ${CFLAGS_DEBUG} ${CFLAGS_${OOMTK_ARCH}} ${ASMFLAGS_ADDONS}")

# Release:
set(CMAKE_C_FLAGS_RELEASE "${CFLAGS_GENERIC} ${CFLAGS_RELEASE} ${CFLAGS_${OOMTK_ARCH}}")
set(CMAKE_CXX_FLAGS_RELEASE "${CFLAGS_GENERIC} ${CFLAGS_RELEASE} ${CFLAGS_${OOMTK_ARCH}} ${CXXFLAGS_ADDONS}")
set(CMAKE_ASM_FLAGS_RELEASE "${CFLAGS_GENERIC} ${CFLAGS_RELEASE} ${CFLAGS_${OOMTK_ARCH}} ${ASMFLAGS_ADDONS}")

# Debug:
set(CMAKE_C_FLAGS_DEBUG "${CFLAGS_GENERIC} ${CFLAGS_DEBUG} ${CFLAGS_${OOMTK_ARCH}}")
set(CMAKE_CXX_FLAGS_DEBUG "${CFLAGS_GENERIC} ${CFLAGS_DEBUG} ${CFLAGS_${OOMTK_ARCH}} ${CXXFLAGS_ADDONS}")
set(CMAKE_ASM_FLAGS_DEBUG "${CFLAGS_GENERIC} ${CFLAGS_DEBUG} ${CFLAGS_${OOMTK_ARCH}} ${ASMFLAGS_ADDONS}")

# # PREP_FLAGS	:= -t -L
# # CPPFLAGS	:= -include $(TOP)/include/macros.h -include $(TOP)/include/config.h -nostdinc
# # CFLAGS		:= $(CPPFLAGS) -O2 -fno-builtin -fomit-frame-pointer -MD -Wall -Wno-format -ggdb
# # CXXFLAGS	:= $(CFLAGS) -fno-exceptions
# #
# # ARFLAGS		:= crs
# #
# # INCS		:= $(TOP) $(TOP)/include
# # DEFS		:= __ARCH__=$(ARCH)
#
# # Flags
# # set(PREP_FLAGS -t -L)
# set(CPPFLAGS "-include ${SYS_SOURCE_DIR}/include/macros.h -include ${SYS_SOURCE_DIR}/include/config.h -DOOMTK_VERSION=${OOMTK_VERSION} -D__ARCH__=${ARCH} -nostdinc")
# set(CMAKE_C_FLAGS "${CPPFLAGS} -O2 -fno-builtin -fomit-frame-pointer -Wall -Wno-format -ggdb -nostdlib -nostartfiles -nodefaultlibs")
# set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -fno-exceptions")
# # set(CMAKE_GnuAssembler_FLAGS "${CMAKE_C_FLAGS}")
#
# include_directories( ${SYS_SOURCE_DIR} ${SYS_SOURCE_DIR}/include )
#
# # Add the include directories and libraries
# # libDisplay:
# # include_directories(${SYS_SOURCE_DIR}/libs/libDisplay/include
# #                     ${SYS_BINARY_DIR}/libs/libDisplay)
# # set(libDisplay "-L${SYS_BINARY_DIR}/libs/libDisplay -lDisplay")
#
# # Libraries
# # add_subdirectory(libs)
#
# # Bootstrap
# # add_subdirectory(boot)
#
# # set(CMAKE_CXX_LINK_EXECUTABLE
# #  "<CMAKE_CXX_COMPILER> <FLAGS> -nostdlib -nostartfiles -nodefaultlibs -fno-rtti -fno-builtin -fno-exceptions -fomit-frame-pointer -Xlinker -T -Xlinker ${SYS_SOURCE_DIR}/boot/${ARCH}/linker.ld <OBJECTS> -o <TARGET>"
# # )
#
# # set(oomtk_SOURCES
# #       ${boot_SOURCES}
# #       ${libDisplay_SOURCES}
# #    )
#
# # add_executable(oomtk ${oomtk_SOURCES})
# # set_target_properties(oomtk PROPERTIES LINK_LANGUAGE CXX)
#
#
