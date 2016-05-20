#include "stdafx.h"
#include "directx11class.h"

int Directx11Class::CreateDeviceAndSwapChain(HWND hWnd,bool windowmode)//�X���b�v�`�F�C���̐ݒ�ƃf�o�C�X,�f�o�C�X�R���e�L�X�g�쐬
{
	D3D_FEATURE_LEVEL g_featurelevelssupported;							//�f�o�C�X�쐬���ɕԂ����@�\���x�����i�[����ϐ�

	ZeroMemory(&sd, sizeof(sd));										//0�ŏ�����
	sd.BufferCount = 1;													//�o�b�N�o�b�t�@��		��������DXGI_MODE_DESC�\����
	sd.BufferDesc.Width = x;											//�o�b�N�o�b�t�@�̕�
	sd.BufferDesc.Height = y;											//�o�b�N�o�b�t�@�̍���
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;					//�f�B�X�v���C�t�H�[�}�b�g
	sd.BufferDesc.RefreshRate.Numerator = 60;							//���t���b�V�����[�g���q
	sd.BufferDesc.RefreshRate.Denominator = 1;							//���t���b�V�����[�g����
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;	//�X�L�������C���`�惂�[�h
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;				//�X�P�[�����O���[�h�@�g��k�����邩���߂�
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;					//�o�b�N�o�b�t�@�̎g�p���@ DXGI_USAGE_RENDER_TARGET_OUTPUT �`��^�[�Q�b�g�Ƃ���
	sd.OutputWindow = hWnd;												//�֘A�t����E�B���h�E�n���h��
	sd.SampleDesc.Count = 1;											//�}���`�T���v���̐�
	sd.SampleDesc.Quality = 0;											//�}���`�T���v���̃N�I���e�B
	sd.Windowed = windowmode;											//��ʃ��[�h TRUE�ŃE�B���h�E���[�h FALSE�Ńt���X�N���[��
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;					//���[�h�����؂�ւ��@ALLOW_MODE_SWITCH �o�b�N�o�b�t�@�̉𑜓x�ɐ؂�ւ��
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;							//��ʂ��X�V��o�b�N�o�b�t�@�̓��e���ǂ����邩

	D3D_FEATURE_LEVEL g_pfeaturelevels[] = { D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0 };	//�@�\���x���̔z��
	UINT g_featurelevels = sizeof(g_pfeaturelevels)/sizeof(g_pfeaturelevels[0]);												//�z��̗v�f��

	hr = D3D11CreateDeviceAndSwapChain		//�X���b�v�`�F�C���̐ݒ�ƃf�o�C�X,�f�o�C�X�R���e�L�X�g�쐬
		(
		NULL,										//�\���Ɏg���f�B�X�v���C�f�o�C�X
		D3D_DRIVER_TYPE_HARDWARE,					//�f�o�C�X�̃h���C�o�^�C�v
		NULL,										//�ʏ��NULL
		0,											//�g�p����API���C���[
		g_pfeaturelevels,							//�@�\���x�����w�肵���z��
		g_featurelevels,							//�@�\���x���̗v�f��
		D3D11_SDK_VERSION,							//�g�p���Ă���SDK�o�[�W����
		&sd,										//�X���b�v�`�F�C���̐ݒ�
		&g_pswapchein,								//�X���b�v�`�F�C���̃C���^�[�t�F�C�X
		&g_pd3dDevice,								//�f�o�C�X�̃C���^�[�t�F�C�X
		&g_featurelevelssupported,					//�@�\���x�����i�[����ϐ�
		&g_pd3dDeviceconteki						//�f�o�C�X�R���e�L�X�g�̃C���^�[�t�F�C�X
		);
	if (FAILED(hr)){ MessageBox(NULL, TEXT("�f�o�C�X�쐬���s"), NULL, MB_OK); }

	return 1;
}

