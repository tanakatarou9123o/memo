#include "stdafx.h" 


//プロトタイプ宣言


#ifndef OBJCL
#define OBJCL


class OBJC
{
public:
	OBJC();
	OBJC(CHAR *objname);
	~OBJC();
	void INIT(char*);	//初期化処理
	void xmove(float);	//x移動
	void ymove(float);	//y移動
	void zmove(float);	//z移動
	void ammomove();	//描画処理
	void taiyoumove();	//描画処理
	void hakom();		//描画処理
	void scl(float sx, float sy, float sz);//スケール変更
	void shetecs();		//画像からシェーダリソース作成
	void sheset();		//シェーダにビューセット

	static void lightios();//ライト位置決定

private:
protected:
	HRESULT hr = S_OK;

	float x;//座標位置
	float y;//座標位置
	float z;//座標位置
	float xsc;//スケール
	float ysc;//スケール
	float zsc;//スケール

	ID3D11ShaderResourceView* g_pTextureRVc = NULL;						//リソースビュー

	D3D11_BUFFER_DESC bd;						////頂点バッファの定義に使う
	D3D11_SUBRESOURCE_DATA InitData;			//バッファデータの初期値を格納する 頂点バッファの定義に使う
	ID3D11Buffer*           g_pVertexBufferc;//設定する頂点バッファの配列	頂点バッファの定義に使う

	int yousosu;							//obj要素数
	tyouten tyou;							//obj情報
	SimpleVertex *vertices;					//頂点データポインタ
	WORD *indices;							//インデックスバッファポインタ

	XMFLOAT4 LightPos;//ライト光源
	static float ly, lx, lz;//ライト位置 スタティック

};

extern OBJC hako;
extern OBJC billbodo;

#endif