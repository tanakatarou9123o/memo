#include "stdafx.h"
#include "objクラス.h"

//定義部分

OBJC::OBJC()
{

}

OBJC::OBJC(CHAR *objname)
{
	OBJ(objname, tyou);//obj読み込み
	vertices = new SimpleVertex[tyou.menindex];	//面の数だけ配列確保
	float iro = 0;
	for (int i = 0; i < tyou.menindex; i++)//面の数だけ配列にセット
	{

		if (iro > 1.0f)
		{
			iro = 0;
		}
		vertices[i] = { XMFLOAT3(tyou.tyoutenx[tyou.tyoutenindex[0][i]], tyou.tyouteny[tyou.tyoutenindex[0][i]], tyou.tyoutenz[tyou.tyoutenindex[0][i]]), XMFLOAT4(iro, iro, iro, 1.0f), XMFLOAT3(tyou.thouseni[tyou.tyoutenindex[2][i]], tyou.thousenj[tyou.tyoutenindex[2][i]], tyou.thousenk[tyou.tyoutenindex[2][i]]), XMFLOAT2(tyou.tecschau[tyou.tyoutenindex[1][i]], tyou.tecschav[tyou.tyoutenindex[1][i]]) };
		iro = iro + 0.01f;

		//tyou[ir].thousenk[tyou[ir].tyoutenindex[2][i]]; 面インデックスから配列にアクセス 配列順に並んでる
		//tyou[ir].tyoutenindex[2][i]
	}
	yousosu = tyou.menindex;//配列の要素数をセット

	x = 0;
	y = 0;
	z = 0;

	//頂点バッファ1の定義

	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;					//デフォルト使用法 
	bd.ByteWidth = sizeof(SimpleVertex)* yousosu;	//3頂点 要素数 と 構造体サイズをかけてる
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;		//バッファの種類 頂点バッファ
	bd.CPUAccessFlags = 0;

	//頂点バッファ1のサブリソースの定義

	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertices;					//バッファデータの初期値

	//頂点バッファ1の作成
	hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &g_pVertexBufferc);
	if (FAILED(hr)){ MessageBox(NULL, L"弾の頂点バッファの作成に失敗", L"", MB_OK); }


	//シェーダ設定
	g_pImmediateContext->VSSetShader(g_pVertexShader, NULL, 0);
	g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
	g_pImmediateContext->PSSetShader(g_pPixelShader, NULL, 0);

	
}

OBJC::~OBJC()
{
	delete[] vertices;

	delete[] tyou.tyoutenx;
	delete[] tyou.tyouteny;
	delete[] tyou.tyoutenz;
	delete[] tyou.tecschau;
	delete[] tyou.tecschav;
	delete[] tyou.tecschaw;
	delete[] tyou.thouseni;
	delete[] tyou.thousenj;
	delete[] tyou.thousenk;
	delete[] tyou.tyoutenindex[0];
	delete[] tyou.tyoutenindex[1];
	delete[] tyou.tyoutenindex[2];


	if (g_pVertexBufferc) g_pVertexBufferc->Release();

}

void OBJC::INIT(CHAR *objname)
{
	
		OBJ(objname, tyou);//obj読み込み
		vertices = new SimpleVertex[tyou.menindex];	//面の数だけ配列確保
		float iro = 0;
		for (int i = 0; i < tyou.menindex; i++)//面の数だけ配列にセット
		{

			if (iro > 1.0f)
			{
				iro = 0;
			}
			vertices[i] = { XMFLOAT3(tyou.tyoutenx[tyou.tyoutenindex[0][i]], tyou.tyouteny[tyou.tyoutenindex[0][i]], tyou.tyoutenz[tyou.tyoutenindex[0][i]]), XMFLOAT4(iro, iro, iro, 1.0f), XMFLOAT3(tyou.thouseni[tyou.tyoutenindex[2][i]], tyou.thousenj[tyou.tyoutenindex[2][i]], tyou.thousenk[tyou.tyoutenindex[2][i]]), XMFLOAT2(tyou.tecschau[tyou.tyoutenindex[1][i]], tyou.tecschav[tyou.tyoutenindex[1][i]]) };
			iro = iro + 0.01f;

			//tyou[ir].thousenk[tyou[ir].tyoutenindex[2][i]]; 面インデックスから配列にアクセス 配列順に並んでる
			//tyou[ir].tyoutenindex[2][i]
		}
		yousosu = tyou.menindex;//配列の要素数をセット

		x = 0;
		y = 0;
		z = 0;

		//頂点バッファ1の定義

		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;					//デフォルト使用法 
		bd.ByteWidth = sizeof(SimpleVertex)* yousosu;	//3頂点 要素数 と 構造体サイズをかけてる
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;		//バッファの種類 頂点バッファ
		bd.CPUAccessFlags = 0;

		//頂点バッファ1のサブリソースの定義

		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = vertices;					//バッファデータの初期値

		//頂点バッファ1の作成
		hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &g_pVertexBufferc);
		if (FAILED(hr)){ MessageBox(NULL, L"弾の頂点バッファの作成に失敗", L"", MB_OK); }


		//シェーダ設定
		g_pImmediateContext->VSSetShader(g_pVertexShader, NULL, 0);
		g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
		g_pImmediateContext->PSSetShader(g_pPixelShader, NULL, 0);

	
}

