#   SCE CONFIDENTIAL
#   PlayStation(R)3 Programmer Tool Runtime Library 400.001
#   Copyright (C) 2007 Sony Computer Entertainment Inc.
#   All Rights Reserved.

CELL_MK_DIR ?= $(CELL_SDK)/samples/mk
include $(CELL_MK_DIR)/sdk.makedef.mk

SUBDIRS				= $(CELL_FW_DIR)

PPU_INCDIRS			:= -I$(CELL_FW_DIR)/include -I$(CELL_FW_DIR)/include/psgl

CELL_PSGL_VERSION	= opt
PPU_CPPFLAGS		= -DPSGL
GCC_PPU_CXXFLAGS		+= -fno-exceptions

PPU_SRCS			= main.cpp
PPU_TARGET			= launcher.elf


PPU_LDLIBDIR		= -L$(PPU_PSGL_LIBDIR) -L$(CELL_FW_DIR)
PPU_LDLIBS			= \
	-ldbgfont \
	-lfw \
	-lPSGL \
	-lPSGLU \
	-lPSGLFX \
	-lio_stub \
	-lgcm_cmd \
	-lgcm_sys_stub \
	-lfs_stub \
	-lsysmodule_stub \
	-lsysutil_stub \
	-lnet_stub \
	-lnetctl_stub \
	-lresc_stub

CLEANFILES = PS3_GAME/USRDIR/EBOOT.BIN $(OBJS_DIR)/$(PPU_TARGET)
.PHONY: gen_image npdrm_package

include $(CELL_MK_DIR)/sdk.target.mk

clean_usrdir::
	rm -rf PS3_GAME/USRDIR/
	mkdir -p PS3_GAME/USRDIR/

gen_image:: $(PPU_TARGET) clean_usrdir
	$(PPU_STRIP) -s $< -o $(OBJS_DIR)/$(PPU_TARGET)
	$(MAKE_FSELF) $(OBJS_DIR)/$(PPU_TARGET) PS3_GAME/USRDIR/EBOOT.BIN

npdrm_package:: $(PPU_TARGET) clean_usrdir
	$(PPU_STRIP) -s $< -o $(OBJS_DIR)/$(PPU_TARGET)
	$(MAKE_FSELF_NPDRM) $(OBJS_DIR)/$(PPU_TARGET) PS3_GAME/USRDIR/EBOOT.BIN
	$(MAKE_PACKAGE_NPDRM) launcher.conf PS3_GAME/


