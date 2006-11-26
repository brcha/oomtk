#
#  Copyright (C) 2006 by Filip Brcic <brcha@users.sourceforge.net>
#
#  This file is part of OOMTK
#

# $Id: $

VPATH += $(SYS_SOURCE_DIR)/boot/$(ARCH_CONFIG_LC)

boot_ia32_SOURCES := start.S init.cc
boot_ia32_OBJECTS := $(call sources2objects,$(boot_ia32_SOURCES))

# boot_ia32_ctors_SOURCES := ctors.cpp
# PREPROCESS_MODULES	+= boot_ia32_ctors
# boot_ia32_OBJECTS	+= $(call sources2objects,$(boot_ia32_ctors_SOURCES))

boot_ia32_SegmentDescriptor_SOURCES	:= SegmentDescriptor.cpp
PREPROCESS_MODULES	+= boot_ia32_SegmentDescriptor
boot_ia32_OBJECTS	+= $(call sources2objects,$(boot_ia32_SegmentDescriptor_SOURCES))

boot_ia32_CPUID_SOURCES	:= CPUID.cpp
PREPROCESS_MODULES	+= boot_ia32_CPUID
boot_ia32_OBJECTS	+= $(call sources2objects,$(boot_ia32_CPUID_SOURCES))

# Paging support
paging_Paging_SOURCES	:= Paging.cpp
PREPROCESS_MODULES	+= paging_Paging_SOURCES
boot_ia32_OBJECTS	+= $(call sources2objects,$(paging_Paging_SOURCES))

linker.ia32.ld: linker.ia32.ld.S
	$(CPP_MESSAGE)
	$(CPP_BUILD)

oomtk: linker.ia32.ld $(boot_ia32_OBJECTS) minilibc.a supc++.a libc++.a Devices.a Kernel.a
	$(LD_MESSAGE)
	$(LD) -N -o $@ -T $< -gc-sections $(filter-out $<,$^) #$(LIBSUPCPP)
