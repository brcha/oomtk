#
#  Copyright (C) 2006 by Filip Brcic <brcha@users.sourceforge.net>
#
#  This file is part of OOMTK
#

ifeq (,$(filter _%,$(notdir $(CURDIR))))
	include Mk/target.mk
else
# Common part
all::
.PHONY: all

help:
	@echo "OOMTK Build System"
	@echo ""
	@echo "Possible build targets are:"
	@echo " menuconfig        - configure kernel (curses interface)"
	@echo " config            - the same as menuconfig"
	@echo " xconfig           - configure kernel (tk interface)"
	@echo " all               - build the kernel"
	@echo " clean             - delete all built files"
.PHONY: help

OOMTK_ROOT = $(SYS_SOURCE_DIR)/../..

VPATH	= $(SYS_SOURCE_DIR)

# Include the configured options
-include $(SYS_SOURCE_DIR)/globalconfig.out

# Include building rules
include $(SYS_SOURCE_DIR)/Mk/variables.mk
include $(SYS_SOURCE_DIR)/Mk/commands.mk
include $(SYS_SOURCE_DIR)/Mk/rules.mk

# If not configuring
#ifeq ($(filter config xconfig menuconfig oldconfig,$(MAKECMDGOALS)),)

ifeq ("$(CONFIG_ARCH)","")

all:: menuconfig
	@echo "Run make again to build!!!"
	@exit 1

endif

$(SYS_SOURCE_DIR)/rules.out: $(SYS_SOURCE_DIR)/rules.cml
	@echo "+---------------------------------------------------+"
	@echo "| rules.out are out-of-date. Do \'make menuconfig\' |"
	@echo "+---------------------------------------------------+"
	$(CML2_COMPILE) -o $@ $<

$(SYS_SOURCE_DIR)/globalconfig.out: $(SYS_SOURCE_DIR)/rules.out
	$(CML2_CONFIGURE) -b -B $(CML2_BANNER) -c -i $(SYS_SOURCE_DIR)/globalconfig.out -o $(SYS_SOURCE_DIR)/globalconfig.out $<

$(SYS_SOURCE_DIR)/globalconfig.h: $(SYS_SOURCE_DIR)/globalconfig.out
	$(CML2_CONFIGTRANS) -h $@ $<

config menuconfig: $(SYS_SOURCE_DIR)/rules.out
	$(CML2_CONFIGURE) -B $(CML2_BANNER) -c -i $(SYS_SOURCE_DIR)/globalconfig.out -o $(SYS_SOURCE_DIR)/globalconfig.out $<
.PHONY: config menuconfig

oldconfig: $(SYS_SOURCE_DIR)/rules.out
	$(CML2_CONFIGURE) -B $(CML2_BANNER) -t -I $(SYS_SOURCE_DIR)/globalconfig.out -o $(SYS_SOURCE_DIR)/globalconfig.out $<
.PHONY: oldconfig

regenconfig: $(SYS_SOURCE_DIR)/rules.out
	$(CML2_CONFIGURE) -B $(CML2_BANNER) -b -I $(SYS_SOURCE_DIR)/globalconfig.out -o $(SYS_SOURCE_DIR)/globalconfig.out $<
.PHONY: regenconfig

xconfig: $(SYS_SOURCE_DIR)/rules.out
	$(CML2_CONFIGURE) -x -i $(SYS_SOURCE_DIR)/globalconfig.out -o $(SYS_SOURCE_DIR)/globalconfig.out $<
.PHONY: xconfig

# Define directories where are the sources
SUBDIRECTORIES  = boot/$(ARCH_CONFIG_LC)
SUBDIRECTORIES += libs/minilibc
# SUBDIRECTORIES += libs/cxxlib
# SUBDIRECTORIES += libs/pdclib
SUBDIRECTORIES += libs/supc++
SUBDIRECTORIES += libs/libc++
SUBDIRECTORIES += devices
SUBDIRECTORIES += kernel

# Initialize the variables
INCLUDE_DIRS := $(SYS_SOURCE_DIR)/include
INCLUDE_DIRS += $(PREP_DIR)
PREPROCESS_STAMPS :=

-include $(foreach dir, $(SUBDIRECTORIES), $(SYS_SOURCE_DIR)/$(dir)/Part.mk)

INCLUDES := $(addprefix -I,$(INCLUDE_DIRS))

ifeq (_prep,$(notdir $(CURDIR)))
# We are now preprocessing files
.SUFFIXES:
.DEFAULT: ; @:

all:: prep.deps $(foreach p, $(PREPROCESS_MODULES), $(p)-preprocessed.stamp) ; @:

# $(foreach p, $(PREPROCESS_MODULES), $(p)-preprocessed.stamp: $($(p)_SOURCES)\n)

prep.deps: $(foreach p, $(PREPROCESS_MODULES), $($(p)_SOURCES))
	@($(foreach p, $(PREPROCESS_MODULES), \
		echo "$(p)-preprocessed.stamp: $($(p)_SOURCES) prep.deps";)) > prep.deps

-include prep.deps

else
# We have preprocessed everything, now we build
VPATH += $(PREP_DIR)

all:: $(SYS_SOURCE_DIR)/globalconfig.h prepUpdate oomtk updateBuildNumber

.PHONY: prepUpdate
prepUpdate:
	@[ -f oomtk ] && cp -f oomtk oomtk.old || true

.PHONY: updateBuildNumber
updateBuildNumber:
	@( diff oomtk oomtk.old&>/dev/null || sed -i "s/OOMTK_BUILD := $(OOMTK_BUILD)/OOMTK_BUILD := $$(($(OOMTK_BUILD)+1))/" $(SYS_SOURCE_DIR)/Mk/variables.mk && true)
	@rm -f oomtk.old

# Include dependencies
-include $(wildcard $(BUILD_DIR)/*.d)

endif
endif


