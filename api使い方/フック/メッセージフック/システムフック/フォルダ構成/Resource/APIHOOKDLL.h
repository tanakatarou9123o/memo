#ifdef __cplusplus
#define EXPORT extern "C" __declspec(dllexport)		//C呼び出しの場合
#else
#define EXPORT __declspec(dllexport)				//C++呼び出しの場合
#endif



EXPORT HHOOK starthook(void);
EXPORT BOOL closehook(void);//フック終了関数
LRESULT CALLBACK MyHookProc(int nCode, WPARAM wp, LPARAM lp); //フックプロシージャ プロトタイプ