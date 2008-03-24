#
#  Copyright (C) 2006 by Filip Brcic <brcha@users.sourceforge.net>
#
#  This file is part of OOMTK
#

# $Id: $

libc++_SOURCE_DIR := $(SYS_SOURCE_DIR)/libs/libc++

VPATH += $(libc++_SOURCE_DIR)
INCLUDE_DIRS += $(libc++_SOURCE_DIR)/include

libc++_SOURCES	:= ios.cc

libc++_OBJECTS	:= $(call sources2objects,$(libc++_SOURCES))

libc++.a: $(libc++_OBJECTS)