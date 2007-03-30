#
#  Copyright (C) 2006 by Filip Brcic <brcha@users.sourceforge.net>
#
#  This file is part of OOMTK
#

VPATH += $(SYS_SOURCE_DIR)/boot/$(ARCH_CONFIG_LC)

boot_ia32_SOURCES := start.S init.cc
boot_ia32_OBJECTS := $(call sources2objects,$(boot_ia32_SOURCES))

# boot_ia32_ctors_SOURCES := ctors.cpp
# PREPROCESS_MODULES	+= boot_ia32_ctors
# boot_ia32_OBJECTS	+= $(call sources2objects,$(boot_ia32_ctors_SOURCES))

boot_ia32_CPUID_SOURCES	:= CPUID.cpp
PREPROCESS_MODULES	+= boot_ia32_CPUID
boot_ia32_OBJECTS	+= $(call sources2objects,$(boot_ia32_CPUID_SOURCES))

# Paging support
paging_Paging_SOURCES	:= Paging.cpp
PREPROCESS_MODULES	+= paging_Paging
boot_ia32_OBJECTS	+= $(call sources2objects,$(paging_Paging_SOURCES))

paging_Legacy_SOURCES	:= PagingLegacy.cpp
PREPROCESS_MODULES	+= paging_Legacy
boot_ia32_OBJECTS	+= $(call sources2objects,$(paging_Legacy_SOURCES))

paging_PAE_SOURCES	:= PagingPAE.cpp
PREPROCESS_MODULES	+= paging_PAE
boot_ia32_OBJECTS	+= $(call sources2objects,$(paging_PAE_SOURCES))

# GDT
gdt_SOURCES		:= GDT.cpp
PREPROCESS_MODULES	+= gdt
boot_ia32_OBJECTS	+= $(call sources2objects,$(gdt_SOURCES))

linker.ia32.ld: linker.ia32.ld.S
	$(CPP_MESSAGE)
	$(CPP_BUILD)

oomtk: linker.ia32.ld $(boot_ia32_OBJECTS) minilibc.a supc++.a libc++.a Devices.a Kernel.a $(LIBGCC)
	$(LD_MESSAGE)
	$(LD) -N -o $@ -T $< -gc-sections $(filter-out $<,$^)
