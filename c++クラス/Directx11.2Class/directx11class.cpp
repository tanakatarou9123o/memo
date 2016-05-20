#include "stdafx.h"
#include "directx11class.h"

int Directx11Class::CreateDeviceAndSwapChain(HWND hWnd,bool windowmode)//スワップチェインの設定とデバイス,デバイスコンテキスト作成
{
	D3D_FEATURE_LEVEL g_featurelevelssupported;							//デバイス作成時に返される機能レベルを格納する変数

	ZeroMemory(&sd, sizeof(sd));										//0で初期化
	sd.BufferCount = 1;													//バックバッファ数		ここからDXGI_MODE_DESC構造体
	sd.BufferDesc.Width = x;											//バックバッファの幅
	sd.BufferDesc.Height = y;											//バックバッファの高さ
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;					//ディスプレイフォーマット
	sd.BufferDesc.RefreshRate.Numerator = 60;							//リフレッシュレート分子
	sd.BufferDesc.RefreshRate.Denominator = 1;							//リフレッシュレート分母
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;	//スキャンライン描画モード
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;				//スケーリングモード　拡大縮小するか決める
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;					//バックバッファの使用方法 DXGI_USAGE_RENDER_TARGET_OUTPUT 描画ターゲットとして
	sd.OutputWindow = hWnd;												//関連付けるウィンドウハンドル
	sd.SampleDesc.Count = 1;											//マルチサンプルの数
	sd.SampleDesc.Quality = 0;											//マルチサンプルのクオリティ
	sd.Windowed = windowmode;											//画面モード TRUEでウィンドウモード FALSEでフルスクリーン
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;					//モード自動切り替え　ALLOW_MODE_SWITCH バックバッファの解像度に切り替わる
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;							//画面を更新後バックバッファの内容をどうするか

	D3D_FEATURE_LEVEL g_pfeaturelevels[] = { D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0 };	//機能レベルの配列
	UINT g_featurelevels = sizeof(g_pfeaturelevels)/sizeof(g_pfeaturelevels[0]);												//配列の要素数

	hr = D3D11CreateDeviceAndSwapChain		//スワップチェインの設定とデバイス,デバイスコンテキスト作成
		(
		NULL,										//表示に使うディスプレイデバイス
		D3D_DRIVER_TYPE_HARDWARE,					//デバイスのドライバタイプ
		NULL,										//通常はNULL
		0,											//使用するAPIレイヤー
		g_pfeaturelevels,							//機能レベルを指定した配列
		g_featurelevels,							//機能レベルの要素数
		D3D11_SDK_VERSION,							//使用しているSDKバージョン
		&sd,										//スワップチェインの設定
		&g_pswapchein,								//スワップチェインのインターフェイス
		&g_pd3dDevice,								//デバイスのインターフェイス
		&g_featurelevelssupported,					//機能レベルを格納する変数
		&g_pd3dDeviceconteki						//デバイスコンテキストのインターフェイス
		);
	if (FAILED(hr)){ MessageBox(NULL, TEXT("デバイス作成失敗"), NULL, MB_OK); }

	return 1;
}

int Directx11Class::CreateAndSetRenderTargetView()//バックバッファ ターゲットビュー設定 深度/ステンシルビュー作成,設定 ビューポート設定
{

	ID3D11Texture2D *pbackbuffer;					//バックバッファを取得する変数
	hr = g_pswapchein->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pbackbuffer);//バックバッファの番号,,バッファを受け取る変数 スワップチェインからバックバッファを取得する
	if (FAILED(hr)){ MessageBox(NULL, TEXT("スワップチェインからバックバッファ取得失敗"), NULL, MB_OK); }

	hr = g_pd3dDevice->CreateRenderTargetView(pbackbuffer, NULL, &g_prendertargetview);//描画ターゲットビューにバックバッファを設定
	if (FAILED(hr)){ MessageBox(NULL, TEXT("バックバッファターゲットビュー作成失敗"), NULL, MB_OK); }
	
	D3D11_TEXTURE2D_DESC descdepth;//深度/ステンシルテクスチャ
	descdepth.Width = x;
	descdepth.Height = y;
	descdepth.MipLevels = 1;						//ミップマップレベル数
	descdepth.ArraySize = 1;						//配列サイズ
	descdepth.Format = DXGI_FORMAT_D32_FLOAT;
	descdepth.SampleDesc.Count = 1;
	descdepth.SampleDesc.Quality = 0;
	descdepth.Usage = D3D11_USAGE_DEFAULT;
	descdepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;//深度/ステンシルとして使用
	descdepth.CPUAccessFlags = 0;
	descdepth.MiscFlags = 0;
	hr = g_pd3dDevice->CreateTexture2D(&descdepth, NULL, &g_pdepthstencil);//深度/ステンシルテクスチャ作成
	if (FAILED(hr)){ MessageBox(NULL, TEXT("ステンシルテクスチャ作成失敗"), NULL, MB_OK); }

	D3D11_DEPTH_STENCIL_VIEW_DESC descdsv;//深度/ステンシルビュー
	descdsv.Format = descdepth.Format;
	descdsv.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descdsv.Flags = 0;
	descdsv.Texture2D.MipSlice = 0;
	hr = g_pd3dDevice->CreateDepthStencilView(g_pdepthstencil, &descdsv, &g_pdepthstencilview);//深度/ステンシルビュー作成
	if (FAILED(hr)){ MessageBox(NULL, TEXT("深度/ステンシルビュー作成失敗"), NULL, MB_OK); }

	g_pd3dDeviceconteki->OMSetRenderTargets(1, &g_prendertargetview, g_pdepthstencilview);//描画ターゲットビューと深度/ステンシルビュー設定

	D3D11_VIEWPORT g_viewport[1];//ビューポート
	g_viewport[0].TopLeftX = 0.0f;
	g_viewport[0].TopLeftY = 0.0f;
	g_viewport[0].Width = (float)x;//ビューポートの幅
	g_viewport[0].Height = (float)y;//ビューポートの高さ
	g_viewport[0].MinDepth = 0.0f;//深度の最小値
	g_viewport[0].MaxDepth = 1.0f;//深度の最大値

	g_pd3dDeviceconteki->RSSetViewports(1, g_viewport);//設定するビューポートの数, ビューポート設定

	return 1;
}

