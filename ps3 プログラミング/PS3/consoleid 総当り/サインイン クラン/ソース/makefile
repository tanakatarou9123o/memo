#   SCE CONFIDENTIAL
#   PlayStation(R)3 Programmer Tool Runtime Library 400.001
#   Copyright (C) 2009 Sony Computer Entertainment Inc.
#   All Rights Reserved.

NP_GUI_DEBUG=no
 
CELL_SDK ?= /usr/local/cell
CELL_MK_DIR ?= $(CELL_SDK)/samples/mk
include $(CELL_MK_DIR)/sdk.makedef.mk



PPU_SRCS = $(wildcard *.cpp) $(VPSHADER_PPU_OBJS) $(FPSHADER_PPU_OBJS)
PPU_TARGET = np_gui_clans.ppu.elf

PPU_INCDIRS	+= -I$(CELL_TARGET_PATH)/ppu/include/sysutil
PPU_CPPFLAGS += -D_DEBUG -g

ifeq ($(NP_GUI_DEBUG), yes)
PPU_CPPFLAGS += -DNP_GUI_DEBUG
endif

PPU_LDLIBS += -lm -lsysutil_stub -ldbgfont_gcm -lgcm_cmd -lgcm_sys_stub -lnet_stub -lnetctl_stub -lio_stub -lsysmodule_stub -lsysutil_np_stub -lsysutil_np_clans_stub -lhttp_stub -lssl_stub -lrtc_stub


VPSHADER_SRCS = vpshader.cg
FPSHADER_SRCS = fpshader.cg

VPSHADER_PPU_OBJS = $(patsubst %.cg, $(OBJS_DIR)/%.ppu.o, $(VPSHADER_SRCS))
FPSHADER_PPU_OBJS = $(patsubst %.cg, $(OBJS_DIR)/%.ppu.o, $(FPSHADER_SRCS))

include $(CELL_MK_DIR)/sdk.target.mk

PPU_OBJS += $(VPSHADER_PPU_OBJS) $(FPSHADER_PPU_OBJS)

$(VPSHADER_PPU_OBJS): $(OBJS_DIR)/%.ppu.o : %.vpo
	@mkdir -p $(dir $(@))
	$(PPU_OBJCOPY)  -I binary -O elf64-powerpc-celloslv2 -B powerpc $< $@

$(FPSHADER_PPU_OBJS): $(OBJS_DIR)/%.ppu.o : %.fpo
	@mkdir -p $(dir $(@))
	$(PPU_OBJCOPY)  -I binary -O elf64-powerpc-celloslv2 -B powerpc $< $@