int Directx11Class::CreateAndSetRenderTargetView()//�o�b�N�o�b�t�@ �^�[�Q�b�g�r���[�ݒ� �[�x/�X�e���V���r���[�쐬,�ݒ� �r���[�|�[�g�ݒ�
{

	ID3D11Texture2D *pbackbuffer;					//�o�b�N�o�b�t�@���擾����ϐ�
	hr = g_pswapchein->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pbackbuffer);//�o�b�N�o�b�t�@�̔ԍ�,,�o�b�t�@���󂯎��ϐ� �X���b�v�`�F�C������o�b�N�o�b�t�@���擾����
	if (FAILED(hr)){ MessageBox(NULL, TEXT("�X���b�v�`�F�C������o�b�N�o�b�t�@�擾���s"), NULL, MB_OK); }

	hr = g_pd3dDevice->CreateRenderTargetView(pbackbuffer, NULL, &g_prendertargetview);//�`��^�[�Q�b�g�r���[�Ƀo�b�N�o�b�t�@��ݒ�
	if (FAILED(hr)){ MessageBox(NULL, TEXT("�o�b�N�o�b�t�@�^�[�Q�b�g�r���[�쐬���s"), NULL, MB_OK); }
	
	D3D11_TEXTURE2D_DESC descdepth;//�[�x/�X�e���V���e�N�X�`��
	descdepth.Width = x;
	descdepth.Height = y;
	descdepth.MipLevels = 1;						//�~�b�v�}�b�v���x����
	descdepth.ArraySize = 1;						//�z��T�C�Y
	descdepth.Format = DXGI_FORMAT_D32_FLOAT;
	descdepth.SampleDesc.Count = 1;
	descdepth.SampleDesc.Quality = 0;
	descdepth.Usage = D3D11_USAGE_DEFAULT;
	descdepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;//�[�x/�X�e���V���Ƃ��Ďg�p
	descdepth.CPUAccessFlags = 0;
	descdepth.MiscFlags = 0;
	hr = g_pd3dDevice->CreateTexture2D(&descdepth, NULL, &g_pdepthstencil);//�[�x/�X�e���V���e�N�X�`���쐬
	if (FAILED(hr)){ MessageBox(NULL, TEXT("�X�e���V���e�N�X�`���쐬���s"), NULL, MB_OK); }

	D3D11_DEPTH_STENCIL_VIEW_DESC descdsv;//�[�x/�X�e���V���r���[
	descdsv.Format = descdepth.Format;
	descdsv.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descdsv.Flags = 0;
	descdsv.Texture2D.MipSlice = 0;
	hr = g_pd3dDevice->CreateDepthStencilView(g_pdepthstencil, &descdsv, &g_pdepthstencilview);//�[�x/�X�e���V���r���[�쐬
	if (FAILED(hr)){ MessageBox(NULL, TEXT("�[�x/�X�e���V���r���[�쐬���s"), NULL, MB_OK); }

	g_pd3dDeviceconteki->OMSetRenderTargets(1, &g_prendertargetview, g_pdepthstencilview);//�`��^�[�Q�b�g�r���[�Ɛ[�x/�X�e���V���r���[�ݒ�

	D3D11_VIEWPORT g_viewport[1];//�r���[�|�[�g
	g_viewport[0].TopLeftX = 0.0f;
	g_viewport[0].TopLeftY = 0.0f;
	g_viewport[0].Width = (float)x;//�r���[�|�[�g�̕�
	g_viewport[0].Height = (float)y;//�r���[�|�[�g�̍���
	g_viewport[0].MinDepth = 0.0f;//�[�x�̍ŏ��l
	g_viewport[0].MaxDepth = 1.0f;//�[�x�̍ő�l

	g_pd3dDeviceconteki->RSSetViewports(1, g_viewport);//�ݒ肷��r���[�|�[�g�̐�, �r���[�|�[�g�ݒ�

	return 1;
}

