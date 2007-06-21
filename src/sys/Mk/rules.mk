#
#  Copyright (C) 2006 by Filip Brcic <brcha@users.sourceforge.net>
#
#  This file is part of OOMTK
#

ifndef OOMTK_RULES_MK
OOMTK_RULES_MK := 1

##                           ##
#  Object construction rules  #
##                           ##

%.o: %.S
	$(ASM_MESSAGE)
	$(ASM_DEP)
	$(ASM_BUILD)

%.o: %.c
	$(C_MESSAGE)
	$(C_DEP)
	$(C_BUILD)

%.o: %.cc
	$(CXX_MESSAGE)
	$(CXX_DEP)
	$(CXX_BUILD)

%.a:
	@rm -f $@
	$(AR_MESSAGE)
	$(AR_BUILD)
	$(RANLIB) $@

%-preprocessed.stamp:
	$(PREP_MESSAGE)
	$(PREP_BUILD)
	@(for i in $(call impl_name,$^){.h,_i.h,.cc}; \
	do \
		$(MOVE_IF_CHANGE) new_$$i $$i &>/dev/null || true; \
	done)

endif # OOMTK_RULES_MK
