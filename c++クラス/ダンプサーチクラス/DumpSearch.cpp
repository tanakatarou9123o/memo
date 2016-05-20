#include "stdafx.h"
#include "DumpSearch.h"
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

void DumpSearch::ReadNew(WCHAR* fn)//初めに新規検索する ファイルサイズの数だけnewする byteフラグに応じて
{
	if (pbuf)//二回目の新規検索ならメモリが確保されてるので
	{
		delete pbuf;//NULLでない場合メモリを解放
	}
	if (pwbuf)
	{
		delete pwbuf;//NULLでない場合メモリを解放
	}
	if (pibuf)
	{
		delete pibuf;//NULLでない場合メモリを解放
	}

	ifstream RPE(fn, ios::in | ios::binary);//ファイルパス 読み込みモード バイナリで開く
	if (!RPE)
	{
		MessageBox(NULL, L"open失敗", L"ReadNew", MB_OK);
		return;//失敗
	}
	RPE.seekg(0, ios::end);//seekを最後に移動
	fileSize = (int)RPE.tellg();//サイズ取得 現在位置を調べる関数
	RPE.seekg(0, ios_base::beg);//seek最初に


	int i = 0;
	int m = 0;//pwbufの一致場所
	//int m = 0;//pibufの一致場所
	
	switch (*flag)
	{
	case 1://1byte
		amari = 0;
		pbuf = new char[fileSize];//ファイルサイズの分だけ確保

		while (!RPE.eof())
		{
			RPE.read(&pbuf[i], 1);
			i++;
		}

		break;
	case 2://2byte
		fsw = fileSize / 2;//2byteの配列の個数
		amari = fileSize % 2;//余りが出るか調べる
		if (amari != 0)//余りがある
		{
			pwbuf = new WORD[fsw + 1];//余りが出る場合のため+1 してメモリ確保
			for (int i = 0; i < fsw+1; i++)//charブッファからWORDバッファ作成
			{
				char temp[2];
				RPE.read(temp, 2);
				if (RPE.eof())
				{
					pwbuf[m] = (0xff & temp[0]);
					pwbuf[m] = pwbuf[m] << 8;//1byte 左シフト
					pwbuf[m] += (0xff & temp[1]);
					break;
				}

				pwbuf[m] = (0xff & temp[0]);
				pwbuf[m] = pwbuf[m] << 8;//1byte 左シフト
				pwbuf[m] += (0xff & temp[1]);

				m++;
			}
			break;
		}

		pwbuf = new WORD[fsw];//余りが出ない メモリ確保
		for (int i = 0; i < fsw; i++)//charブッファからWORDバッファ作成
		{
			char temp[2];
			RPE.read(temp, 2);

			pwbuf[m] = (0xff & temp[0]);
			pwbuf[m] = pwbuf[m] << 8;//1byte 左シフト
			pwbuf[m] += (0xff & temp[1]);

			m++;
		}
		break;

	case 4:
		fsi = fileSize / 4;//4byteの配列の個数
		amari = fileSize % 4;//余りが出るか調べる
		if (amari!=0)//余りがある
		{
			pibuf = new int[fsi + 1];//余りが出る場合のため+1 してメモリ確保
			for (int i = 0; i < fsi+1; i += 4)//charブッファからintバッファ作成
			{
				char temp[4];
				RPE.read(temp, 4);
				if (RPE.eof())//割り切れたらeofは発生しない
				{
					pibuf[m] = (0xff & temp[0]);
					pibuf[m] = pibuf[m] << 8;//1byte 左シフト
					pibuf[m] += (0xff & temp[1]);
					pibuf[m] = pibuf[m] << 8;//1byte 左シフト
					pibuf[m] += (0xff & temp[2]);
					pibuf[m] = pibuf[m] << 8;//1byte 左シフト
					pibuf[m] += (0xff & temp[3]);
					break;
				}

				pibuf[m] = (0xff & temp[0]);
				pibuf[m] = pibuf[m] << 8;//1byte 左シフト
				pibuf[m] += (0xff & temp[1]);
				pibuf[m] = pibuf[m] << 8;//1byte 左シフト
				pibuf[m] += (0xff & temp[2]);
				pibuf[m] = pibuf[m] << 8;//1byte 左シフト
				pibuf[m] += (0xff & temp[3]);

				m++;
			}
			break;

		}
		
		pibuf = new int[fsi];//余りが出ない メモリ確保
		for (int i = 0; i < fsi; i++)//charブッファからintバッファ作成
		{
			char temp[4];
			RPE.read(temp, 4);

			pibuf[m] = (0xff & temp[0]);
			pibuf[m] = pibuf[m] << 8;//1byte 左シフト
			pibuf[m] += (0xff & temp[1]);
			pibuf[m] = pibuf[m] << 8;//1byte 左シフト
			pibuf[m] += (0xff & temp[2]);
			pibuf[m] = pibuf[m] << 8;//1byte 左シフト
			pibuf[m] += (0xff & temp[3]);

			m++;
		}
		break;

	default:
		break;
	}
	

}

