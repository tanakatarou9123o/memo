// dllmain.cpp : DLL アプリケーションのエントリ ポイントを定義します。
#include "stdafx.h"

#pragma comment(linker, "/Section:MY_DATA,RWS")//共有変数 プロセス間
#pragma data_seg("MY_DATA") 
HHOOK hhok = 0;									////共有変数 プロセス間 フックプロシージャハンドル
#pragma data_seg()

#include "APIHOOKDLL.h"//ヘッダ
HINSTANCE hInst;//グローバル変数




BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH: 
		hInst = hModule;
		//MessageBox(NULL, TEXT("アタッチしました"), TEXT("OK"), MB_OK); 
		break;

	case DLL_PROCESS_DETACH: 
		//MessageBox(NULL, TEXT("デタッチしました"), TEXT("OK"), MB_OK); 
		break;

	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}


EXPORT HHOOK starthook()//フック開始関数
{
	hhok = SetWindowsHookEx
		(
		WH_KEYBOARD | WH_MOUSE, // インストールするフックのタイプ マウスとキーボード
		(HOOKPROC)MyHookProc, // フックプロシージャのアドレス
		hInst, // インスタンスハンドル 
		0 // スレッドID 
		);

	return hhok;
}

EXPORT BOOL closehook()//フック終了関数
{
	if (!UnhookWindowsHookEx(hhok))// 取り除くフックプロシージャハンドル
	{
		MessageBox(NULL, TEXT(""), TEXT(""), MB_OK);
	}

	return true;
}



LRESULT CALLBACK MyHookProc(int nCode, WPARAM wp, LPARAM lp) //フックプロシージャ
{

	if (nCode < 0)//マイナスなら
	{
		return CallNextHookEx(hhok, nCode, wp, lp);
	}

	if (wp == WM_LBUTTONUP)//左クリック取得
	{
		MessageBox(NULL, TEXT("左クリック"), TEXT("OK"), MB_OK);
		return CallNextHookEx(hhok, nCode, wp, lp);
	}



	return CallNextHookEx(hhok, nCode, wp, lp);//本来の処理へ渡す


	return 0;
}