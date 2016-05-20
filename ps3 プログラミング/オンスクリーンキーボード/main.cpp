/*   SCE CONFIDENTIAL                                       */
/*   PlayStation(R)3 Programmer Tool Runtime Library 400.001 */
/*   Copyright (C) 2007 Sony Computer Entertainment Inc.    */
/*   All Rights Reserved.                                   */
/*   File: main.cpp
 *   Description:
 *     simple sample to show how to use osk dialog utility
 *
 */

#include <wchar.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include <string.h>
#include <sys/process.h>
#include <sys/memory.h>
#include <sys/timer.h>
#include <sys/return_code.h>
#include <sys/prx.h>
#include <sys/sys_time.h>
#include <cell/gcm.h>
#include <cell/pad.h>
#include <cell/keyboard.h>
#include <cell/sysmodule.h>
#include <math.h>
#include <stddef.h>
#include <sysutil_sysparam.h>
#include <sysutil_oskdialog.h>
#include <sysutil_oskdialog_ext.h>
#include <cell/dbgfont.h>

#ifdef _QA_LRSTART_KEYEXIT_				//L1+R1+START同時押し終了
#include "../../include_private/sysutil_game_I.h"
#endif

#include "graphics.h"

// --- debug --------------------------------------------------
#define OSK_SAMPLE_DEBUG
#ifdef OSK_SAMPLE_DEBUG
	#define INFORM(x...)			{DbgPrintf("[IMFORM] "); DbgPrintf(x); printf(x);}
	#define ERR_PRINTF(x...)		{DbgPrintf("[ERROR] "); DbgPrintf(x); printf(x);}
	#define DEBUG_PRINTF(x...)		printf(x)
#else
	#define INFORM(x...)
	#define ERR_PRINTF(x...)
	#define DEBUG_PRINTF(x...)
#endif


// --- variables ----------------------------------------------
/* double buffering */
using namespace cell::Gcm;

// button[2]
#define	CTRL_SELECT		(1<<0)
#define	CTRL_L3			(1<<1)
#define	CTRL_R3			(1<<2)
#define	CTRL_START		(1<<3)
#define	CTRL_UP			(1<<4)
#define	CTRL_RIGHT		(1<<5)
#define	CTRL_DOWN		(1<<6)
#define	CTRL_LEFT		(1<<7)
// button[3]
#define	CTRL_L2			(1<<8)
#define	CTRL_R2			(1<<9)
#define	CTRL_L1			(1<<10)
#define	CTRL_R1			(1<<11)
#define	CTRL_TRIANGLE	(1<<12)
#define	CTRL_CIRCLE		(1<<13)
#define	CTRL_CROSS		(1<<14)
#define	CTRL_SQUARE		(1<<15)

/*J オンスクリーンキーボードの状態 */
enum{
	MODE_IDLE = 0,
	MODE_OPEN,
	MODE_RUNNING,
	MODE_CLOSE,
	MODE_ENTERED,
	MODE_CANCELED,
	SET_ABORT_TIMER,
	CHANGE_PANEL_MODE,
	CHANGE_SCALE,
	SET_CALLBACK,
	MODE_EXIT,
	START_DIALOG_TYPE,
	START_SEPARATE_TYPE_1,
	START_SEPARATE_TYPE_2,
};
static int oskdialog_mode = MODE_IDLE;

enum{
	MCMD_START_DIALOG_TYPE,
	MCMD_START_SEPARATE_TYPE_1,
	MCMD_START_SEPARATE_TYPE_2,
	MCMD_SET_ABORT_TIMER,
	MCMD_CHANGE_PANEL_MODE,
	MCMD_CHANGE_SCALE,
	MCMD_SET_CALLBACK,
	MCMD_NUM
};

/*J コマンドメニュー */
struct MenuCommand_st
{
	int command;
	int mode;
	char string[64];
};

static struct MenuCommand_st menu_command[] = {
	{ MCMD_START_DIALOG_TYPE,		START_DIALOG_TYPE,		"Start Dialog Type OSK" },
	{ MCMD_START_SEPARATE_TYPE_1,	START_SEPARATE_TYPE_1,	"Start Separate Type OSK[REMAIN_OPEN]" },
	{ MCMD_START_SEPARATE_TYPE_2,	START_SEPARATE_TYPE_2,	"Start Separate Type OSK[HIDE]" },
	{ MCMD_SET_ABORT_TIMER,			SET_ABORT_TIMER,		"Set Abort Timer ON/OFF (Current Setting:OFF)" },
	{ MCMD_CHANGE_PANEL_MODE,		CHANGE_PANEL_MODE,		"Change Panel Mode (current mode:DEFAULT)" },
	{ MCMD_CHANGE_SCALE,			CHANGE_SCALE,			"Change Scale (current scale:1.0)" },
	{ MCMD_SET_CALLBACK,			SET_CALLBACK,			"Set Callback (current setting:OFF)" },
};
static int command_selected = 0;


#define DBGFONT_SCALE_DEFAULT	(0.8f)
#define DBGFONT_SCALE_FOCUSED	(1.2f)
#define DBGFONT_COLOR_DEFAULT	(0xffffffff)
#define DBGFONT_COLOR_FOCUSED	(0xffffff00)


// --- init ----------------------------------------------------
#define MESSAGE				L"OSK Dialog"
#define INIT_TEXT			L"123ABC"
#define ABORT_TIME			5					/*J Abort呼び出し時間(経過秒) */
#define ENTERED_TIMES		3					/*J 確定続行時の確定回数 */
#define DEVICE_UNLOCK_TIME	5 * 1000 * 1000		/*J デバイスロックの解除タイマー */

// --- global     ----------------------------------------------
/* local memory allocation */

// ------- OSKユーティリティ----------------
static bool is_cross_enter = false;

static sys_memory_container_t container_id = SYS_MEMORY_CONTAINER_ID_INVALID;	/*J メモリコンテナID */
static CellOskDialogInputFieldInfo inputFieldInfo;								/*J 入力フィールドの情報 */
static CellOskDialogPoint pos;													/*J OSKの初期表示位置 (x, y) */
static CellOskDialogLayoutInfo inputFieldLayoutInfo, inputPanelLayoutInfo;		/*J 分離型ウィンドウの表示位置情報 */
static CellOskDialogParam dialogParam;											/*J OSK ユーティリティ起動パラメータ */ 
static CellOskDialogCallbackReturnParam OutputInfo;								/*J 返却文字列格納バッファ */
static int counter = 0;															/*J 強制終了タイマーカウンタ */
static int enteredCounter = 0;													/*J 確定続行時の確定回数カウンタ */
static bool abortflg = false;													/*J 強制終了設定フラグ */
static bool callbacksetflg = false;												/*J コールバック登録設定フラグ(フィルタ、キーフックコールバック共通) */
static CellOskDialogSeparateWindowOption WindowOption;							/*J 分離型ウィンドウ */

static float windowScale = 1.00f;												/*J OSKの表示スケール */
static system_time_t lockedTime = 0;											/*J デバイスロック開始時間 */
static bool deviceLock = false;													/*J デバイスロックの状態 */

static int forceFinished = 0;													/*J PSボタンによるシステムメニュー表示のための終了 */

static uint16_t s_resultTextBuffer[CELL_OSKDIALOG_STRING_SIZE + 1];
// -----------------------------------------
static void *host_addr;
static CellKbInfo info;		/*J キーボード情報 */


/* prototypes */
static void drawListMenu( MenuCommand_st *menuCommand, int menu_size, int focused );

static bool isCrossButtonEnter( void );
static int pad_read( void );
static bool force_finish_callback( void );
static void sysutil_callback( uint64_t status, uint64_t param, void *userdata );
static int osk_filtering_callback( uint16_t *pConfirmString, int32_t wordLength );
static bool osk_keyhook_callback( CellOskDialogKeyMessage *keyMessage, uint32_t *action, void *pActionInfo );

static int initializeModule( void );
static int finalizeModule( void );
static int setOskFieldParams( void );

static void register_osk_callback( void );
static int osk_main( void );


static void drawListMenu( MenuCommand_st *menuCommand, int menu_size, int focused )
{
	float y = 0.1f;
	int i = 0;

	while( i < menu_size ){
		cellDbgFontPrintf( 0.08f, y,
			( i == focused ) ? DBGFONT_SCALE_FOCUSED : DBGFONT_SCALE_DEFAULT,
			( i == focused ) ? DBGFONT_COLOR_FOCUSED : DBGFONT_COLOR_DEFAULT,
            menuCommand[i].string
		);
		y += 0.05f;
		i++;
	}
}


/* XボタンがEnterかどうかをチェックします */
static bool isCrossButtonEnter( void )
{
	int ret = 0;
	int enter_button_assign;
	ret = cellSysutilGetSystemParamInt( CELL_SYSUTIL_SYSTEMPARAM_ID_ENTER_BUTTON_ASSIGN, &enter_button_assign );
	if( ret != CELL_OK ){
		ERR_PRINTF( "ERROR : cellSysutilGetSystemParamInt() = 0x%x\n", ret );
		return false;
	}
	else{
		if( enter_button_assign == CELL_SYSUTIL_ENTER_BUTTON_ASSIGN_CROSS ){
			return true;
		}
		else{
			return false;
		}
	}
}

/*J パッドデータの読み出し */
static int pad_read( void )
{
	int ret;
	static uint32_t	opadd;
	uint32_t		padd;

	CellPadData databuf;
	CellPadInfo2 infobuf;
	static uint32_t old_info = 0;

	ret = cellPadGetInfo2( &infobuf );
	if( ret != CELL_PAD_OK ){
		DEBUG_PRINTF( "cellPadGetInfo2() = 0x%x\n", ret );
		opadd = 0;
		return 1;
	}

	if( infobuf.port_status[0] == CELL_PAD_STATUS_DISCONNECTED ){
		DEBUG_PRINTF("%s:%s(%d)\tif ( infobuf.port_status[0] == CELL_PAD_STATUS_DISCONNECTED )\treturn 1\n", __FILE__, __FUNCTION__, __LINE__ );
		opadd = 0;
		return 1;
	}

	if( (infobuf.system_info & CELL_PAD_INFO_INTERCEPTED) && (!(old_info & CELL_PAD_INFO_INTERCEPTED)) ){
		DEBUG_PRINTF("This program lost the ownership of the game pad data\n");
		old_info = infobuf.system_info;
	}
	else if( (!(infobuf.system_info & CELL_PAD_INFO_INTERCEPTED)) && (old_info & CELL_PAD_INFO_INTERCEPTED) ){
		DEBUG_PRINTF("This program got the ownership of the game pad data\n");
		old_info = infobuf.system_info;
		DEBUG_PRINTF("%s:%s(%d)\treturn 1\n", __FILE__, __FUNCTION__, __LINE__ );
		opadd = 0;
		return 1;
	}

	ret = cellPadGetData( 0, &databuf );
	if( ret != CELL_PAD_OK ){
		DEBUG_PRINTF("%s:%s(%d)\tif ( ret != CELL_PAD_OK )\treturn 1\n", __FILE__, __FUNCTION__, __LINE__ );
		opadd = 0;
		return 1;
	}
	if( databuf.len == 0 ){
		opadd = 0;
		return 1;
	}

	padd = ( databuf.button[2] | ( databuf.button[3] << 8 ) ); 
#ifdef _QA_LRSTART_KEYEXIT_
	if ( padd & CTRL_L1 &&
		 padd & CTRL_R1 &&
		 padd & CTRL_START )
	{
		if ( opadd != padd ){
			cellSysutilGameExit_I();
		}
	} 
#endif
	if( padd & CTRL_UP ){
		if( opadd != padd ){
			if( command_selected == 0 ){
				command_selected = MCMD_NUM-1;
			}
			else{
				command_selected--;
			}
		}
		if( oskdialog_mode != MODE_IDLE ){
			DEBUG_PRINTF("CTRL_UP was pressed during utility running\n");
		}
	}
	else if( padd & CTRL_DOWN ){
		if( opadd != padd ){
			if( command_selected == MCMD_NUM - 1 ){
				command_selected = 0;
			}
			else{
				command_selected++;
			}
		}
		if( oskdialog_mode != MODE_IDLE ){
			DEBUG_PRINTF("CTRL_DOWN was pressed during utility running\n");
		}
	}
	else if( (padd & CTRL_CROSS) || (padd & CTRL_CIRCLE) ){
		if( opadd != padd ){
			if( oskdialog_mode == MODE_IDLE ){
				if( (is_cross_enter && padd & CTRL_CROSS) || (!is_cross_enter && padd & CTRL_CIRCLE) ){
					oskdialog_mode = menu_command[command_selected].mode;
				}
			}
			else{
				DEBUG_PRINTF("press CTRL_CROSS during utility running\n");
			}
		}
	}
	opadd = padd;

	return 1;
}

/*J 強制終了時に呼び出されるコールバック関数 */
static bool force_finish_callback( void )
{
	INFORM("force_finish_callback\n");
	forceFinished = 1;
	return true;
}

/*J ゲーム終了通知用コールバック関数 */
static void sysutil_callback( uint64_t status, uint64_t param, void *userdata )
{
	(void)param;
	(void)userdata;
	int ret = 0;

	switch( status ){
	case CELL_SYSUTIL_OSKDIALOG_LOADED:
		break;
	case CELL_SYSUTIL_OSKDIALOG_FINISHED:
		if( deviceLock == true ){
			ret = cellOskDialogExtInputDeviceLock();
			if( !ret ){
				deviceLock = false;
				lockedTime = 0;
			}
		}
		oskdialog_mode = MODE_CLOSE;
		break;
	case CELL_SYSUTIL_OSKDIALOG_UNLOADED:
		break;
	case CELL_SYSUTIL_REQUEST_EXITGAME:
		oskdialog_mode = MODE_EXIT;
		break;
	case CELL_SYSUTIL_DRAWING_BEGIN:
	case CELL_SYSUTIL_DRAWING_END:
		break;
	case CELL_SYSUTIL_OSKDIALOG_INPUT_ENTERED:
		oskdialog_mode = MODE_ENTERED;
		break;
	case CELL_SYSUTIL_OSKDIALOG_INPUT_CANCELED:
		oskdialog_mode = MODE_CANCELED;
		break;
	case CELL_SYSUTIL_OSKDIALOG_INPUT_DEVICE_CHANGED:
		if( param == CELL_OSKDIALOG_INPUT_DEVICE_KEYBOARD ){
			INFORM("Input device changed to keyboard.\n");
		}
		else if( param == CELL_OSKDIALOG_INPUT_DEVICE_PAD ){
			INFORM("Input device changed to pad.\n");
		}
		break;
	case CELL_SYSUTIL_OSKDIALOG_DISPLAY_CHANGED:
		if( param == CELL_OSKDIALOG_DISPLAY_STATUS_SHOW ){
			INFORM("Start show of OSK.\n");
		}
		else if( param == CELL_OSKDIALOG_DISPLAY_STATUS_HIDE ){
			INFORM("Completed hide of OSK.\n");
		}
		break;
	case CELL_SYSUTIL_SYSTEM_MENU_CLOSE:
		INFORM("CELL_SYSUTIL_SYSTEM_MENU_CLOSE\n");
		/*J システムメニューが閉じたので再び起動 */
		if( forceFinished ){
			forceFinished = 0;
			oskdialog_mode = START_SEPARATE_TYPE_2;
		}
		break;
	case CELL_SYSUTIL_SYSTEM_MENU_OPEN:
		INFORM("CELL_SYSUTIL_SYSTEM_MENU_OPEN\n");
		break;
	default:
		DEBUG_PRINTF("unknown status : 0x%x\n", (int)status);
		/* 
		 *  今後通知される情報が増える場合があります。
		 *  switch/case文のdefault節でアプリケーションが正常に動作
		 *  しなくなるような処理を行うことがないように注意してください。
		 */
		break;
	}
}

/*J 入力文字フィルターコールバック関数 */
static int osk_filtering_callback( uint16_t *pConfirmString, int32_t wordLength )
{
	/*J
	 *  [abc]が来たら[def]に変更、[def]は[abc]に変更
	 *  [ghi]が来たらその文字を削除
	 *  全角、半角、大文字、小文字をチェック
	 */

	//J  削除文字
	uint16_t deleteCharTable[] = {0x0067, 0x0068, 0x0069, 0xFF47, 0xFF48, 0xFF49,
							0x0000};

	//J  文字チェックテーブル
	uint16_t checkCharTable[] = {0x0061, 0x0062, 0x0063, 0xFF41, 0xFF42, 0xFF43,
								0x0064, 0x0065, 0x0066, 0xFF44, 0xFF45, 0xFF46,
							0x0000};
	//J  変換文字テーブル
	uint16_t changeCharTable[] = {0x0064, 0x0065, 0x0066, 0xFF44, 0xFF45, 0xFF46,
								0x0061, 0x0062, 0x0063, 0xFF41, 0xFF42, 0xFF43,
							0x0000};

	int ret = CELL_OSKDIALOG_NOT_CHANGE;

	//J  禁止文字削除処理
	for( int i = 0; i <= wordLength && pConfirmString[i] != 0; i++ ){
		for( int j = 0; deleteCharTable[j] != 0; j++ ){
			if( deleteCharTable[j] == pConfirmString[i] ){
				wchar_t tempstring[2];
				tempstring[0] = deleteCharTable[j];
				tempstring[1] = 0;
				INFORM("The character of \"%S\" was deleted.\n",tempstring);
				int k = 0;
				//J 文字詰めを行う
				do{
					pConfirmString[i+k] = pConfirmString[i+k+1];
					pConfirmString[i+k+1] = NULL;
					k++;
				}while( pConfirmString[i+k+1] != NULL );
				i--;
				ret = CELL_OSKDIALOG_CHANGE_WORD;
				break;
			}
		}
	}

	//J  文字変換処理
	for( int i = 0;  i <= wordLength && pConfirmString[i] != 0; i++ ){
		for(int j=0; checkCharTable[j] != 0;j++){
			if( checkCharTable[j] == pConfirmString[i] ){
				wchar_t tempstring1[2];
				wchar_t tempstring2[2];
				tempstring1[0] = pConfirmString[i];
				tempstring1[1] = 0;
				tempstring2[0] = changeCharTable[j];
				tempstring2[1] = 0;
				INFORM("\"%S\" was changed to \"%S\".\n", tempstring1, tempstring2);
				pConfirmString[i] = changeCharTable[j];
				ret = CELL_OSKDIALOG_CHANGE_WORD;
				break;
			}
		}
	}

	return ret;
}

