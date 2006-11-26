#
#  Copyright (C) 2006 by Filip Brcic <brcha@users.sourceforge.net>
#
#  This file is part of OOMTK
#

# $Id: $

VPATH += $(SYS_SOURCE_DIR)/boot/$(ARCH_CONFIG_LC)

boot_ia32_SOURCES := start.S init.cc
boot_ia32_OBJECTS := $(call sources2objects,$(boot_ia32_SOURCES))

boot_ia32_ctors_SOURCES := ctors.cpp
PREPROCESS_MODULES	+= boot_ia32_ctors
boot_ia32_OBJECTS	+= $(call sources2objects,$(boot_ia32_ctors_SOURCES))

oomtk: linker.ia32.ld $(boot_ia32_OBJECTS) minilibc.a cxxlib.a Devices.a
	$(LD_MESSAGE)
	$(LD) -N -o $@ -T $< -gc-sections $(filter-out $<,$^) #$(LIBSUPCPP)
