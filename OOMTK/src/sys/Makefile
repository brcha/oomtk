#
#  Copyright (C) 2006 by Filip Brcic <brcha@users.sourceforge.net>
#
#  This file is part of OOMTK
#

# $Id: Makefile 200 2006-11-09 08:07:28Z brcha $

ifeq (,$(filter _%,$(notdir $(CURDIR))))
	include Mk/target.mk
else
# Common part
all::
.PHONY: all

OOMTK_ROOT = $(SYS_SOURCE_DIR)/../..

VPATH	= $(SYS_SOURCE_DIR)

# Include building rules
include $(SYS_SOURCE_DIR)/Mk/variables.mk
include $(SYS_SOURCE_DIR)/Mk/commands.mk
include $(SYS_SOURCE_DIR)/Mk/rules.mk

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

all:: prepare_preprocess $(foreach p, $(PREPROCESS_MODULES), $(p)-preprocessed.stamp) ; @:

# $(foreach p, $(PREPROCESS_MODULES), $(p)-preprocessed.stamp: $($(p)_SOURCES)\n)

prepare_preprocess:
	@($(foreach p, $(PREPROCESS_MODULES), \
		echo "$(p)-preprocessed.stamp: $($(p)_SOURCES)";)) > $(PREP_DIR)/prep.deps

-include $(PREP_DIR)/prep.deps

else
# We have preprocessed everything, now we build
VPATH += $(PREP_DIR)

all:: prepUpdate oomtk updateBuildNumber

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
