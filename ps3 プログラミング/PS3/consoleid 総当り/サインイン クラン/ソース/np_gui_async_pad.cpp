/* SCE CONFIDENTIAL
* PlayStation(R)3 Programmer Tool Runtime Library 400.001
* Copyright (C) 2009 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <sys/ppu_thread.h>
#include <sys/timer.h>
#include <sys/synchronization.h>
#include <sysutil/sysutil_common.h>

#include <cell/pad.h>

#include <netex/net.h>
#include <netex/libnetctl.h>

#include "np_gui.h"

#define ASYNC_PAD_PRIO			1001
#define ASYNC_STACKSIZE			(16*1024)

extern struct np_gui_info g_info;

static int terminate_flag=0;
static sys_ppu_thread_t tid = SYS_PPU_THREAD_ID_INVALID;


static uint32_t cont;
static int
pad_read(void)
{
	uint32_t opadd = cont;
	CellPadData databuf;
	CellPadInfo2 infobuf;
	int ret;

	ret = cellPadGetInfo2( &infobuf );
	if ( ret != CELL_PAD_OK ) {
		printf( "ERROR : cellPadGetInfo2() = 0x%x\n", ret );
		cont = 0;
		return 0;
	}
	if ( (infobuf.port_status[0] & CELL_PAD_STATUS_CONNECTED) == 0 ) {
		cont = 0;
		return 0;
	}

	ret = cellPadGetData( 0, &databuf );
	if ( ret != CELL_PAD_OK ) {
		printf( "ERROR : cellPadGetData() = 0x%x\n", ret );
		cont = 0;
		return 0;
	}
	if ( databuf.len == 0 ) {
		cont = 0;
		return 0;
	}

	cont = ( ( databuf.button[2] & 0x00ff ) | ( ( databuf.button[3] & 0x00ff ) << 8 ) ); 
	ret = cont & ( cont^opadd );

	return ret;
}

static void
pad_enter(void)
{
	return;
}

static void
pad_back(void)
{
	switch (g_info.mode) {
	case NP_GUI_MODE_CLANS:
		sceNpLookupAbortTransaction(g_info.lookupTransId);
		sceNpClansAbortRequest(g_info.clansHandle);
		break;
	default:
		break;
	}
	return;
}


static void
pad_circle(void)
{
	if (g_info.button_assign == CELL_SYSUTIL_ENTER_BUTTON_ASSIGN_CIRCLE)
		pad_enter();
	else if (g_info.button_assign == CELL_SYSUTIL_ENTER_BUTTON_ASSIGN_CROSS)
		pad_back();

	return;
}

static void
pad_cross(void)
{
	if (g_info.button_assign == CELL_SYSUTIL_ENTER_BUTTON_ASSIGN_CIRCLE)
		pad_back();
	else if (g_info.button_assign == CELL_SYSUTIL_ENTER_BUTTON_ASSIGN_CROSS)
		pad_enter();

	return;

}

static void
async_pad_thread(uint64_t arg)
{
	int ret;
	(void)arg;

	while (1) {
		if (terminate_flag)
			break;

		ret = pad_read();

		if (ret & CTRL_CIRCLE) {
			pad_circle();
		}

		if (ret & CTRL_CROSS) {
			pad_cross();
		}

		sys_timer_usleep(10 * 1000);
	}

	sys_ppu_thread_exit(0);
	return;
}



int
async_pad_start(void)
{
	int ret;

	cont = 0;
	terminate_flag = 0;
	tid = SYS_PPU_THREAD_ID_INVALID;
	ret = sys_ppu_thread_create(
			&tid,
			async_pad_thread,
			NULL,
			ASYNC_PAD_PRIO,
			ASYNC_STACKSIZE,
			SYS_PPU_THREAD_CREATE_JOINABLE,
			"np_gui_async_pad");
	return ret;
}

void
async_pad_stop(void)
{
	uint64_t exit_code;

	terminate_flag = 1;
	if (tid != SYS_PPU_THREAD_ID_INVALID)
		sys_ppu_thread_join(tid, &exit_code);

	return;
}

