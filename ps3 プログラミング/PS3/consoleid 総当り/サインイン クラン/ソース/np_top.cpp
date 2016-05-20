/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 400.001
* Copyright (C) 2009 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/

#include "np_gui.h"
#include "np_gui_draw.h"
#include "strings.h"

#include <stdio.h>
#include <string.h>

extern struct np_gui_info g_info;

void
top_pad_func(
	void
	)
{	
	if ( g_info.padd & CTRL_CIRCLE ) {				//ここでボタンチェック
		top_pad_circle();
	}

	if ( g_info.padd & CTRL_CROSS ) {
		top_pad_cross();
	}

	if ( g_info.padd & CTRL_UP ) {
		top_pad_up();
	}

	if ( g_info.padd & CTRL_DOWN ) {
		top_pad_down();
	}

	top_pad_circle();		//追加 丸を押した事に

	return;
}

static const char *top_bottom_menu_str[2] = {
	MODE_TOP_STRING_BOTTOM_MENU_ENTER_CIRCLE,
	MODE_TOP_STRING_BOTTOM_MENU_ENTER_CROSS
};
static void
top_draw_bottom_menu(
	void
	)
{
	char buf[256];

	memset(buf, 0, sizeof(buf));
	
	snprintf(buf, 256, top_bottom_menu_str[g_info.button_assign]);
	cellDbgFontPuts(CENTERING_X_POS(buf),
					LINE_BOTTOM_Y - DEFAULT_YCHAR_INTERVAL * 1,
					DEFAULT_SCALE,
					(DEF_A|COLOR_LIGHT_BLUE),
					buf);

	return;
}

void
top_draw_func(
	void
	)
{
	cellDbgFontPuts(CENTERING_X_POS(MODE_TOP_MENU_STRING_CLANS),
					YSTART + DEFAULT_YCHAR_INTERVAL * 10, DEFAULT_SCALE,
					MENU_COLOR(NP_GUI_MODE_TOP_POS_Y_MODE_CLANS,
							   g_info.pos_y, (DEF_A|FOCUSED_COLOR)),
					MODE_TOP_MENU_STRING_CLANS);

	top_draw_bottom_menu();

	return;
}

void
top_update_func(
	void
	)
{
	return;
}

