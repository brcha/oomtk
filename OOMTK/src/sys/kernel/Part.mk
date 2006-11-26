#
#  Copyright (C) 2006 by Filip Brcic <brcha@users.sourceforge.net>
#
#  This file is part of OOMTK
#

VPATH += $(SYS_SOURCE_DIR)/kernel $(SYS_SOURCE_DIR)/kernel/$(ARCH_CONFIG_LC)

kernel_fatal_SOURCES	:= fatal.cpp
PREPROCESS_MODULES	+= kernel_fatal
kernel_OBJECTS		:= $(call sources2objects,$(kernel_fatal_SOURCES))

Kernel.a: $(kernel_OBJECTS)
