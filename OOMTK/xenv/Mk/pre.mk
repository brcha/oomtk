# Where is OOMTK root?
OOMTK_SRC	?= $(shell pwd)/../..
OOMTK_DIR	?= /oomtk

# Include configuration options
#include $(OOMTK_SRC)/Mk/config.mk

# Provide empty variables
TARBZ2=
PATCH=
CONFIGURE_OPTS=
NAME=

# All is first target!
all:
