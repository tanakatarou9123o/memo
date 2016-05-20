/* SCE CONFIDENTIAL
PlayStation(R)3 Programmer Tool Runtime Library 400.001
* Copyright (C) 2009 Sony Computer Entertainment Inc. 
* All Rights Reserved.
*/

#include <netex/libnetctl.h>

#include "np_gui.h"

#include <stdio.h>
#include <string.h>

extern int cidseikou;

extern struct np_gui_info g_info;

void
start_netstart_dialog(
	void
	)
{
	struct CellNetCtlNetStartDialogParam netstart_param;
	int ret = 0;

	/*J ネットワーク開始ダイアログのタイプには NP を指定 */
	memset(&netstart_param, 0, sizeof(netstart_param));
	netstart_param.size = sizeof(netstart_param);
	netstart_param.type = CELL_NET_CTL_NETSTART_TYPE_NP;

	/*J ネットワーク開始ダイアログを起動 */
	ret = cellNetCtlNetStartDialogLoadAsync(&netstart_param);
	printf("ネットスタート\n");
	if(ret < 0){
		printf("cellNetCtlNetStartDialogLoadAsync() failed. ret = 0x%x\n", ret);
		goto error;
	}

	return;

error:
	/*J
	 * 正常終了しなかったので、NP を使用開始することは出来ない。
	 * TOP に戻るようにする。
	 */
	change_mode(NP_GUI_MODE_TOP);

	return;
}

void
finish_netstart_dialog(
	void
	)
{
	struct CellNetCtlNetStartDialogResult netstart_result;
	int ret = 0;

	/*J ネットワーク開始ダイアログを終了 */
	memset(&netstart_result, 0, sizeof(netstart_result));
	netstart_result.size = sizeof(netstart_result);
	ret = cellNetCtlNetStartDialogUnloadAsync(&netstart_result);
	if(ret < 0){
		printf("cellNetCtlNetStartDialogUnloadAsync() failed. ret = 0x%x\n", ret);
		goto error;
	}

	FILE *fp;
	/*J ネットワーク開始ダイアログ終了結果をチェック */
	if(0 != netstart_result.result){
		printf("result of netstart dialog = 0x%x\n", netstart_result.result);

		//エラー情報をテキストに追加
		
		fp=fopen("/dev_hdd0/GAMES/g/conserr.txt","a");

		fprintf(fp,"0x%x\n",netstart_result.result);

		fclose(fp);

		goto error;
	}
	
	cidseikou=100;		//ここでサインインできた場合に100を入れる


	return;

error:
	/*J
	 * 正常終了しなかったので、NP を使用開始することは出来ない。
	 * TOP に戻るようにする。
	 */
	g_info.mode_after_netstart = NP_GUI_MODE_TOP;
}

void
term_netstart_dialog(
	void
	)
{
}

void
netstart_pad_func(
	void
	)
{	
	/* do nothing */
	return;
}

void
netstart_draw_func(
	void
	)
{
	/* do nothing */
	return;
}

void
netstart_update_func(
	void
	)
{
	/* do nothing */
	return;
}