int Directx11Class::CreateRasterizer()
{
	D3D11_RASTERIZER_DESC RSDesc;
	RSDesc.FillMode = D3D11_FILL_SOLID;		//���ʂɕ`��	//D3D11_FILL_WIREFRAME ���C���t���[���ŕ`��
	RSDesc.CullMode = D3D11_CULL_NONE;		//D3D11_CULL_NONE ���ʂ�`��  D3D11_CULL_FRONT �\��`�悵�Ȃ� D3D11_CULL_BACK ����`�悵�Ȃ� 
	RSDesc.FrontCounterClockwise = FALSE;	//���v��肪�\��
	RSDesc.DepthBias = 0;					//�[�x�o�C�A�X
	RSDesc.DepthBiasClamp = 0;				//
	RSDesc.SlopeScaledDepthBias = 0;		//
	RSDesc.DepthClipEnable = FALSE;			//�[�x�N���b�s���O�Ȃ�
	RSDesc.ScissorEnable = FALSE;			//�V�U�[�Z�`�Ȃ�
	RSDesc.MultisampleEnable = FALSE;		//�}���`�T���v�����O�Ȃ�
	RSDesc.AntialiasedLineEnable = FALSE;	//���C���A���`�G�C���A�V���O�Ȃ�
	hr = g_pd3dDevice->CreateRasterizerState(&RSDesc, &g_pRasterizerState);//���X�^���C�U�쐬�֐�
	if (FAILED(hr)){ return hr; }//�G���[�`�F�b�N

	g_pd3dDeviceconteki->RSSetState(g_pRasterizerState);	//RS�Ƀ��X�^���C�U�X�e�[�g�I�u�W�F�N�g��ݒ�

	return 1;
}

int Directx11Class::CreateShader()//�V�F�[�_�쐬
{
	g_pd3dDevice->CreateVertexShader(&g_vs_main, sizeof(g_vs_main), NULL, &hpVertexShader);//�s�N�Z���V�F�[�_�[�쐬
	g_pd3dDeviceconteki->VSSetShader(hpVertexShader, NULL, 0);//���_�V�F�[�_�[���R���e�L�X�g�ɐݒ�

	g_pd3dDevice->CreatePixelShader(&g_ps_main, sizeof(g_ps_main), NULL, &hpPixelShader);//���_�V�F�[�_�[�쐬
	g_pd3dDeviceconteki->PSSetShader(hpPixelShader, NULL, 0);//�s�N�Z���V�F�[�_�[���R���e�L�X�g�ɐݒ�

	return 1;
}