/*J キーボードフックコールバック関数 */
static bool osk_keyhook_callback( CellOskDialogKeyMessage *keyMessage, uint32_t *action, void *pActionInfo )
{
	int ret = 0;
	uint16_t *temp = (uint16_t *)pActionInfo;
	uint16_t registWordTable[][4] = {
		{0x0046, 0x0031, 0x0000},
		{0x0046, 0x0032, 0x0000},
		{0x0046, 0x0033, 0x0000},
		{0x0046, 0x0034, 0x0000},
		{0x0000, },
	};
	uint16_t registWordListTable[][6] = {
		{0x004C, 0x0049, 0x0053, 0x0054, 0x0031, 0x0000},
		{0x004C, 0x0049, 0x0053, 0x0054, 0x0032, 0x0000},
		{0x004C, 0x0049, 0x0053, 0x0054, 0x0033, 0x0000},
		{0x004C, 0x0049, 0x0053, 0x0054, 0x0034, 0x0000},
		{0x0000, },
	};

	static int loop=-1;

	switch( keyMessage->keycode ){
	case CELL_KEYC_F1:
		*action = CELL_OSKDIALOG_CHANGE_WORDS_INSERT;
		wcscpy( (wchar_t *)pActionInfo, (wchar_t *)registWordTable[0] );
		INFORM("\"%S\" was inserted.\n", (wchar_t *)registWordTable[0]);
		return true;
	case CELL_KEYC_F2:
		*action = CELL_OSKDIALOG_CHANGE_WORDS_INSERT;
		wcscpy( (wchar_t *)pActionInfo, (wchar_t *)registWordTable[1] );
		INFORM("\"%S\" was inserted.\n", (wchar_t *)registWordTable[1]);
		return true;
	case CELL_KEYC_F3:
		if( temp[0] != 0 ){
			/*J 未確定文字列がある場合はイベントをキャンセルする */
			*action = CELL_OSKDIALOG_CHANGE_EVENT_CANCEL;
			INFORM("UnspecifiedString	:	%S\n", (wchar_t*)pActionInfo);
			INFORM("Keyhook event was canceled.\n");
		}
		else{
			*action = CELL_OSKDIALOG_CHANGE_WORDS_REPLACE_ALL;
			wcscpy( (wchar_t *)pActionInfo, (wchar_t *)registWordTable[2] );
			INFORM("\"%S\" was replaced.\n", (wchar_t *)registWordTable[2]);
		}
		return true;
	case CELL_KEYC_F4:
		*action = CELL_OSKDIALOG_CHANGE_WORDS_REPLACE_ALL;
		wcscpy( (wchar_t *)pActionInfo, (wchar_t *)registWordTable[3] );
		INFORM("\"%S\" was replaced.\n", (wchar_t *)registWordTable[3]);
		return true;
	case CELL_KEYC_F5:
		/*J 入力文字列を取得可能にする */
		ret = cellOskDialogExtUpdateInputText();
		if( !ret ){
			ret = cellOskDialogGetInputText( &OutputInfo );
			if( !ret && (OutputInfo.result == CELL_OSKDIALOG_INPUT_FIELD_RESULT_OK) ){
				INFORM("cellOskDialogGetInputText	:	%S\n", (wchar_t*)OutputInfo.pResultString);
			}
			else{
				ERR_PRINTF("cellOskDialogGetInputText() error : 0x%x\n", ret);
			}
		}
		return false;
	case CELL_KEYC_PAGE_UP:
		if( loop == -1 ){
			loop = 0;
		}
		else{
			loop--;
		}
		if( loop < 0 ){
			loop = 3;
		}
		wcscpy( (wchar_t *)pActionInfo, (wchar_t *)registWordListTable[loop] );
		*action = CELL_OSKDIALOG_CHANGE_WORDS_INPUT;
		INFORM("\"%S\" was input.\n", (wchar_t *)registWordListTable[loop]);
		return true;
	case CELL_KEYC_PAGE_DOWN:
		if( loop == -1 ){
			loop = 0;
		}
		else{
			loop++;
		}
		if( loop > 3 ){
			loop = 0;
		}
		wcscpy( (wchar_t *)pActionInfo, (wchar_t *)registWordListTable[loop] );
		*action = CELL_OSKDIALOG_CHANGE_WORDS_INPUT;
		INFORM("\"%S\" was input.\n", (wchar_t *)registWordListTable[loop]);
		return true;
	case CELL_KEYC_Z:
		if( (keyMessage->mkey & (CELL_KB_MKEY_L_CTRL | CELL_KB_MKEY_R_CTRL)) ){
			/*J Ctrl+Zですべての文字列を削除する */
			wcscpy( (wchar_t *)pActionInfo, (wchar_t *)"" );
			*action = CELL_OSKDIALOG_CHANGE_WORDS_REPLACE_ALL;
			INFORM("All string was deleted!!\n");
			return true;
		}
		break;
	case CELL_KEYC_F11:
		if( deviceLock == false ){
			ret = cellOskDialogExtInputDeviceLock();
			if( !ret ){
				deviceLock = true;
				ret = cellOskDialogExtSendFinishMessage( CELL_OSKDIALOG_CLOSE_CONFIRM );
				lockedTime = sys_time_get_system_time();
				INFORM("The keyboard event was locked!!\n");
				INFORM("It will be removed five seconds later.\n");
			}
		}
		return false;
	}

	loop=-1;

	*action = CELL_OSKDIALOG_CHANGE_NO_EVENT;
	return false;
}