void OBJC::xmove(float xx)
{
	x += xx;
}

void OBJC::ymove(float yy)
{
	y += yy;
}

void OBJC::zmove(float zz)
{
	z += zz;

	if (z>10000)
	{
		ax = 3;
	}
}

void OBJC::scl(float sx, float sy, float sz)
{
	xsc = sx;
	ysc = sy;
	zsc = sz;

}

void OBJC::ammomove()
{
	

	g_pImmediateContext->PSSetShaderResources	//リソースを設定
		(
		0,				//設定する最初のスロット番号
		1,				//設定するシェーダリソースビュー数
		&g_pTextureRV	//設定するシェーダリソースビュー配列
		);
	XMFLOAT4 LightPos = XMFLOAT4(lx, ly, 0.0f, 1.0f);//ライト光源



	zmove(100);

	XMMATRIX mSpin1 = XMMatrixRotationZ(2*D3DX_PI);								//z軸回転
	XMMATRIX mOrbit1 = XMMatrixRotationY(2*D3DX_PI);						//t分 y軸回転 x移動速度	向き
	XMMATRIX mTranslate1 = XMMatrixTranslation(0, -500.0f, z);		//平行移動 xyz
	XMMATRIX mScale1 = XMMatrixScaling(10, 10.01f, 10.0f);				//xyz 分 拡大
	XMMATRIX g_World = mScale1 * mSpin1 * mTranslate1 * mOrbit1;

	//キューブの構造体を設定 定数バッファをシェーダに送る
	ConstantBuffer cb;
	cb.mWorld = XMMatrixTranspose(g_World);
	cb.mView = XMMatrixTranspose(g_View);												//カメラ
	cb.mProjection = XMMatrixTranspose(g_Projection);
	cb.mLight = LightPos;																//ライト

	

	g_pImmediateContext->UpdateSubresource(g_pConstantBuffer, 0, NULL, &cb, 0, 0);
	g_pImmediateContext->Draw(yousosu, 0);

}

void OBJC::taiyoumove()
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

	XMFLOAT4 LightPos = XMFLOAT4(lx, ly, 0.0f, 1.0f);//ライト光源

	XMMATRIX mSpin1 = XMMatrixRotationY(kait * D3DX_PI);								//z軸回転
	XMMATRIX mOrbit1 = XMMatrixRotationX(2 * D3DX_PI);						//t分 y軸回転 x移動速度	向き
	XMMATRIX mTranslate1 = XMMatrixTranslation(lx, ly, 0.0f);		//平行移動 xyz
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


void OBJC::hakom()
{



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

	
	LightPos = XMFLOAT4(lx, ly, 100.0f, 1.0f);//ライト光源

	XMMATRIX mSpin1 = XMMatrixRotationY(2 * D3DX_PI);								//z軸回転
	XMMATRIX mOrbit1 = XMMatrixRotationX(2 * D3DX_PI);						//t分 y軸回転 x移動速度	向き
	XMMATRIX mTranslate1 = XMMatrixTranslation(x, y, z);		//平行移動 xyz
	XMMATRIX mScale1 = XMMatrixScaling(xsc, ysc, 1.0f);				//xyz 分 拡大
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

void OBJC::shetecs()
{

	D3DX11CreateShaderResourceViewFromFile//リソースビュー作成			画像ファイルからテクスチャリソースとビュー作成
		(
		g_pd3dDevice,								//D3D11デバイス
		L"Resource\\texture\\sora.jpg",			//画像ファイル名
		NULL,										//作成するテクステャ
		NULL,										//非同期実行の設定
		&g_pTextureRVc,								//ビューを受け取る変数
		&hr											//戻り値
		);
	if (FAILED(hr)){ MessageBox(NULL, L"D3DX11CreateShaderResourceViewFromFile失敗", 0, MB_OK); }//エラーチェック


}

void OBJC::sheset()
{
	/*
	ID3D11Resource* g_pTexture1 = NULL;//テクスチャリソースの変数
	WCHAR* texturenama = L"Resource\\texture\\Cover1.jpg";

	D3DX11CreateTextureFromFile			//画像ファイルからテクスチャリソースを作成
		(
		g_pd3dDevice,				//リソースを作成するデバイス
		texturenama,				//画像のファイルパス
		NULL,						//デフォルト設定			作成するテクスチャの設定をD3DX11_IMAGE_LOAD_INFO構造体にセットしてアドレスを渡す
		NULL,						//非同期で実行しない
		&g_pTexture1,				//テクスチャを取得する変数
		&hr							//戻り値を取得する変数
		);
	if (FAILED(hr)){ MessageBox(NULL, L"D3DX11CreateTextureFromFile失敗", 0, MB_OK); }//エラーチェック
	*/

	g_pImmediateContext->PSSetShaderResources	//リソースを設定
		(
		0,				//設定する最初のスロット番号
		1,				//設定するシェーダリソースビュー数
		&g_pTextureRVc	//設定するシェーダリソースビュー配列
		);


}

void OBJC::lightios()
{
	ly -= 2.5;
	if (ly<-200)
	{

		int x;
		x = rand() % 1000;
		lx = x - 500;
		ly = 500;
		if (lx>200)
		{
			//lx = -100;
		}
	}



}
