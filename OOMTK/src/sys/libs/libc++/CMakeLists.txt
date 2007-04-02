#
#  Copyright (C) 2007 by Filip Brcic <brcha@users.sourceforge.net>
#
#  This file is part of OOMTK
#

# $ Id: $

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

set(libc++_SOURCES ios.cc)

add_library(libc++ STATIC ${libc++_SOURCES})