/*J ダイアログ型、分離型共通 <各モジュールロード、メモリコンテナ作成、コールバック登録> */
static int initializeModule( void )
{
	int ret;

	ret = cellSysmoduleLoadModule( CELL_SYSMODULE_IO );
	if( ret != CELL_OK ){
		ERR_PRINTF( "cellSysmoduleLoadModule( CELL_SYSMODULE_IO ) failed: 0x%x\n", ret );

		return ret;
	}
	ret = cellSysmoduleLoadModule( CELL_SYSMODULE_GCM_SYS );
	if( ret != CELL_OK ){
		ERR_PRINTF("cellSysmoduleLoadModule( CELL_SYSMODULE_GCM_SYS ) failed: 0x%x\n", ret);

		return ret;
	}

	/*J OskExtロード */	
	ret = cellSysmoduleLoadModule( CELL_SYSMODULE_SYSUTIL_OSK_EXT );
	if( ret != CELL_OK ){
		printf("cellSysmoduleLoadModule( CELL_SYSMODULE_SYSUTIL_OSK_EXT ) failed 0x%08X\n", ret);
		return ret;
	}

	host_addr = memalign( 1024*1024, HOST_SIZE );
	if( cellGcmInit(CB_SIZE, HOST_SIZE, host_addr) != CELL_OK ){
		return -1;
	}
	
	if( initDisplay() != 0 ){
		printf("initDisplay failed.\n");
		return -1;
	}

	initShader();

	setDrawEnv();

	if( setRenderObject() ){
		return -1;
	}

	setRenderState();

	/*J ゲーム終了通知用コールバック関数を登録する */
	ret = cellSysutilRegisterCallback( 0, sysutil_callback, NULL );
	assert(ret==0);

	ret = cellPadInit( 1 );
	if( ret != 0 ){
		ERR_PRINTF( "cellPadInit() = 0x%x\n", ret );
		return ret;
	}

	ret = cellKbInit( 1 );
	if( ret != CELL_OK ){
		printf ("cellPadInit failed 0x%08x\n", ret);
		return ret;
	}

	// 1st time
#ifndef	_QA_3D_
	setRenderTarget(0);
#endif
	/*J デバックフォントを初期化する */
	initDbgFont();

	return ret;
}

/*J ダイアログ型、分離型共通 <各モジュールアンロード、メモリコンテナ破棄、コールバック解除> */
static int finalizeModule( void )
{
	int ret;

	ret = cellPadEnd();
	if( ret != 0 ){
		ERR_PRINTF( "cellPadEnd() = 0x%x\n", ret );
		return ret;
	}

	/*J デバックフォントを終了する */
	termDbgFont();

	free( host_addr );

	/*J 登録したコールバックを解除する */
	ret = cellSysutilUnregisterCallback( 0 );
	assert(ret == 0);

	/*J OskExtを終了 */
	ret = cellSysmoduleUnloadModule( CELL_SYSMODULE_SYSUTIL_OSK_EXT );
	if( ret != CELL_OK ){
		ERR_PRINTF( "cellSysmoduleLoadModule( CELL_SYSMODULE_SYSUTIL_OSK_EXT ) failed: 0x%x\n", ret );
		return ret;
	}
	
	/*J ユーティリティを終了 */
	ret = cellSysmoduleUnloadModule( CELL_SYSMODULE_IO );
	if( ret != CELL_OK ){
		ERR_PRINTF( "cellSysmoduleLoadModule( CELL_SYSMODULE_IO ) failed: 0x%x\n", ret );
		return ret;
	}
	ret = cellSysmoduleUnloadModule( CELL_SYSMODULE_GCM_SYS );
	if( ret != CELL_OK ){
		ERR_PRINTF("cellSysmoduleUnloadModule(CELL_SYSMODULE_GCM_SYS) failed: (0x%x)\n", ret);
		return ret;
	}
	return ret;
}

/*J ダイアログ型、分離型共通 OSKフィールドパラメータ設定 */
static int setOskFieldParams( void )
{
	inputFieldInfo.message   = (uint16_t*)MESSAGE;				/*J OSKアプリの左上にガイドとして表示される文字列 */
	inputFieldInfo.init_text = (uint16_t*)INIT_TEXT;			/*J 初期入力文字列 */
	inputFieldInfo.limit_length = CELL_OSKDIALOG_STRING_SIZE;	/*J 入力文字の制限値 */

	pos.x = 0.0;
	pos.y = 0.0;

	/*J 使用するパネルをフラグで選択(英数入力、ひらがな入力など) */
	dialogParam.allowOskPanelFlg = CELL_OSKDIALOG_PANELMODE_DEFAULT;
	dialogParam.firstViewPanel = CELL_OSKDIALOG_PANELMODE_DEFAULT;	/*J 最初に表示するパネル */
	dialogParam.controlPoint = pos;		/*J OSKの初期表示位置 */
	dialogParam.prohibitFlgs = 0;		/*J 禁止条項フラグ (ex  CELL_OSKDIALOG_NO_SPACE*/

	OutputInfo.result = CELL_OSKDIALOG_INPUT_FIELD_RESULT_OK;		/*J OSKの終了要因 */ 
	OutputInfo.numCharsResultString = 32;			/*J 返却文字数指定 */ 

	memset( s_resultTextBuffer, 0, sizeof(s_resultTextBuffer) );
	OutputInfo.pResultString = s_resultTextBuffer;	/*J 返却文字列格納バッファ */ 

	return 0;
}

/*J OSKのコールバック関数登録 */
static void register_osk_callback( void )
{
	/*J 入力文字フィルターコールバック関数を登録する */
	cellOskDialogExtRegisterConfirmWordFilterCallback( osk_filtering_callback );

	/*J H/Wキーボードフックコールバック関数を登録する */
	cellOskDialogExtRegisterKeyboardEventHookCallback( CELL_OSKDIALOG_EVENT_HOOK_TYPE_FUNCTION_KEY | CELL_OSKDIALOG_EVENT_HOOK_TYPE_ASCII_KEY, osk_keyhook_callback );
}

