#   SCE CONFIDENTIAL
#   PlayStation(R)3 Programmer Tool Runtime Library 400.001
#   Copyright (C) 2008 Sony Computer Entertainment Inc.
#   All Rights Reserved.

CELL_SDK ?= /usr/local/cell
CELL_MK_DIR ?= $(CELL_SDK)/samples/mk
include $(CELL_MK_DIR)/sdk.makedef.mk



PPU_SRCS	= 1.cpp
PPU_TARGET	= 1.elf
PPU_LDLIBS	+= -lsysutil_stub -lio_stub -lpadfilter padutil.a



include $(CELL_MK_DIR)/sdk.target.mk



