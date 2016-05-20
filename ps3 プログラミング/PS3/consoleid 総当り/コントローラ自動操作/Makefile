#   SCE CONFIDENTIAL
#   PlayStation(R)3 Programmer Tool Runtime Library 400.001
#   Copyright (C) 2006 Sony Computer Entertainment Inc.
#   All Rights Reserved.

CELL_MK_DIR ?= $(CELL_SDK)/samples/mk
include $(CELL_MK_DIR)/sdk.makedef.mk

PPU_SRCS		=	SampleApp.cpp
PPU_TARGET		=	CustomController.elf

SUBDIRS			=	$(CELL_FW_DIR)

PPU_INCDIRS		+=	-I$(CELL_FW_DIR)/include -I$(CELL_FW_DIR)/include/gcm


PPU_CXXFLAGS 	+=	-fno-rtti -fno-exceptions
PPU_LIBS		+=	$(CELL_FW_DIR)/libfwgcm.a \
				$(CELL_FW_DIR)/libfw.a 

PPU_LDLIBS	 	+=	-lusbd_stub -lio_stub \
				-lgcm_cmd -lgcm_sys_stub -lsysutil_stub -lsysmodule_stub \
				-lm -lstdc++

include $(CELL_MK_DIR)/sdk.target.mk

