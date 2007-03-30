#
#  Copyright (C) 2006 by Filip Brcic <brcha@users.sourceforge.net>
#
#  This file is part of OOMTK
#

ifndef OOMTK_TARGET_MK
OOMTK_TARGET_MK := 1

.SUFFIXES:

##                                     ##
#  Define the build & prep directories  #
##                                     ##

BUILD_DIR	?= $(CURDIR)/_build
PREP_DIR	?= $(CURDIR)/_prep

##                                                       ##
#  Ensure that everything goes on in the build directory  #
##                                                       ##

MAKETARGET=$(MAKE) --no-print-directory -C $@ -f $(CURDIR)/Makefile SYS_SOURCE_DIR=$(CURDIR) PREP_DIR=$(PREP_DIR) BUILD_DIR=$(BUILD_DIR) $(MAKECMDGOALS)

.PHONY: $(BUILD_DIR) $(PREP_DIR)
$(BUILD_DIR) $(PREP_DIR):
	+@[ -d $@ ] || mkdir -p $@
	+@$(MAKETARGET)

$(BUILD_DIR): $(PREP_DIR)

# Don't rebuild Makefiles :)
Makefile : ;
%.mk :: ;

# Anything we don't know how to build will depend on this rule
% :: $(PREP_DIR) $(BUILD_DIR) ;

.PHONY: clean
clean:
	$(if $(PREP_DIR),@rm -f $(PREP_DIR)/*)
	@rm -rf $(BUILD_DIR)

endif # OOMTK_TARGET_MK