static int osk_main( void )
{
	int ret = 0;

	/*J コールバックをチェックする */
	ret = cellSysutilCheckCallback();
	assert(ret==0);

	if( oskdialog_mode == START_DIALOG_TYPE ){
		if( callbacksetflg ){
			register_osk_callback();
		}

		/*J OSKダイアログのサイズ */ 
		CellOskDialogType OskDialogType = CELL_OSKDIALOG_TYPE_FULL_KEYBOARD_SINGLELINE_OSK;
		uint16_t width = 0;
		uint16_t height = 0;

		/*J OSKダイアログのサイズ取得(単一行入力ダイアログ) */
		ret = cellOskDialogGetSize( &width, &height, OskDialogType);

		if( ret ){
			ERR_PRINTF( "cellOskDialogGetSize() = 0x%x \n", ret );
		}
		else{
			INFORM("##########   OskDialog Size  ###########\n");
			INFORM( "OskDialogType:	%d\n", OskDialogType );
			INFORM( "Width :		%d\n", width );
			INFORM( "Height:		%d\n", height);
			INFORM("########################################\n\n");
		}

		/*J OSKの配置に関するオプション設定 */
		int32_t LayoutMode = CELL_OSKDIALOG_LAYOUTMODE_X_ALIGN_CENTER | CELL_OSKDIALOG_LAYOUTMODE_Y_ALIGN_CENTER;
		ret = cellOskDialogSetLayoutMode( LayoutMode );	
		if( ret ){
			ERR_PRINTF( "cellOskDialogSetLayoutMode() = 0x%x \n", ret );
		}
		else{
			INFORM("###########   SetLayoutMode  ###########\n");
			INFORM( "LayoutMode :	0x%x\n", LayoutMode );
			INFORM("########################################\n\n");
		}
		oskdialog_mode = MODE_OPEN;
	}
	if( oskdialog_mode == START_SEPARATE_TYPE_1 ){
		INFORM( "Separate OSK[REMAIN_OPEN] start \n\n" );
		INFORM( "Set Disable Dimmer OSK\n\n" );

		if( callbacksetflg ){
			register_osk_callback();
		}

		/*J OSKの裏の暗幕を非表示にする */
		ret = cellOskDialogDisableDimmer();
		if( ret ){
			ERR_PRINTF( "cellOskDialogDisableDimmer() = 0x%x \n", ret );
		}
		INFORM( "Set Initial Device Keyboard\n\n" );
		/*J 初期デバイス設定 */
		ret = cellOskDialogSetInitialInputDevice( CELL_OSKDIALOG_INPUT_DEVICE_KEYBOARD );
		if( ret ){
			ERR_PRINTF( "cellOskDialogDisableDimmer() = 0x%x \n", ret );
		}
		/*J 分離型ウィンドウ設定 */
		/*J 入力文字表示ウィンドウの座標の基準点 */
		inputFieldLayoutInfo.layoutMode = CELL_OSKDIALOG_LAYOUTMODE_X_ALIGN_LEFT | CELL_OSKDIALOG_LAYOUTMODE_Y_ALIGN_TOP;
		/*J 入力文字表示ウィンドウの座標 */
		inputFieldLayoutInfo.position.x = -100.0;
		inputFieldLayoutInfo.position.y = 150.0;
		/*J 入力パネル表示表示ウィンドウの座標の基準点 */
		inputPanelLayoutInfo.layoutMode = CELL_OSKDIALOG_LAYOUTMODE_X_ALIGN_LEFT | CELL_OSKDIALOG_LAYOUTMODE_Y_ALIGN_TOP;
		/*J 入力パネル表示表示ウィンドウの座標 */
		inputPanelLayoutInfo.position.x = -100.0;
		inputPanelLayoutInfo.position.y = 0.0;

		WindowOption.continuousMode = CELL_OSKDIALOG_CONTINUOUS_MODE_REMAIN_OPEN;	/*J 確定続行モード */
		WindowOption.deviceMask = 0;												/*J デバイスイベントのマスク設定 */
		WindowOption.inputFieldWindowWidth = 500;									/*J 入力文字表示ウィンドウのサイズ */
		WindowOption.inputFieldBackgroundTrans = 1.0;								/*J 入力領域背景で表示される背景の透過率 */
		WindowOption.inputFieldLayoutInfo = &inputFieldLayoutInfo;					/*J 入力フィールド表示ウィンドウのレイアウト情報 */
		WindowOption.inputPanelLayoutInfo = &inputPanelLayoutInfo;					/*J 入力パネル表示ウィンドウのレイアウト情報 */
		WindowOption.reserved = NULL;												/*J 予約領域（NULL） */

		ret = cellOskDialogSetSeparateWindowOption( &WindowOption );
		if( ret ){
			INFORM( "cellOskDialogSetSeparateWindowOption()= 0x%x \n", ret);
		}

		oskdialog_mode = MODE_OPEN;
	}
	if( oskdialog_mode == START_SEPARATE_TYPE_2 ){
		INFORM( "Separate type OSK[HIDE] start \n\n" );

		if( callbacksetflg ){
			register_osk_callback();
		}

		/*J 初期入力文字列 */
		inputFieldInfo.init_text = (uint16_t*)"";

		/*J キーボード接続状態のチェック */
		if( (ret = cellKbGetInfo (&info)) != CELL_KB_OK ){
			ERR_PRINTF( "cellKbGetInfo() = 0x%x \n", ret );
			oskdialog_mode = MODE_IDLE;
			return 1;
		}
		else if( info.status[0] == CELL_KB_STATUS_DISCONNECTED ){
			INFORM( "keyboard is not connected\n" );
			INFORM( "Please connect a keyboard.\n" );
			oskdialog_mode = MODE_IDLE;
			return 0;
		}

		/*J OSKの裏の暗幕を非表示にする */
		INFORM( "Set Disable Dimmer OSK\n\n" );
		ret = cellOskDialogDisableDimmer();
		if( ret ){
			ERR_PRINTF( "cellOskDialogDisableDimmer() = 0x%x \n", ret );
		}
		INFORM( "Set Initial Device Keyboard.\n\n" );
		/*J 初期デバイス設定 */
		ret = cellOskDialogSetInitialInputDevice( CELL_OSKDIALOG_INPUT_DEVICE_KEYBOARD );
		if( ret ){
			ERR_PRINTF( "cellOskDialogDisableDimmer() = 0x%x \n", ret );
		}
		/*J 分離型ウィンドウ設定 */
		/*J 入力文字表示ウィンドウの座標の基準点 */
		inputFieldLayoutInfo.layoutMode = CELL_OSKDIALOG_LAYOUTMODE_X_ALIGN_LEFT | CELL_OSKDIALOG_LAYOUTMODE_Y_ALIGN_TOP;
		/*J 入力文字表示ウィンドウの座標 */
		inputFieldLayoutInfo.position.x = -100.0;
		inputFieldLayoutInfo.position.y = 150.0;

		WindowOption.continuousMode = CELL_OSKDIALOG_CONTINUOUS_MODE_HIDE;	/*J 確定続行モード */
		WindowOption.deviceMask = CELL_OSKDIALOG_DEVICE_MASK_PAD;			/*J ダイアログで取得しないデバイスイベント */
		WindowOption.inputFieldWindowWidth = 500;							/*J 入力文字表示ウィンドウのサイズ */
		WindowOption.inputFieldBackgroundTrans = 0.0;						/*J 入力領域背景で表示される背景の透過率 */
		WindowOption.inputFieldLayoutInfo = &inputFieldLayoutInfo;			/*J 入力フィールド表示ウィンドウのレイアウト情報 */
		WindowOption.inputPanelLayoutInfo = NULL;							/*J 入力パネル表示ウィンドウのレイアウト情報 */
		WindowOption.reserved = NULL;										/*J 予約領域（NULL） */

		ret = cellOskDialogSetSeparateWindowOption( &WindowOption );
		if( ret ){
			INFORM( "cellOskDialogSetSeparateWindowOption()= 0x%x \n", ret);
		}

		INFORM("OSK is hidden. Please input by keyboard. \n");

		oskdialog_mode = MODE_OPEN;
	}
	if( oskdialog_mode == SET_ABORT_TIMER ){
		abortflg = !abortflg;
		if( abortflg ){
			INFORM( "Set Abort Timer %d seconds\n\n", ABORT_TIME );
			strcpy(menu_command[MCMD_SET_ABORT_TIMER].string, "Set Abort Timer ON/OFF (current setting:ON)");
		}
		else{
			INFORM( "Set Abort Timer OFF\n\n" );
			strcpy(menu_command[MCMD_SET_ABORT_TIMER].string, "Set Abort Timer ON/OFF (current setting:OFF)");
		}
		oskdialog_mode = MODE_IDLE;
	}
	if( oskdialog_mode == SET_CALLBACK ){
		callbacksetflg = !callbacksetflg;
		if( callbacksetflg ){
			INFORM( "Set Callback ON\n\n" );
			strcpy(menu_command[MCMD_SET_CALLBACK].string, "Set Callback (current setting:ON)");
		}
		else{
			INFORM( "Set Callback OFF\n\n" );
			strcpy(menu_command[MCMD_SET_CALLBACK].string, "Set Callback (current setting:OFF)");
		}
		oskdialog_mode = MODE_IDLE;
	}

	if( oskdialog_mode == CHANGE_PANEL_MODE ){
		static int panelmode = 1;

		panelmode = panelmode % 3 + 1;
		switch( panelmode ){
		case 1:
			dialogParam.allowOskPanelFlg = CELL_OSKDIALOG_PANELMODE_DEFAULT;
			dialogParam.firstViewPanel = CELL_OSKDIALOG_PANELMODE_DEFAULT;
			INFORM( "Change Panel Mode to DEFAULT\n" );
			strcpy(menu_command[MCMD_CHANGE_PANEL_MODE].string, "Change Panel Mode (current mode:DEFAULT)");
			break;
		case 2:
			dialogParam.allowOskPanelFlg = CELL_OSKDIALOG_PANELMODE_DEFAULT_NO_JAPANESE;
			dialogParam.firstViewPanel = CELL_OSKDIALOG_PANELMODE_DEFAULT_NO_JAPANESE;
			INFORM( "Change Panel Mode to DEFAULT_NO_JAPANESE\n" );
			strcpy(menu_command[MCMD_CHANGE_PANEL_MODE].string, "Change Panel Mode (current mode:DEFAULT_NO_JAPANESE)");
			break;
		case 3:
			dialogParam.allowOskPanelFlg = CELL_OSKDIALOG_PANELMODE_ALPHABET;
			dialogParam.firstViewPanel = CELL_OSKDIALOG_PANELMODE_ALPHABET;
			INFORM( "Change Panel Mode to ALPHABET\n" );
			strcpy(menu_command[MCMD_CHANGE_PANEL_MODE].string, "Change Panel Mode (current mode:ALPHABET)");
			break;
		default:
			dialogParam.allowOskPanelFlg = CELL_OSKDIALOG_PANELMODE_DEFAULT;
			dialogParam.firstViewPanel = CELL_OSKDIALOG_PANELMODE_DEFAULT;
			INFORM( "Change Panel Mode to DEFAULT\n" );
			strcpy(menu_command[MCMD_CHANGE_PANEL_MODE].string, "Change Panel Mode (current mode:DEFAULT)");
			break;
		}
		oskdialog_mode = MODE_IDLE;
	}
	if( oskdialog_mode == CHANGE_SCALE ){
		windowScale += 0.05f;
		if( windowScale > CELL_OSKDIALOG_SCALE_MAX ){
			windowScale = CELL_OSKDIALOG_SCALE_MIN;
		}
		INFORM( "Change Scale to %.2f\n\n",windowScale );
		sprintf( menu_command[MCMD_CHANGE_SCALE].string, "Change Scale (current scale:%.2f)", windowScale );
		oskdialog_mode = MODE_IDLE;
	}
	if( oskdialog_mode == MODE_OPEN ){
		/*J OSKのサポート言語を追加する */
		ret = cellOskDialogAddSupportLanguage( CELL_OSKDIALOG_PANELMODE_KOREAN				|
											   CELL_OSKDIALOG_PANELMODE_DANISH				|
											   CELL_OSKDIALOG_PANELMODE_SWEDISH				|
											   CELL_OSKDIALOG_PANELMODE_NORWEGIAN			|
											   CELL_OSKDIALOG_PANELMODE_FINNISH				|
											   CELL_OSKDIALOG_PANELMODE_POLISH				|
											   CELL_OSKDIALOG_PANELMODE_PORTUGUESE_BRAZIL	|
											   CELL_OSKDIALOG_PANELMODE_TRADITIONAL_CHINESE	|
											   CELL_OSKDIALOG_PANELMODE_SIMPLIFIED_CHINESE );
		if( ret ){
			ERR_PRINTF( "cellOskDialogAddSupportLanguage() = 0x%x \n", ret );
		}

		/*J キーレイアウト設定 */
		ret = cellOskDialogSetKeyLayoutOption( CELL_OSKDIALOG_10KEY_PANEL | CELL_OSKDIALOG_FULLKEY_PANEL );
		if( ret ){
			ERR_PRINTF( "cellOskDialogSetKeyLayoutOption() = 0x%x \n", ret );
		}	

		/*J 初期キーレイアウト設定 */
		ret = cellOskDialogSetInitialKeyLayout( CELL_OSKDIALOG_INITIAL_PANEL_LAYOUT_FULLKEY );
		if( ret ){
			ERR_PRINTF( "cellOskDialogSetKeyLayoutOption()= 0x%x \n", ret);
		}
		
		/*J 初期スケール設定 */
		ret = cellOskDialogExtSetInitialScale( windowScale );
		if( ret ){
			ERR_PRINTF( "cellOskDialogExtSetInitialScale()= 0x%x \n", ret);
		}

		/*J PSボタン短押しの挙動を制御するコールバック登録 */
		cellOskDialogExtRegisterForceFinishCallback( force_finish_callback );

		ret = cellOskDialogLoadAsync( container_id, &dialogParam, &inputFieldInfo );
		if( ret ){
			ERR_PRINTF( "cellOskDialogLoadAsync() = 0x%x \n", ret );
			oskdialog_mode = MODE_EXIT;
			return ret;
		}
		else{
			oskdialog_mode = MODE_RUNNING;
		}
	}
	if( oskdialog_mode == MODE_ENTERED ){
		enteredCounter++;
		ret = cellOskDialogGetInputText( &OutputInfo );
		if( !ret && (OutputInfo.result == CELL_OSKDIALOG_INPUT_FIELD_RESULT_OK) ){
			INFORM("cellOskDialogGetInputText	:	%S\n", (wchar_t*)OutputInfo.pResultString);
		}
		else{
			ERR_PRINTF( "cellOskDialogGetInputText() = 0x%x \n", ret );
		}
		oskdialog_mode = MODE_RUNNING;
		if( enteredCounter >= ENTERED_TIMES ){
			INFORM( "Entered %d Times \n\n", ENTERED_TIMES );
			ret = cellOskDialogAbort();
			enteredCounter = 0;
		}
	}
	if( oskdialog_mode == MODE_CANCELED ){
		oskdialog_mode = MODE_RUNNING;
		INFORM( "Input Canceled\n\n" );
		ret = cellOskDialogAbort();
	}

	if( oskdialog_mode == MODE_CLOSE ){
		ret = cellOskDialogUnloadAsync( &OutputInfo );
		if( ret ){
			ERR_PRINTF( "cellOskDialogUnloadAsync() = 0x%x \n", ret );
			oskdialog_mode = MODE_EXIT;
		}
		else{
			inputFieldInfo.init_text = (uint16_t*)INIT_TEXT;		/*J 初期入力文字列 */
			oskdialog_mode = MODE_IDLE;
			counter = 0;
			abortflg = false;
			/*J  無条件でOFFの文字列に入れ替え*/
			strcpy( menu_command[MCMD_SET_ABORT_TIMER].string, "Set Abort Timer ON/OFF (current setting:OFF)" );

			INFORM("##############   result   ##############\n");
			INFORM( "OskDialog result status:	%d\n", OutputInfo.result );
			if( OutputInfo.result == CELL_OSKDIALOG_INPUT_FIELD_RESULT_OK ){
				INFORM( "OskDialog return chars :	%d\n", OutputInfo.numCharsResultString);
				INFORM( "OskDialog return text  :	%S\n", (wchar_t*)OutputInfo.pResultString);
			}
			INFORM("########################################\n\n");
		}
	}
	if( (oskdialog_mode != MODE_IDLE) && (abortflg == true) ){
		counter++;
		if( counter == 60 * ABORT_TIME ){
			ret = cellOskDialogAbort();
			if ( ret == CELL_OK ) {
				INFORM( " !! cellOskDialogAbort() success !! \n" );
			}
			else {
				INFORM( "cellOskDialogAbort() = 0x%x \n", ret );
				oskdialog_mode = MODE_EXIT;
			}
		}
	}
	if( (oskdialog_mode != MODE_IDLE) && (deviceLock == true) ){
		if( (sys_time_get_system_time() - lockedTime) >= DEVICE_UNLOCK_TIME ){
			ret = cellOskDialogExtInputDeviceUnlock();
			if( !ret ){
				deviceLock = false;
				lockedTime = 0;
			}
			INFORM("The device lock was removed!!\n");
		}
	}

	return ret;
}