void DumpSearch::ReadFile(WCHAR* fn)//二回目以降
{
	if (!pbuf && !pwbuf && !pibuf)
	{
		return;
	}

	ifstream RPE(fn, ios::in | ios::binary);
	if (!RPE)
	{
		//MessageBox(NULL, L"open失敗", L"", MB_OK);
		return;//失敗
	}
	RPE.seekg(0, ios::end);//seekを最後に移動
	int fileSize2 = (int)RPE.tellg();//サイズ取得 現在位置を調べる関数
	RPE.seekg(0, ios_base::beg);//seek最初に
	if (fileSize!=fileSize2)
	{
		MessageBox(NULL, L"ファイルサイズが違います", L"", MB_OK);
		return;
	}

	int i = 0;
	switch (*flag)
	{
	case 1://1byte
		while (!RPE.eof())
		{
			RPE.read(&pbuf[i], 1);
			i++;
		}

		break;
	case 2://2byte
		if (amari != 0)//余りがある
		{
			for (int i = 0; i < fsw + 1; i++)//charブッファからWORDバッファ作成
			{
				char temp[2];
				RPE.read(temp, 2);

				pwbuf[i] = (0xff & temp[0]);
				pwbuf[i] = pwbuf[i] << 8;//1byte 左シフト
				pwbuf[i] += (0xff & temp[1]);

			}
		}
		else
		{
			for (int i = 0; i < fsw; i++)//charブッファからWORDバッファ作成
			{
				char temp[2];
				RPE.read(temp, 2);

				pwbuf[i] = (0xff & temp[0]);
				pwbuf[i] = pwbuf[i] << 8;//1byte 左シフト
				pwbuf[i] += (0xff & temp[1]);

			}
		}
		

		break;
	case 4:
		if (amari != 0)//余りがある
		{
			for (int i = 0; i < fsi + 1; i++)//charブッファからintバッファ作成
			{
				char temp[4];
				RPE.read(temp, 4);

				pibuf[i] = (0xff & temp[0]);
				pibuf[i] = pibuf[i] << 8;//1byte 左シフト
				pibuf[i] += (0xff & temp[1]);
				pibuf[i] = pibuf[i] << 8;//1byte 左シフト
				pibuf[i] += (0xff & temp[2]);
				pibuf[i] = pibuf[i] << 8;//1byte 左シフト
				pibuf[i] += (0xff & temp[3]);

			}
		}
		else
		{
			for (int i = 0; i < fsi; i++)//charブッファからintバッファ作成
			{
				char temp[4];
				RPE.read(temp, 4);

				pibuf[i] = (0xff & temp[0]);
				pibuf[i] = pibuf[i] << 8;//1byte 左シフト
				pibuf[i] += (0xff & temp[1]);
				pibuf[i] = pibuf[i] << 8;//1byte 左シフト
				pibuf[i] += (0xff & temp[2]);
				pibuf[i] = pibuf[i] << 8;//1byte 左シフト
				pibuf[i] += (0xff & temp[3]);

			}
		}

		break;

	default:
		break;
	}

	
}

void DumpSearch::SearchStart()//新規検索 エンディアンから検索数値を変換
{
	if (!pbuf && !pwbuf && !pibuf)//nullなら
	{
		return;
	}

	SendMessage((*hwn)["HLISTBOX1"] , LB_RESETCONTENT, 0, 0);	//listbox クリア
	adores.clear();//アドレスベクタークリア

	WCHAR text[50];
	GetWindowText((*hwn)["HEDIT1"] , text, 50);//ウィンドウハンドルのテキストを取得

	int s1;
	s1 = _wtoi(text);
	
	if ((text[0]<0x30)|(text[0]>0x39))//数値以外なら 文字列チェック
	{
		MessageBox(NULL, L"文字列です", L"", MB_OK);
		return;
	}

	if (*flag==1)//1byte
	{
		for (int i = 0; i < fileSize; i++)
		{
			int b = (0xff & pbuf[i]);//オーバーフロー対策
			if (s1==b)
			{
				WCHAR s1[50];

				wsprintf(s1, TEXT("0x%08X"), i);

				SendMessage((*hwn)["HLISTBOX1"], LB_ADDSTRING, 0, (LPARAM)s1);	//listbox 項目追加 ウィンドウタイトル

				adores.push_back(i);//アドレスの場所
			}
		}
		SetWindowText((*hwn)["HWND1"], L"1byte");
	}
	else if (*flag == 2)//2byte ビッグエンディアン
	{
		int m = 0;//pwbufの一致場所
		if (amari!=0)//余りがある
		{
			for (int i = 0; i < fsi + 2; i += 2)//charブッファからWORDバッファ作成
			{
				if (s1 == pwbuf[m])
				{
					WCHAR s1[50];
					wsprintf(s1, TEXT("0x%08X"), i);

					SendMessage((*hwn)["HLISTBOX1"], LB_ADDSTRING, 0, (LPARAM)s1);	//listbox 項目追加 ウィンドウタイトル

					adores.push_back(m);//アドレスの場所
				}

				m++;
			}
		}
		else
		{
			for (int i = 0; i < fileSize; i += 2)//charブッファからWORDバッファ作成
			{
				if (s1 == pwbuf[m])
				{
					WCHAR s1[50];
					wsprintf(s1, TEXT("0x%08X"), i);

					SendMessage((*hwn)["HLISTBOX1"], LB_ADDSTRING, 0, (LPARAM)s1);	//listbox 項目追加 ウィンドウタイトル

					adores.push_back(m);//アドレスの場所
				}

				m++;
			}
		}
		

		SetWindowText((*hwn)["HWND1"], L"2byte");

	}
	else if (*flag == 4)//4byte
	{
		int m = 0;//pibufの一致場所

		if (amari != 0)//余りがある
		{
			for (int i = 0; i < fileSize + 4; i += 4)//charブッファからWORDバッファ作成
			{
				if (s1 == pibuf[m])
				{
					WCHAR s1[50];
					wsprintf(s1, TEXT("0x%08X"), i);

					SendMessage((*hwn)["HLISTBOX1"], LB_ADDSTRING, 0, (LPARAM)s1);	//listbox 項目追加 ウィンドウタイトル

					adores.push_back(m);//アドレスの場所
				}

				m++;
			}

		}
		else
		{
			for (int i = 0; i < fileSize; i += 4)//charブッファからWORDバッファ作成
			{
				if (s1 == pibuf[m])
				{
					WCHAR s1[50];
					wsprintf(s1, TEXT("0x%08X"), i);

					SendMessage((*hwn)["HLISTBOX1"], LB_ADDSTRING, 0, (LPARAM)s1);	//listbox 項目追加 ウィンドウタイトル

					adores.push_back(m);//アドレスの場所
				}

				m++;
			}

		}
		
		

		SetWindowText((*hwn)["HWND1"], L"4byte");
	}

}

