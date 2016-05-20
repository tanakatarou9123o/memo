/*J   SCE CONFIDENTIAL                                       */
/*J   PlayStation(R)3 Programmer Tool Runtime Library 400.001 */
/*J   Copyright (C) 2007 Sony Computer Entertainment Inc.    */
/*J   All Rights Reserved.                                   */
/*J   File: graphics.h
 *    Description:
 *    simple sample to show how to use osk dialog system utility
 *
 */

#include <cell/gcm.h>
#include <cell/dbgfont.h>

#define HOST_SIZE	(1*1024*1024)
#define CB_SIZE		(0x10000)

#ifdef	_QA_3D_
enum{
	EN_3D_FRAMEPACKING,
	EN_3D_SIDE_BY_SIDE,
	EN_SIMULVIEW_FRAMEPACKING,
	EN_SIMULVIEW_FRAMEPACKING_1024x720,
	EN_SIMULVIEW_FRAMEPACKING_960x720,
	EN_SIMULVIEW_FRAMEPACKING_800x720,
	EN_SIMULVIEW_FRAMEPACKING_640x720,
};
#endif

void initShader( void );
int32_t initDisplay( void );
void setDrawEnv( void );
void setRenderTarget( int lrCount );
void setRenderState( void );
int32_t setRenderObject( void );
void flip( void );

int initDbgFont( void );
int termDbgFont( void );
int drawDbgFont( void );
int DbgPrintf( const char *string, ... );
void titleDbgPrintf( int lrCount );
