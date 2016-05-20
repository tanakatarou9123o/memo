#ifndef DumpSearchaH
#define DumpSearchH
#include <vector>
#include <map>//マップクラス
using namespace std;

class DumpSearch
{
public:
	DumpSearch(map<char*, HWND>*,unsigned char*);//コンテナでウィンドウハンドル取得 コンストラクタ byteflagのアドレス受け取る
	~DumpSearch();
	void SearchStart();
	void SearchRefine();
	void ReadFile(WCHAR* fn);
	void ReadNew(WCHAR* fn);

private:
	map<char*, HWND>* hwn;//ウィンドウハンドル コンテナ

	char* pbuf = NULL;//1byte
	WORD* pwbuf = NULL;//2byte
	int* pibuf = NULL;//4byte
	int fileSize;//char配列数
	int fsw;//WORD配列数
	int fsi;//int配列数
	int amari;//余りが出るか
	vector<int> adores;//ここにヒットしたアドレスを格納

	unsigned char* flag;//バイト数 識別フラグ
};

#endif