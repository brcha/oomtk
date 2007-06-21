#
#  Copyright (C) 2006 by Filip Brcic <brcha@users.sourceforge.net>
#
#  This file is part of OOMTK
#

# $Id: $

minilibc_SOURCE_DIR := $(SYS_SOURCE_DIR)/libs/minilibc

VPATH += $(minilibc_SOURCE_DIR) $(minilibc_SOURCE_DIR)/$(ARCH_CONFIG_LC)
INCLUDE_DIRS += $(minilibc_SOURCE_DIR)/include
INCLUDE_DIRS += $(minilibc_SOURCE_DIR)/$(ARCH_CONFIG_LC)/include

minilibc_SOURCES := atexit.c memccpy.c memcmp.c memmove.c memset.c memcpy.c \
                    memchr.c strchr.c strcmp.c strcpy.c strlen.c strncmp.c \
                    strncpy.c strstr.c __assert_fail.c printf.c __v_printf.c \
                    vprintf.c strtol.c strtoul.c lltostr.c __ltostr.c \
                    putchar.c puts.c getchar.c gets.c sprintf.c snprintf.c \
                    vsnprintf.c vsprintf.c longjmp.c isalnum.c isalpha.c \
                    isascii.c isblank.c iscntrl.c isdigit.c isgraph.c islower.c \
                    isprint.c ispunct.c isspace.c isupper.c isxdigit.c tolower.c \
                    strcspn.c strspn.c panic.c setjmp.S

minilibc_OBJECTS := $(addsuffix .o,$(basename $(minilibc_SOURCES)))

minilibc.a: $(minilibc_OBJECTS)