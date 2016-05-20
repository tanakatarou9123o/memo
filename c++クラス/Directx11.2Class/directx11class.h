#include "stdafx.h"
#pragma comment(lib, "d3d11.lib")									//ライブラリ ロード
//#pragma comment(lib, "d3dcompiler.lib")								//コンパイラライブラリ

#include <d3d11_2.h>
#include <DirectXMath.h>
#include "vs.h"
#include "ps.h"

using namespace DirectX;

#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

//ポリゴン頂点構造体
struct Vertex3D 
{
	float pos[3];	//x-y-z
	float col[4];	//r-g-b-a
};

struct ConstantBuffer//定数バッファ
{
	XMMATRIX g_World1;		//ワールド変換行列
	XMMATRIX g_View;			//ビュー変換行列
	XMMATRIX g_Projection;	//透視射影変換行列
};

class Directx11Class
{
public:
	Directx11Class();
	~Directx11Class();
	int CreateDeviceAndSwapChain(HWND,bool=1);//スワップチェインの設定とデバイス,デバイスコンテキスト作成
	int CreateAndSetRenderTargetView();//バックバッファ ターゲットビュー設定 深度/ステンシルビュー作成,設定 ビューポート設定
	int CreateRasterizer();//ラスタライザ作成
	int CreateShader();//シェーダ作成
	int CreateBuffer();//バッファ作成
	int CreateConstantBuffer();//定数バッファ作成
	int Render();
	int WorldTransformation();//ワールド変換
	int ViewConversion(float x=0,float y=0,float z=5.0f);//ビュー変換
	int ProjectiveTransformation();//射影変換

	
private:
	DXGI_SWAP_CHAIN_DESC sd;											//DXGI_SWAP_CHAIN_DESC構造体作成
	ID3D11Device* g_pd3dDevice = NULL;									//デバイス変数
	ID3D11DeviceContext* g_pd3dDeviceconteki = NULL;					//デバイスコンテキスト変数
	IDXGISwapChain* g_pswapchein = NULL;								//スワップチェイン変数
	ID3D11RenderTargetView* g_prendertargetview = NULL;					//描画ターゲットビュー変数
	ID3D11DepthStencilView * g_pdepthstencilview = NULL;				//深度/ステンシルビュー
	
	ID3D11Texture2D* g_pdepthstencil;//ステンシルテクスチャ
	ID3D11RasterizerState* g_pRasterizerState;//ラスタライザステート
	
	ID3D11PixelShader* hpPixelShader;//ピクセルシェーダー
	ID3D11VertexShader* hpVertexShader;//頂点シェーダー

	HRESULT hr;

	int x;
	int y;

	XMMATRIX g_World1;		//ワールド変換行列
	XMMATRIX g_View;			//ビュー変換行列
	XMMATRIX g_Projection;	//透視射影変換行列
	ID3D11Buffer*           g_pConstantBuffer = NULL;//定数バッファ


};