int Directx11Class::CreateBuffer()//�o�b�t�@�쐬
{
	const int tyouten = 6;

	Vertex3D hVectorData[tyouten] = //���_�f�[�^(�O�p�|���S��1��)
	{
		{ { +0.0f, +0.8f, +0.5f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
		{ { +0.5f, +0.5f, +0.5f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
		{ { -0.5f, +0.5f, +0.5f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
		{ { +0.5f, -0.5f, +0.5f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
		{ { -0.5f, -0.5f, +0.5f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
		{ { +0.0f, -0.8f, +0.5f }, { 1.0f, 1.0f, 1.0f, 1.0f } }
	};

	//���_���C�A�E�g
	//�T�Ԗڂ̃p�����[�^�͐擪����̃o�C�g���Ȃ̂ŁCCOLOR�ɂ�POSITION��float�^4�o�C�g�~3���L�q
	D3D11_INPUT_ELEMENT_DESC hInElementDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	//���_�o�b�t�@�쐬
	D3D11_BUFFER_DESC hBufferDesc;
	hBufferDesc.ByteWidth = sizeof(Vertex3D) * tyouten;//Vertex3D*3 �\���̂̃T�C�Y������z��
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

	g_pd3dDevice->CreateBuffer(&hBufferDesc, &hSubResourceData, &hpBuffer);//�o�b�t�@�쐬

	//���̒��_�o�b�t�@���R���e�L�X�g�ɐݒ�
	UINT hStrides = sizeof(Vertex3D);
	UINT hOffsets = 0;
	g_pd3dDeviceconteki->IASetVertexBuffers(0, 1, &hpBuffer, &hStrides, &hOffsets);

	//�v���~�e�B�u(�|���S���̌`��)���R���e�L�X�g�ɐݒ�
	g_pd3dDeviceconteki->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);//D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST

	//���_���C�A�E�g�쐬
	ID3D11InputLayout* hpInputLayout = NULL;
	g_pd3dDevice->CreateInputLayout(hInElementDesc, ARRAYSIZE(hInElementDesc), &g_vs_main, sizeof(g_vs_main), &hpInputLayout);

	//���_���C�A�E�g���R���e�L�X�g�ɐݒ�
	g_pd3dDeviceconteki->IASetInputLayout(hpInputLayout);

	return 1;
}

int Directx11Class::CreateConstantBuffer()//�萔�o�b�t�@�쐬
{
	//���_�o�b�t�@�쐬
	D3D11_BUFFER_DESC hBufferDesc;
	hBufferDesc.ByteWidth = sizeof(ConstantBuffer);//�\���̂̃T�C�Y
	hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	hBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	hBufferDesc.CPUAccessFlags = 0;
	hBufferDesc.MiscFlags = 0;
	hBufferDesc.StructureByteStride = sizeof(float);

	g_pd3dDevice->CreateBuffer(&hBufferDesc, NULL, &g_pConstantBuffer);

	ConstantBuffer hConstantBuffer;//�萔�o�b�t�@
	hConstantBuffer.g_World1 = XMMatrixTranspose(g_World1);//���[���h�ϊ��s��
	hConstantBuffer.g_View = XMMatrixTranspose(g_View);//�r���[�ϊ��s��
	hConstantBuffer.g_Projection = XMMatrixTranspose(g_Projection);//�����ˉe�ϊ��s��
	g_pd3dDeviceconteki->UpdateSubresource(g_pConstantBuffer, 0, NULL, &hConstantBuffer, 0, 0);//�萔�o�b�t�@�ɒ萔��ݒ�

	//�R���e�L�X�g�ɐݒ�
	g_pd3dDeviceconteki->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);

	return 1;
}

int Directx11Class::Render()
{
	const int tyouten = 6;

	float clearcolor[4] = { 0.0f, 0.125f, 0.9f, 1.0f };//�N���A�F�ݒ�
	g_pd3dDeviceconteki->ClearRenderTargetView(g_prendertargetview, clearcolor);//�`��^�[�Q�b�g��ݒ肵���F�ŃN���A
	g_pd3dDeviceconteki->ClearDepthStencilView(g_pdepthstencilview, D3D11_CLEAR_DEPTH, 1.0f, 0);//�X�e���V��

	//�`��
	g_pd3dDeviceconteki->Draw(tyouten, 0);

	//�o�b�N�o�b�t�@���X���b�v
	hr = g_pswapchein->Present(0, 0);//��ʍX�V
	return 1;
}

int Directx11Class::WorldTransformation()//���[���h�ϊ�
{
	//���[���h�ϊ��p�s��𐶐�				//���[���h�ϊ��s��
	g_World1 = XMMatrixIdentity();	//������
	//g_World2 = XMMatrixIdentity();	//������

	return 1;
}

int Directx11Class::ViewConversion(float x, float y, float z)//�r���[�ϊ�
{
	XMVECTOR Eye = XMVectorSet(x, y, z, 0.0f);//�J�����̈ʒu x,y,z
	XMVECTOR At = XMVectorSet(x, y+1.0f, z-5, 0.0f);//�œ_�̈ʒu	�J�����̎�U��(x +�ŉE),�J�����̎�U��(y +�ŏ�)	xy
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	g_View = XMMatrixLookAtLH(Eye, At, Up);//�r���[�s����쐬

	return 1;
}

int Directx11Class::ProjectiveTransformation()//�ˉe�ϊ�
{
	//�����ˉe�ϊ��s��
	//����Ɋ�Â��ē����ˉe�s��(������W�n)���쐬 ��P���� ����p�x
	g_Projection = XMMatrixPerspectiveFovLH(XM_PIDIV2, x / (FLOAT)y, 0.01f, 10000.0f);

	return 1;
}


Directx11Class::Directx11Class()//
{
	if (!XMVerifyCPUSupport())
	{
		MessageBox(nullptr, L"CPU���Ή����Ă܂���", L"XMVerifyCPUSupport", MB_OK);
		exit(0);
	}

	x = GetSystemMetrics(SM_CXSCREEN);//�f�B�X�v���C�𑜓xx�擾
	y = GetSystemMetrics(SM_CYSCREEN);//�f�B�X�v���C�𑜓xy�擾
}


Directx11Class::~Directx11Class()
{
	g_pd3dDeviceconteki->ClearState();	//�f�o�C�X�X�e�[�g�̃N���A
	SAFE_RELEASE(g_prendertargetview);	//�C���^�[�t�F�C�X�J��
	SAFE_RELEASE(g_pswapchein);
	SAFE_RELEASE(g_pd3dDeviceconteki);
	SAFE_RELEASE(g_pd3dDevice);
	SAFE_RELEASE(g_pdepthstencilview);

}
