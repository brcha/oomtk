#
#  Copyright (C) 2006 by Filip Brcic <brcha@users.sourceforge.net>
#
#  This file is part of OOMTK
#

# $Id: $

ifndef OOMTK_COMMANDS_MK
OOMTK_COMMANDS_MK := 1

#
# This file defines commands used for compilation
#

##                                           ##
#  Kernel config = RELEASE, DEBUG or PROFILE  #
##                                           ##
KERNEL_CONFIG	?= RELEASE

##                              ##
#  Arch config = IA32, ARM, PPC  #
#  (note: currently only ia32)   #
##                              ##
ARCH_CONFIG	?= IA32
ARCH_CONFIG	:= $(shell echo -n $(ARCH_CONFIG) | tr a-z A-Z)
ARCH_CONFIG_LC	:= $(shell echo -n $(ARCH_CONFIG) | tr A-Z a-z)

##                            ##
#  Standard defines for OOMTK  #
##                            ##
DEFINES	:= -DOOMTK -DOOMTK_VERSION="\"$(OOMTK_VERSION)\"" -DOOMTK_BUILD="\"$(OOMTK_BUILD)\"" -DOOMTK_BUILD_DATE="\"$(shell date)\"" -DOOMTK_BUILD_USER="\"$(shell whoami)\""

##                          ##
#  OOMTK Toolchain Commands  #
##                          ##

V	?= @
OOMTK_TOOLCHAIN_PREFIX	?= /oomtk/host/bin/i386-pc-oomtk-

CCACHE	:= `type -p ccache`
CC	:= $(V)$(CCACHE) $(OOMTK_TOOLCHAIN_PREFIX)gcc
CXX	:= $(V)$(CCACHE) $(OOMTK_TOOLCHAIN_PREFIX)g++
AR	:= $(V)$(OOMTK_TOOLCHAIN_PREFIX)ar
RANLIB	:= $(V)$(OOMTK_TOOLCHAIN_PREFIX)ranlib
LD	:= $(V)$(OOMTK_TOOLCHAIN_PREFIX)ld
PREP    := $(V)perl $(OOMTK_ROOT)/tools/preprocess/src/preprocess
MOVE_IF_CHANGE	:= sh $(OOMTK_ROOT)/tools/move-if-change

LIBGCC	:= $(shell $(OOMTK_TOOLCHAIN_PREFIX)g++ -print-libgcc-file-name)
LIBSUPCPP	:= $(shell $(OOMTK_TOOLCHAIN_PREFIX)g++ -print-file-name=libsupc++.a)

##                       ##
#  OOMTK Toolchain Flags  #
##                       ##

# Generic flags
CFLAGS_GEN	:= -static -include $(SYS_SOURCE_DIR)/include/macros.h -include $(SYS_SOURCE_DIR)/include/config.h -nostdinc -nostdlib -fno-builtin -fomit-frame-pointer -Wall -Wno-format -nostartfiles -nodefaultlibs -I$(wildcard $(dir $(LIBGCC))/include $(dir $(LIBGCC))/../include)
CXXFLAGS_ADDONS	:= -fno-exceptions -fno-rtti
ASMFLAGS_ADDONS	:= -D__ASSEMBLER__

CFLAGS_RELEASE	:=
CFLAGS_DEBUG	:=
CFLAGS_PROFILE	:= -pg

PREP_TAGS_RELEASE	:=
PREP_TAGS_DEBUG		:= debug
PREP_TAGS_PROFILE	:= profile

# IA32 flags
CFLAGS_IA32	:= -DIA32 -DIA32 -D__IA32__ -D__ARCH__=ia32 -march=i686 -mpreferred-stack-boundary=2 -falign-functions=4 -mtune=pentium4

PREP_TAGS_IA32	:= ia32

CFLAGS_IA32RELEASE	:= -O2
CFLAGS_IA32DEBUG	:= -O2
CFLAGS_IA32PROFILE	:= -O2

# Complete {C,CXX,AS}_FLAGS

CFLAGS		= ${CFLAGS_GEN} $($(addsuffix $(ARCH_CONFIG),CFLAGS_)) $($(addsuffix $(KERNEL_CONFIG),CFLAGS_)) $($(addsuffix $(KERNEL_CONFIG), $(addsuffix $(ARCH_CONFIG),CFLAGS_))) $(DEFINES) $(INCLUDES)
CXXFLAGS	= $(CFLAGS) $(CXXFLAGS_ADDONS)
ASMFLAGS	= $(CFLAGS) $(ASMFLAGS_ADDONS)
PREPFLAGS       = -t -L -i
PREP_TAGS	= $($(addsuffix $(ARCH_CONFIG),PREP_TAGS_)) $($(addsuffix $(KERNEL_CONFIG),PREP_TAGS_))
ARFLAGS         = r

# Dependencies
C_DEP	=$(CC) $(CFLAGS) -M -MT $@ -MF $(BUILD_DIR)/$(patsubst %.o,%.d,$(notdir $@)) $<
CXX_DEP	=$(CXX) $(CXXFLAGS) -M -MT $@ -MF $(BUILD_DIR)/$(patsubst %.o,%.d,$(notdir $@)) $<
ASM_DEP	=$(CC) $(ASMFLAGS) -M -MT $@ -MF $(BUILD_DIR)/$(patsubst %.o,%.d,$(notdir $@)) $<

# Basename of implementation files = Basename of first .cpp file
impl_name = $(basename $(firstword $(notdir $(1))))

# Building
C_BUILD		=$(CC) $(CFLAGS) -c $< -o $@
CPP_BUILD	=$(CC) -E -P $(CFLAGS) $< -o $@
CXX_BUILD	=$(CXX) $(CXXFLAGS) -c $< -o $@
ASM_BUILD	=$(CC) $(ASMFLAGS) -c $< -o $@
PREP_BUILD	=$(PREP) $(PREPFLAGS) -e "$(PREP_TAGS)" -s \
			-c $(call impl_name, $^) -o new_$(call impl_name, $^) \
			$(filter %.cpp, $^)
AR_BUILD        =$(AR) $(ARFLAGS) $@ $^ &>/dev/null

# Messages
C_MESSAGE	=$(V)echo -e "    \033[01;35m CC    \033[01;37m $< \033[00m"
CPP_MESSAGE	=$(V)echo -e "    \033[01;36m CPP   \033[01;37m $< \033[00m"
CXX_MESSAGE	=$(V)echo -e "    \033[01;33m C++   \033[01;37m $< \033[00m"
ASM_MESSAGE	=$(V)echo -e "    \033[01;32m ASM   \033[01;37m $< \033[00m"
PREP_MESSAGE	=$(V)echo -e "    \033[01;36m PREP  \033[01;37m $< \033[00m"
LD_MESSAGE	=$(V)echo -e "    \033[01;31m LD    \033[01;37m $@ \033[00m"
AR_MESSAGE      =$(V)echo -e "    \033[01;34m AR    \033[01;37m $@ \033[00m"

# Generate object files from sources
sources2objects	= $(addsuffix .o,$(basename $(notdir $(1))))

endif # OOMTK_COMMANDS_MK
