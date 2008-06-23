#
#  Copyright (C) 2007 by Filip Brcic <brcha@users.sourceforge.net>
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

include_directories(${CMAKE_CURRENT_SOURCE_DIR}
                    ${CMAKE_CURRENT_SOURCE_DIR}/include
                    ${CMAKE_CURRENT_SOURCE_DIR}/${OOMTK_ARCH}
                    ${CMAKE_CURRENT_SOURCE_DIR}/${OOMTK_ARCH}/include
                   )

set(minilibc_SOURCES
    atexit.c memccpy.c memcmp.c memmove.c memset.c memcpy.c
    memchr.c strchr.c strcmp.c strcpy.c strlen.c strncmp.c
    strncpy.c strstr.c __assert_fail.c printf.c __v_printf.c
    vprintf.c strtol.c strtoul.c __ltostr.c
    putchar.c puts.c getchar.c gets.c sprintf.c snprintf.c
    vsnprintf.c vsprintf.c longjmp.c isalnum.c isalpha.c
    isascii.c isblank.c iscntrl.c isdigit.c isgraph.c islower.c
    isprint.c ispunct.c isspace.c isupper.c isxdigit.c tolower.c
    strcspn.c strspn.c panic.c
    ${OOMTK_ARCH}/lltostr.c
    ${OOMTK_ARCH}/setjmp.S
   )
# this solution will not work for arm, but we'll se then what we'll do - maybe change the libc(?)

add_library(minilibc STATIC ${minilibc_SOURCES})
