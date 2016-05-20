#include "stdafx.h"
#include "obj派生クラス.h" 


YUKA::YUKA()
{
	;
}

YUKA::YUKA(char* a) :OBJC(a)
{
	;
}

void YUKA::yukakaiten()
{
	//lightios();

	g_pImmediateContext->PSSetShaderResources	//リソースを設定
		(
		0,				//設定する最初のスロット番号
		1,				//設定するシェーダリソースビュー数
		&g_pTextureRV	//設定するシェーダリソースビュー配列
		);

	// Set vertex buffer
	UINT stride = sizeof(SimpleVertex); //頂点バッファに含まれる頂点データのサイズ
	UINT offset = 0;					//頂点バッファ1のオフセット

	g_pImmediateContext->IASetVertexBuffers//入力アセンブラに頂点バッファを設定
		(
		0,								//設定する最初の入力スロット番号
		1, 								//設定する頂点バッファの数
		&g_pVertexBufferc,				//設定する頂点バッファの配列
		&stride, 						//各頂点バッファの要素サイズ
		&offset							//各頂点バッファのオフセット
		);



	static float kait = 0;
	static DWORD zikant = 0, zikango = 0;

	zikant = timeGetTime();

	if (zikango + 10<zikant)
	{
		kait -= 0.0005f;
		zikango = zikant;
		if (kait<-10)
		{
			kait = 0;
		}
	}

	LightPos = XMFLOAT4(lx, ly, 0.0f, 1.0f);//ライト光源

	XMMATRIX mSpin1 = XMMatrixRotationY(kait * D3DX_PI);								//z軸回転
	XMMATRIX mOrbit1 = XMMatrixRotationX(2 * D3DX_PI);						//t分 y軸回転 x移動速度	向き
	XMMATRIX mTranslate1 = XMMatrixTranslation(0, 0.0f, 50);		//平行移動 xyz
	XMMATRIX mScale1 = XMMatrixScaling(1.0f, 1.0f, 1.0f);				//xyz 分 拡大
	XMMATRIX g_World = mScale1 * mSpin1 * mTranslate1 * mOrbit1;

	//キューブの構造体を設定 定数バッファをシェーダに送る
	ConstantBuffer cb;
	cb.mWorld = XMMatrixTranspose(g_World);
	cb.mView = XMMatrixTranspose(g_View);												//カメラ
	cb.mProjection = XMMatrixTranspose(g_Projection);
	cb.mLight = LightPos;																//ライト

	g_pImmediateContext->UpdateSubresource(g_pConstantBuffer, 0, NULL, &cb, 0, 0);//定数バッファを更新
	g_pImmediateContext->Draw(yousosu, 0);


}
