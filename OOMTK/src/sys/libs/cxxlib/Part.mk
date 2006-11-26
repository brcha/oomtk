#
#  Copyright (C) 2006 by Filip Brcic <brcha@users.sourceforge.net>
#
#  This file is part of OOMTK
#

# $Id: $

VPATH += $(SYS_SOURCE_DIR)/libs/cxxlib

cxxlib_s_cruft_SOURCES	:= s_cruft.cpp
PREPROCESS_MODULES	+= cxxlib_s_cruft
cxxlib_OBJECTS		:= $(call sources2objects,$(cxxlib_s_cruft_SOURCES))

cxxlib_paranoia_SOURCES	:= paranoia.cpp
PREPROCESS_MODULES	+= cxxlib_paranoia
cxxlib_OBJECTS		+= $(call sources2objects,$(cxxlib_paranoia_SOURCES))

cxxlib.a: $(cxxlib_OBJECTS)
