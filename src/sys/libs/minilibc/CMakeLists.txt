#
#  Copyright (C) 2007 by Filip Brcic <brcha@users.sourceforge.net>
#
#  This file is part of OOMTK
#

# $ Id: $

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