void DumpSearch::SearchRefine()//繰り返し検索
{
	if (flag==0)
	{
		return;
	}

	SendMessage((*hwn)["HLISTBOX1"], LB_RESETCONTENT, 0, 0);	//listbox クリア

	WCHAR text[50];
	GetWindowText((*hwn)["HEDIT1"], text, 50);//ウィンドウハンドルのテキストを取得
	int s1 = _wtoi(text);

	switch (*flag)
	{
	case 1://1byte
	{
		SetWindowText((*hwn)["HWND1"], L"1byte");
		vector<int>::iterator pa;//コンテナの位置を格納する変数
		pa = adores.begin();//コンテナの最初の要素を示す

		while (pa != adores.end())//最後でないなら繰り返す
		{
			int b = (0xff & pbuf[*pa]);//オーバーフロー対策
			if (s1 == b)
			{
				WCHAR s1[50];

				wsprintf(s1, TEXT("0x%08X"), *pa);

				SendMessage((*hwn)["HLISTBOX1"], LB_ADDSTRING, 0, (LPARAM)s1);	//listbox 項目追加 ウィンドウタイトル

				++pa;//次の要素に
			}
			else
			{
				pa = adores.erase(pa);//削除
			}
		}
	}
		break;
	case 2://2byte
	{
		SetWindowText((*hwn)["HWND1"], L"2byte");

		vector<int>::iterator pa;//コンテナの位置を格納する変数
		pa = adores.begin();//コンテナの最初の要素を示す

		while (pa != adores.end())//最後でないなら繰り返す
		{
			int b = (0xffff & pwbuf[*pa]);//オーバーフロー対策
			if (s1 == b)
			{
				WCHAR s1[50];

				wsprintf(s1, TEXT("0x%08X"), (*pa)*2);//WORDだから2倍してる

				SendMessage((*hwn)["HLISTBOX1"], LB_ADDSTRING, 0, (LPARAM)s1);	//listbox 項目追加 ウィンドウタイトル

				++pa;//次の要素に
			}
			else
			{
				pa = adores.erase(pa);//削除
			}
		}
	}

		break;
	case 4://4byte
	{
		SetWindowText((*hwn)["HWND1"], L"4byte");

		vector<int>::iterator pa;//コンテナの位置を格納する変数
		pa = adores.begin();//コンテナの最初の要素を示す

		while (pa != adores.end())//最後でないなら繰り返す
		{
			int b = pibuf[*pa];//オーバーフロー対策
			if (s1 == b)
			{
				WCHAR s1[50];

				wsprintf(s1, TEXT("0x%08X"), (*pa) * 4);//intだから4倍してる

				SendMessage((*hwn)["HLISTBOX1"], LB_ADDSTRING, 0, (LPARAM)s1);	//listbox 項目追加 ウィンドウタイトル

				++pa;//次の要素に
			}
			else
			{
				pa = adores.erase(pa);//削除
			}
		}
	}
		break;

	default:
		break;
	}

}


DumpSearch::DumpSearch(map<char*, HWND>* h, unsigned char* byteflag)
{
	hwn=h;//ウィンドウハンドルのコンテナのアドレス受け取る 
	flag = byteflag;
}


DumpSearch::~DumpSearch()
{
}
