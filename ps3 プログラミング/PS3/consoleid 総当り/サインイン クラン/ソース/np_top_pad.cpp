/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 400.001
* Copyright (C) 2009 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/

#include <sysutil/sysutil_common.h>
#include "np_gui.h"

extern struct np_gui_info g_info;

static void
top_pad_enter(
	void
	)
{	

		g_info.mode_after_netstart = NP_GUI_MODE_CLANS;		//追加	サインイン画面
		change_mode(NP_GUI_MODE_NETSTART);			//追加
	if (g_info.pos_y == NP_GUI_MODE_TOP_POS_Y_MODE_CLANS) {				//サインイン画面起動
		/*J ネットワーク開始ダイアログを起動してから NP を使うこと */
		g_info.mode_after_netstart = NP_GUI_MODE_CLANS;
		change_mode(NP_GUI_MODE_NETSTART);
		
	}

	return;
}

static void
top_pad_back(
	void
	)
{
	
	return;
}

void
top_pad_circle(
	void
	)
{
		top_pad_enter();				//追加
	if (g_info.button_assign == CELL_SYSUTIL_ENTER_BUTTON_ASSIGN_CIRCLE)	//ここでチェック?
		top_pad_enter();
	else if (g_info.button_assign == CELL_SYSUTIL_ENTER_BUTTON_ASSIGN_CROSS)
		top_pad_back();
	
	return;
}

void
top_pad_cross(
	void
	)
{
	if (g_info.button_assign == CELL_SYSUTIL_ENTER_BUTTON_ASSIGN_CIRCLE)
		top_pad_back();
	else if (g_info.button_assign == CELL_SYSUTIL_ENTER_BUTTON_ASSIGN_CROSS)
		top_pad_enter();

	return;
}

void
top_pad_down(
	void
	)
{
	if (g_info.pos_y < NP_GUI_MODE_TOP_POS_Y_MAX) {
		g_info.pos_y++;
	}	
	
	return;
}

void
top_pad_up(
	void
	)
{
	if (g_info.pos_y > NP_GUI_MODE_TOP_POS_Y_MIN) {
		g_info.pos_y--;
	}

	return;
}