int Directx11Class::CreateRasterizer()
{
	D3D11_RASTERIZER_DESC RSDesc;
	RSDesc.FillMode = D3D11_FILL_SOLID;		//普通に描画	//D3D11_FILL_WIREFRAME ワイヤフレームで描画
	RSDesc.CullMode = D3D11_CULL_NONE;		//D3D11_CULL_NONE 両面を描画  D3D11_CULL_FRONT 表を描画しない D3D11_CULL_BACK 裏を描画しない 
	RSDesc.FrontCounterClockwise = FALSE;	//時計回りが表面
	RSDesc.DepthBias = 0;					//深度バイアス
	RSDesc.DepthBiasClamp = 0;				//
	RSDesc.SlopeScaledDepthBias = 0;		//
	RSDesc.DepthClipEnable = FALSE;			//深度クリッピングなし
	RSDesc.ScissorEnable = FALSE;			//シザー短形なし
	RSDesc.MultisampleEnable = FALSE;		//マルチサンプリングなし
	RSDesc.AntialiasedLineEnable = FALSE;	//ラインアンチエイリアシングなし
	hr = g_pd3dDevice->CreateRasterizerState(&RSDesc, &g_pRasterizerState);//ラスタライザ作成関数
	if (FAILED(hr)){ return hr; }//エラーチェック

	g_pd3dDeviceconteki->RSSetState(g_pRasterizerState);	//RSにラスタライザステートオブジェクトを設定

	return 1;
}

int Directx11Class::CreateShader()//シェーダ作成
{
	g_pd3dDevice->CreateVertexShader(&g_vs_main, sizeof(g_vs_main), NULL, &hpVertexShader);//ピクセルシェーダー作成
	g_pd3dDeviceconteki->VSSetShader(hpVertexShader, NULL, 0);//頂点シェーダーをコンテキストに設定

	g_pd3dDevice->CreatePixelShader(&g_ps_main, sizeof(g_ps_main), NULL, &hpPixelShader);//頂点シェーダー作成
	g_pd3dDeviceconteki->PSSetShader(hpPixelShader, NULL, 0);//ピクセルシェーダーをコンテキストに設定

	return 1;
}

