#
#  Copyright (C) 2006 by Filip Brcic <brcha@users.sourceforge.net>
#
#  This file is part of OOMTK
#

# $Id: $

supc++_SOURCE_DIR := $(SYS_SOURCE_DIR)/libs/supc++

VPATH += $(supc++_SOURCE_DIR)
INCLUDE_DIRS += $(supc++_SOURCE_DIR)/include

supc++_SOURCES	:= initialize.cc memory.cc stdexcept.cc finalize.cc
supc++_SOURCES	+= purevirtual.cc typeinfo.cc guard.cc

supc++_OBJECTS	:= $(call sources2objects,$(supc++_SOURCES))

supc++.a: $(supc++_OBJECTS)