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

project(OOMTK)

# where to look first for cmake modules, before ${CMAKE_ROOT}/Modules/ is checked
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")
enable_language(C CXX GnuAssembler)

# Find some utility programs
find_package(UnixCommands REQUIRED)
find_package(Grep REQUIRED)
find_package(Sed REQUIRED)

# Setup some basic build information

set(CMAKE_INSTALL_PREFIX "/oomtk" CACHE PATH "Where is the root directory of the OOMTK installation? Please note that the cross-compilers don't build using CMake, so you have to install them yourself. Makefiles are provided and they install cross-compilers into /oomtk." FORCE)

set(OOMTK_ARCH "ia32" CACHE STRING "CPU Architecture - this should be chooser, but since this is the only architecture supported right now, I'll leave it like this :)")

add_subdirectory(src)
