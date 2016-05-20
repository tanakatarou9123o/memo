#include "stdafx.h"
#pragma comment(lib, "d3d11.lib")									//���C�u���� ���[�h
//#pragma comment(lib, "d3dcompiler.lib")								//�R���p�C�����C�u����

#include <d3d11_2.h>
#include <DirectXMath.h>
#include "vs.h"
#include "ps.h"

using namespace DirectX;

#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

//�|���S�����_�\����
struct Vertex3D 
{
	float pos[3];	//x-y-z
	float col[4];	//r-g-b-a
};

struct ConstantBuffer//�萔�o�b�t�@
{
	XMMATRIX g_World1;		//���[���h�ϊ��s��
	XMMATRIX g_View;			//�r���[�ϊ��s��
	XMMATRIX g_Projection;	//�����ˉe�ϊ��s��
};

class Directx11Class
{
public:
	Directx11Class();
	~Directx11Class();
	int CreateDeviceAndSwapChain(HWND,bool=1);//�X���b�v�`�F�C���̐ݒ�ƃf�o�C�X,�f�o�C�X�R���e�L�X�g�쐬
	int CreateAndSetRenderTargetView();//�o�b�N�o�b�t�@ �^�[�Q�b�g�r���[�ݒ� �[�x/�X�e���V���r���[�쐬,�ݒ� �r���[�|�[�g�ݒ�
	int CreateRasterizer();//���X�^���C�U�쐬
	int CreateShader();//�V�F�[�_�쐬
	int CreateBuffer();//�o�b�t�@�쐬
	int CreateConstantBuffer();//�萔�o�b�t�@�쐬
	int Render();
	int WorldTransformation();//���[���h�ϊ�
	int ViewConversion(float x=0,float y=0,float z=5.0f);//�r���[�ϊ�
	int ProjectiveTransformation();//�ˉe�ϊ�

	
private:
	DXGI_SWAP_CHAIN_DESC sd;											//DXGI_SWAP_CHAIN_DESC�\���̍쐬
	ID3D11Device* g_pd3dDevice = NULL;									//�f�o�C�X�ϐ�
	ID3D11DeviceContext* g_pd3dDeviceconteki = NULL;					//�f�o�C�X�R���e�L�X�g�ϐ�
	IDXGISwapChain* g_pswapchein = NULL;								//�X���b�v�`�F�C���ϐ�
	ID3D11RenderTargetView* g_prendertargetview = NULL;					//�`��^�[�Q�b�g�r���[�ϐ�
	ID3D11DepthStencilView * g_pdepthstencilview = NULL;				//�[�x/�X�e���V���r���[
	
	ID3D11Texture2D* g_pdepthstencil;//�X�e���V���e�N�X�`��
	ID3D11RasterizerState* g_pRasterizerState;//���X�^���C�U�X�e�[�g
	
	ID3D11PixelShader* hpPixelShader;//�s�N�Z���V�F�[�_�[
	ID3D11VertexShader* hpVertexShader;//���_�V�F�[�_�[

	HRESULT hr;

	int x;
	int y;

	XMMATRIX g_World1;		//���[���h�ϊ��s��
	XMMATRIX g_View;			//�r���[�ϊ��s��
	XMMATRIX g_Projection;	//�����ˉe�ϊ��s��
	ID3D11Buffer*           g_pConstantBuffer = NULL;//�萔�o�b�t�@


};

