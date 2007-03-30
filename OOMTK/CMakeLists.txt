#
#  Copyright (C) 2006-2007 by Filip Brcic <brcha@users.sourceforge.net>
#
#  This file is part of OOMTK
#

project(OOMTK)

# where to look first for cmake modules, before ${CMAKE_ROOT}/Modules/ is checked
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")
enable_language(C CXX)

# Setup some basic build information

set(CMAKE_INSTALL_PREFIX "/oomtk" CACHE PATH "Where is the root directory of
the OOMTK installation? Please note that the cross-compilers don't build using
CMake, so you have to install them yourself. Makefiles are provided and they
install cross-compilers into /oomtk.")

set(OOMTK_ARCH "ia32" CACHE STRING "CPU Architecture - this should be chooser,
but since this is the only architecture supported right now, I'll leave it
like this :)")

add_subdirectory(src)