SYS_PROCESS_PARAM(1001, 0x10000)

int main( void )
{
	INFORM("sysutil_oskdialog_sample start\n");
	int ret;

	/*J モジュールのロード、メモリコンテナ作成等 */
	ret = initializeModule();
	if( ret != 0 ){
		free( host_addr );
		return -1;
	}
	/*J OSKの起動パラメータを設定する */
	setOskFieldParams();

	/*J 決定ボタンアサインを取得 */
	is_cross_enter = isCrossButtonEnter();

	/* main loop */
	while( (pad_read() != 0) && (oskdialog_mode != MODE_EXIT) ){
#ifdef	_QA_3D_
		for( int lr=0; lr<2; lr++ ){
		setRenderTarget(lr);
#endif

		// clear frame buffer
		cellGcmSetClearSurface( CELL_GCM_CLEAR_Z	|
								CELL_GCM_CLEAR_R	|
								CELL_GCM_CLEAR_G	|
								CELL_GCM_CLEAR_B	|
								CELL_GCM_CLEAR_A );
		// set draw command
		cellGcmSetDrawArrays( CELL_GCM_PRIMITIVE_TRIANGLES, 0, 3 );
#ifdef	_QA_3D_
		}
		for( int lr=0; lr<2; lr++ ){
		setRenderTarget(lr);
		titleDbgPrintf(lr);
#else
		titleDbgPrintf(0);
#endif

		if( oskdialog_mode == MODE_IDLE ){
			/* draw Menu */
			drawListMenu( menu_command, MCMD_NUM, command_selected );
		}

		/*J デバックフォントを描画する */
		drawDbgFont();
#ifdef	_QA_3D_
		}
#endif
		// start reading the command buffer
		flip();

		ret = osk_main();
		if( ret ) break;

	}/* End main loop */

	/*J モジュールのアンロード、メモリコンテナの破棄等 */
	ret = finalizeModule();

	INFORM("sysutil_oskdialog_sample end\n");

	return 0;
}