int Directx11Class::CreateBuffer()//バッファ作成
{
	const int tyouten = 6;

	Vertex3D hVectorData[tyouten] = //頂点データ(三角ポリゴン1枚)
	{
		{ { +0.0f, +0.8f, +0.5f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
		{ { +0.5f, +0.5f, +0.5f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
		{ { -0.5f, +0.5f, +0.5f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
		{ { +0.5f, -0.5f, +0.5f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
		{ { -0.5f, -0.5f, +0.5f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
		{ { +0.0f, -0.8f, +0.5f }, { 1.0f, 1.0f, 1.0f, 1.0f } }
	};

	//頂点レイアウト
	//５番目のパラメータは先頭からのバイト数なので，COLORにはPOSITIONのfloat型4バイト×3を記述
	D3D11_INPUT_ELEMENT_DESC hInElementDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	//頂点バッファ作成
	D3D11_BUFFER_DESC hBufferDesc;
	hBufferDesc.ByteWidth = sizeof(Vertex3D) * tyouten;//Vertex3D*3 構造体のサイズかける配列数
	hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	hBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	hBufferDesc.CPUAccessFlags = 0;
	hBufferDesc.MiscFlags = 0;
	hBufferDesc.StructureByteStride = sizeof(float);

	D3D11_SUBRESOURCE_DATA hSubResourceData;
	hSubResourceData.pSysMem = hVectorData;
	hSubResourceData.SysMemPitch = 0;
	hSubResourceData.SysMemSlicePitch = 0;

	ID3D11Buffer* hpBuffer;

	g_pd3dDevice->CreateBuffer(&hBufferDesc, &hSubResourceData, &hpBuffer);//バッファ作成

	//その頂点バッファをコンテキストに設定
	UINT hStrides = sizeof(Vertex3D);
	UINT hOffsets = 0;
	g_pd3dDeviceconteki->IASetVertexBuffers(0, 1, &hpBuffer, &hStrides, &hOffsets);

	//プリミティブ(ポリゴンの形状)をコンテキストに設定
	g_pd3dDeviceconteki->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);//D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST

	//頂点レイアウト作成
	ID3D11InputLayout* hpInputLayout = NULL;
	g_pd3dDevice->CreateInputLayout(hInElementDesc, ARRAYSIZE(hInElementDesc), &g_vs_main, sizeof(g_vs_main), &hpInputLayout);

	//頂点レイアウトをコンテキストに設定
	g_pd3dDeviceconteki->IASetInputLayout(hpInputLayout);

	return 1;
}

int Directx11Class::CreateConstantBuffer()//定数バッファ作成
{
	//頂点バッファ作成
	D3D11_BUFFER_DESC hBufferDesc;
	hBufferDesc.ByteWidth = sizeof(ConstantBuffer);//構造体のサイズ
	hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	hBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	hBufferDesc.CPUAccessFlags = 0;
	hBufferDesc.MiscFlags = 0;
	hBufferDesc.StructureByteStride = sizeof(float);

	g_pd3dDevice->CreateBuffer(&hBufferDesc, NULL, &g_pConstantBuffer);

	ConstantBuffer hConstantBuffer;//定数バッファ
	hConstantBuffer.g_World1 = XMMatrixTranspose(g_World1);//ワールド変換行列
	hConstantBuffer.g_View = XMMatrixTranspose(g_View);//ビュー変換行列
	hConstantBuffer.g_Projection = XMMatrixTranspose(g_Projection);//透視射影変換行列
	g_pd3dDeviceconteki->UpdateSubresource(g_pConstantBuffer, 0, NULL, &hConstantBuffer, 0, 0);//定数バッファに定数を設定

	//コンテキストに設定
	g_pd3dDeviceconteki->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);

	return 1;
}

int Directx11Class::Render()
{
	const int tyouten = 6;

	float clearcolor[4] = { 0.0f, 0.125f, 0.9f, 1.0f };//クリア色設定
	g_pd3dDeviceconteki->ClearRenderTargetView(g_prendertargetview, clearcolor);//描画ターゲットを設定した色でクリア
	g_pd3dDeviceconteki->ClearDepthStencilView(g_pdepthstencilview, D3D11_CLEAR_DEPTH, 1.0f, 0);//ステンシル

	//描画
	g_pd3dDeviceconteki->Draw(tyouten, 0);

	//バックバッファをスワップ
	hr = g_pswapchein->Present(0, 0);//画面更新
	return 1;
}

int Directx11Class::WorldTransformation()//ワールド変換
{
	//ワールド変換用行列を生成				//ワールド変換行列
	g_World1 = XMMatrixIdentity();	//初期化
	//g_World2 = XMMatrixIdentity();	//初期化

	return 1;
}

int Directx11Class::ViewConversion(float x, float y, float z)//ビュー変換
{
	XMVECTOR Eye = XMVectorSet(x, y, z, 0.0f);//カメラの位置 x,y,z
	XMVECTOR At = XMVectorSet(x, y+1.0f, z-5, 0.0f);//焦点の位置	カメラの首振り(x +で右),カメラの首振り(y +で上)	xy
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	g_View = XMMatrixLookAtLH(Eye, At, Up);//ビュー行列を作成

	return 1;
}

int Directx11Class::ProjectiveTransformation()//射影変換
{
	//透視射影変換行列
	//視野に基づいて透視射影行列(左手座標系)を作成 第１引数 視野角度
	g_Projection = XMMatrixPerspectiveFovLH(XM_PIDIV2, x / (FLOAT)y, 0.01f, 10000.0f);

	return 1;
}


Directx11Class::Directx11Class()//
{
	if (!XMVerifyCPUSupport())
	{
		MessageBox(nullptr, L"CPUが対応してません", L"XMVerifyCPUSupport", MB_OK);
		exit(0);
	}

	x = GetSystemMetrics(SM_CXSCREEN);//ディスプレイ解像度x取得
	y = GetSystemMetrics(SM_CYSCREEN);//ディスプレイ解像度y取得
}


Directx11Class::~Directx11Class()
{
	g_pd3dDeviceconteki->ClearState();	//デバイスステートのクリア
	SAFE_RELEASE(g_prendertargetview);	//インターフェイス開放
	SAFE_RELEASE(g_pswapchein);
	SAFE_RELEASE(g_pd3dDeviceconteki);
	SAFE_RELEASE(g_pd3dDevice);
	SAFE_RELEASE(g_pdepthstencilview);

}
