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
if(OOMTK_ARCH EQUAL "ia32")
  set(GCCPREFIX "${CROSS_BIN_PREFIX}/i386-unknown-oomtk-")
else(OOMTK_ARCH EQUAL "ia32")
  MESSAGE(FATAL_ERROR "Unsupported architecture :(")
endif(OOMTK_ARCH EQUAL "ia32")

# Cross-gcc tools
set(CMAKE_C_COMPILER "${GCCPREFIX}gcc")
set(CMAKE_COMPILER_IS_GNUCC 1)
set(CMAKE_CXX_COMPILER "${GCCPREFIX}g++")
set(CMAKE_COMPILER_IS_GNUCXX 1)
set(CMAKE_AR "${GCCPREFIX}ar")
set(CMAKE_RANLIB "${GCCPREFIX}ranlib")

# PREP_FLAGS	:= -t -L
# CPPFLAGS	:= -include $(TOP)/include/macros.h -include $(TOP)/include/config.h -nostdinc
# CFLAGS		:= $(CPPFLAGS) -O2 -fno-builtin -fomit-frame-pointer -MD -Wall -Wno-format -ggdb
# CXXFLAGS	:= $(CFLAGS) -fno-exceptions
#
# ARFLAGS		:= crs
#
# INCS		:= $(TOP) $(TOP)/include
# DEFS		:= __ARCH__=$(ARCH)

# Flags
# set(PREP_FLAGS -t -L)
set(CPPFLAGS "-include ${SYS_SOURCE_DIR}/include/macros.h -include ${SYS_SOURCE_DIR}/include/config.h -DOOMTK_VERSION=${OOMTK_VERSION} -D__ARCH__=${ARCH} -nostdinc")
set(CMAKE_C_FLAGS "${CPPFLAGS} -O2 -fno-builtin -fomit-frame-pointer -Wall -Wno-format -ggdb -nostdlib -nostartfiles -nodefaultlibs")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -fno-exceptions")
# set(CMAKE_GnuAssembler_FLAGS "${CMAKE_C_FLAGS}")

include_directories( ${SYS_SOURCE_DIR} ${SYS_SOURCE_DIR}/include )

# Add the include directories and libraries
# libDisplay:
# include_directories(${SYS_SOURCE_DIR}/libs/libDisplay/include
#                     ${SYS_BINARY_DIR}/libs/libDisplay)
# set(libDisplay "-L${SYS_BINARY_DIR}/libs/libDisplay -lDisplay")

# Libraries
# add_subdirectory(libs)

# Bootstrap
# add_subdirectory(boot)

# set(CMAKE_CXX_LINK_EXECUTABLE
#  "<CMAKE_CXX_COMPILER> <FLAGS> -nostdlib -nostartfiles -nodefaultlibs -fno-rtti -fno-builtin -fno-exceptions -fomit-frame-pointer -Xlinker -T -Xlinker ${SYS_SOURCE_DIR}/boot/${ARCH}/linker.ld <OBJECTS> -o <TARGET>"
# )

# set(oomtk_SOURCES
#       ${boot_SOURCES}
#       ${libDisplay_SOURCES}
#    )

# add_executable(oomtk ${oomtk_SOURCES})
# set_target_properties(oomtk PROPERTIES LINK_LANGUAGE CXX)


