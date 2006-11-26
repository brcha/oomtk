#
#  Copyright (C) 2006 by Filip Brcic <brcha@users.sourceforge.net>
#
#  This file is part of OOMTK
#

# $Id: $

VPATH += $(SYS_SOURCE_DIR)/devices $(SYS_SOURCE_DIR)/devices/$(ARCH_CONFIG_LC)

devices_CPU_SOURCES		:= CPU.cpp CPU-$(ARCH_CONFIG_LC).cpp
PREPROCESS_MODULES		+= devices_CPU
devices_OBJECTS			:= $(call sources2objects,$(devices_CPU_SOURCES))

devices_PortIO_SOURCES		:= PortIO.cpp PortIO-$(ARCH_CONFIG_LC).cpp
PREPROCESS_MODULES		+= devices_PortIO
devices_OBJECTS			+= $(call sources2objects,$(devices_PortIO_SOURCES))

devices_Console_SOURCES 	:= Console.cpp
PREPROCESS_MODULES		+= devices_Console
devices_OBJECTS			:= $(call sources2objects,$(devices_Console_SOURCES))

devices_VgaConsole_SOURCES	:= VgaConsole.cpp
PREPROCESS_MODULES		+= devices_VgaConsole
devices_OBJECTS			+= $(call sources2objects,$(devices_VgaConsole_SOURCES))

devices_SOURCES			:= libc_backend.cc
devices_OBJECTS			+= $(call sources2objects,$(devices_SOURCES))

Devices.a: $(devices_OBJECTS)
