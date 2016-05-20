#include "padutil.h"

int main()
{
	
	cellPadUtilPadInit();		//パッド初期化
	
	int cono=-1;
	while(cono==-1)
	{
		cellPadUtilUpdate();		//パッド情報更新
		cono=cellPadUtilGetFirstConnectedPad();		//パッドナンバー取得
	}
	if(cono==-1){return 1;}

	while(1)
	{
		cellPadUtilUpdate();			//パッド情報更新
		if(cellPadUtilButtonReleasedOnce(cono,CELL_UTIL_BUTTON_START))	//ボタンが離された瞬間を取得
		{return 0;}
	}

    	cellPadUtilPadEnd();		//パッド終了

	return 0;
}