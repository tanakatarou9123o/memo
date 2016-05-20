#ifndef WINCLASSH
#define WINCLASSH
// ヘッダファイルの中身

#define MAX_LOADSTRING 100

class WinClass
{
public:
	WinClass();
	~WinClass();
	virtual ATOM MyRegisterClass(HINSTANCE hInstance);//ウィンドウクラス登録
	BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);//ウィンドウ作成
	int MessageWhile();//メッセージループ

	// バージョン情報ボックスのメッセージ ハンドラーです。
	//static INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	//static HINSTANCE hInsts;


private:
protected:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);//コールバック関数 スタティック

	HINSTANCE hInst;								// 現在のインターフェイス
	TCHAR szTitle[MAX_LOADSTRING];					// タイトル バーのテキスト
	TCHAR szWindowClass[MAX_LOADSTRING];			// メイン ウィンドウ クラス名
	MSG msg;
	HACCEL hAccelTable;

};

#endif