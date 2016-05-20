/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 400.001
* Copyright (C) 2009 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/

#ifndef _SCE_NP_SAMPLE_NP_GUI_DRAW_H_
#define _SCE_NP_SAMPLE_NP_GUI_DRAW_H_

/*J
 * スクリーン周囲の一定領域は表示しない。
 */
#define MARGIN_TOP_BOTTOM		0.075f /* 7.5% */
#define MARGIN_LEFT_RIGHT		0.075f /* 7.5% */

/*J
 * DEFAULT_MAX_LETTER_ROW, DEFAULT_MAX_LETTER_COLUMN
 * is a max number in row/column on a screen
 * when scale is 1.0f and MARGIN_(TOP/LEFT) is 0.0f.
 */
#define DEFAULT_SCALE				(0.95f)
#define DEFAULT_MAX_LETTER_IN_ROW		(80 / DEFAULT_SCALE)
#define DEFAULT_MAX_LETTER_IN_COLUMN	(32 / DEFAULT_SCALE)

#define XSTART				MARGIN_LEFT_RIGHT
#define YSTART				MARGIN_TOP_BOTTOM
#define DEFAULT_XCHAR_INTERVAL      ((float)(1.0f/(float)DEFAULT_MAX_LETTER_IN_ROW))
#define DEFAULT_YCHAR_INTERVAL		((float)(1.0f/(float)DEFAULT_MAX_LETTER_IN_COLUMN))

#define MAX_LETTER_IN_ROW		((int)((1.0f - (MARGIN_LEFT_RIGHT * 2)) * DEFAULT_MAX_LETTER_IN_ROW))
#define MAX_LETTER_IN_COLUMN	((int)((1.0f - (MARGIN_TOP_BOTTOM * 2)) * DEFAULT_MAX_LETTER_IN_COLUMN))

#define LINE_BOTTOM_Y (1.0f - MARGIN_TOP_BOTTOM - DEFAULT_YCHAR_INTERVAL)

#define STRING_WIDTH(str)	((float)(strlen(str) * DEFAULT_XCHAR_INTERVAL))

#define CENTERING_X_POS(str) \
	(XSTART + ((1.0f - (MARGIN_LEFT_RIGHT * 2)) - STRING_WIDTH(str)) / 2)
#define ALIGN_RIGHT_X_POS(str) \
	(XSTART + ((1.0f - (MARGIN_LEFT_RIGHT * 2)) - (strlen(str) + 1) * DEFAULT_XCHAR_INTERVAL))

/*
 * for scale
 */
#define DEFAULT_SCALE				(0.95f)
#define S_DEFAULT_MAX_LETTER_IN_ROW(scale)		(80 / (scale))
#define S_DEFAULT_MAX_LETTER_IN_COLUMN(scale)	(32 / (scale))

#define XSTART				MARGIN_LEFT_RIGHT
#define YSTART				MARGIN_TOP_BOTTOM
#define S_DEFAULT_XCHAR_INTERVAL(scale)      ((float)(1.0f/(float)S_DEFAULT_MAX_LETTER_IN_ROW(scale)))
#define S_DEFAULT_YCHAR_INTERVAL(scale)		((float)(1.0f/(float)S_DEFAULT_MAX_LETTER_IN_COLUMN(scale)))

#define S_MAX_LETTER_IN_ROW(scale)		((int)((1.0f - (MARGIN_LEFT_RIGHT * 2)) * S_DEFAULT_MAX_LETTER_IN_ROW(scale)))
#define S_MAX_LETTER_IN_COLUMN(scale)	((int)((1.0f - (MARGIN_TOP_BOTTOM * 2)) * S_DEFAULT_MAX_LETTER_IN_COLUMN(scale)))

#define S_LINE_BOTTOM_Y(scale) (1.0f - MARGIN_TOP_BOTTOM - S_DEFAULT_YCHAR_INTERVAL(scale))

#define S_STRING_WIDTH(str, scale)	((float)(strlen(str) * S_DEFAULT_XCHAR_INTERVAL(scale)))

#define S_CENTERING_X_POS(str, scale) \
	(XSTART + ((1.0f - (MARGIN_LEFT_RIGHT * 2)) - S_STRING_WIDTH(str, scale)) / 2)
#define S_ALIGN_RIGHT_X_POS(str, scale) \
	(XSTART + ((1.0f - (MARGIN_LEFT_RIGHT * 2)) - (strlen(str) + 1) * S_DEFAULT_XCHAR_INTERVAL(scale)))

/* 0xAABBGGRR */
#define COLOR_WHITE			0x00ffffff
#define COLOR_YELLOW		0x0000ffff
#define COLOR_GREEN			0x0000ff00
#define COLOR_RED			0x000000ff
#define COLOR_LIGHT_BLUE	0x00ffff00

#define NON_FOCUSED_COLOR	0x0000ff00
#define FOCUSED_COLOR		0x000000ff

#define DEF_A				0xff000000
#define UNFOCUSED_A			0x50000000

#define MENU_COLOR(num, pos, color) \
	(num == pos ? color : (DEF_A|COLOR_WHITE))
#define MENU_COLOR_WITH_A(num, pos, color, a) \
	(num == pos ? color : (a|COLOR_WHITE))

#endif //_SCE_NP_SAMPLE_NP_GUI_H_
