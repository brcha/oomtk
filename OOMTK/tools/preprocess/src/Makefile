#
# Makefile for the preprocess sources
#

# Where is OOMTK root?
OOMTK_SRC	?= $(shell pwd)/../../..

# Include configuration options
include $(OOMTK_SRC)/Mk/config.mk

TARGET = preprocess

all:: $(TARGET)

install:: all
	-$(INSTALL) -d $(OOMTK_DIR)/tool/bin
	$(INSTALL) -m 755 $(TARGET) $(OOMTK_DIR)/tool/bin

clean:: 
#	$(RM) $(TARGET) $(OBJS) tmp.*

cleanall::      clean
	$(RM) *~ .*.d

#include $(DEPS)
