#include "stdafx.h"
#include "obj�N���X.h"

//��`����

OBJC::OBJC()
{

}

OBJC::OBJC(CHAR *objname)
{
	OBJ(objname, tyou);//obj�ǂݍ���
	vertices = new SimpleVertex[tyou.menindex];	//�ʂ̐������z��m��
	float iro = 0;
	for (int i = 0; i < tyou.menindex; i++)//�ʂ̐������z��ɃZ�b�g
	{

		if (iro > 1.0f)
		{
			iro = 0;
		}
		vertices[i] = { XMFLOAT3(tyou.tyoutenx[tyou.tyoutenindex[0][i]], tyou.tyouteny[tyou.tyoutenindex[0][i]], tyou.tyoutenz[tyou.tyoutenindex[0][i]]), XMFLOAT4(iro, iro, iro, 1.0f), XMFLOAT3(tyou.thouseni[tyou.tyoutenindex[2][i]], tyou.thousenj[tyou.tyoutenindex[2][i]], tyou.thousenk[tyou.tyoutenindex[2][i]]), XMFLOAT2(tyou.tecschau[tyou.tyoutenindex[1][i]], tyou.tecschav[tyou.tyoutenindex[1][i]]) };
		iro = iro + 0.01f;

		//tyou[ir].thousenk[tyou[ir].tyoutenindex[2][i]]; �ʃC���f�b�N�X����z��ɃA�N�Z�X �z�񏇂ɕ���ł�
		//tyou[ir].tyoutenindex[2][i]
	}
	yousosu = tyou.menindex;//�z��̗v�f�����Z�b�g

	x = 0;
	y = 0;
	z = 0;

	//���_�o�b�t�@1�̒�`

	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;					//�f�t�H���g�g�p�@ 
	bd.ByteWidth = sizeof(SimpleVertex)* yousosu;	//3���_ �v�f�� �� �\���̃T�C�Y�������Ă�
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;		//�o�b�t�@�̎�� ���_�o�b�t�@
	bd.CPUAccessFlags = 0;

	//���_�o�b�t�@1�̃T�u���\�[�X�̒�`

	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertices;					//�o�b�t�@�f�[�^�̏����l

	//���_�o�b�t�@1�̍쐬
	hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &g_pVertexBufferc);
	if (FAILED(hr)){ MessageBox(NULL, L"�e�̒��_�o�b�t�@�̍쐬�Ɏ��s", L"", MB_OK); }


	//�V�F�[�_�ݒ�
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
	
		OBJ(objname, tyou);//obj�ǂݍ���
		vertices = new SimpleVertex[tyou.menindex];	//�ʂ̐������z��m��
		float iro = 0;
		for (int i = 0; i < tyou.menindex; i++)//�ʂ̐������z��ɃZ�b�g
		{

			if (iro > 1.0f)
			{
				iro = 0;
			}
			vertices[i] = { XMFLOAT3(tyou.tyoutenx[tyou.tyoutenindex[0][i]], tyou.tyouteny[tyou.tyoutenindex[0][i]], tyou.tyoutenz[tyou.tyoutenindex[0][i]]), XMFLOAT4(iro, iro, iro, 1.0f), XMFLOAT3(tyou.thouseni[tyou.tyoutenindex[2][i]], tyou.thousenj[tyou.tyoutenindex[2][i]], tyou.thousenk[tyou.tyoutenindex[2][i]]), XMFLOAT2(tyou.tecschau[tyou.tyoutenindex[1][i]], tyou.tecschav[tyou.tyoutenindex[1][i]]) };
			iro = iro + 0.01f;

			//tyou[ir].thousenk[tyou[ir].tyoutenindex[2][i]]; �ʃC���f�b�N�X����z��ɃA�N�Z�X �z�񏇂ɕ���ł�
			//tyou[ir].tyoutenindex[2][i]
		}
		yousosu = tyou.menindex;//�z��̗v�f�����Z�b�g

		x = 0;
		y = 0;
		z = 0;

		//���_�o�b�t�@1�̒�`

		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;					//�f�t�H���g�g�p�@ 
		bd.ByteWidth = sizeof(SimpleVertex)* yousosu;	//3���_ �v�f�� �� �\���̃T�C�Y�������Ă�
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;		//�o�b�t�@�̎�� ���_�o�b�t�@
		bd.CPUAccessFlags = 0;

		//���_�o�b�t�@1�̃T�u���\�[�X�̒�`

		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = vertices;					//�o�b�t�@�f�[�^�̏����l

		//���_�o�b�t�@1�̍쐬
		hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &g_pVertexBufferc);
		if (FAILED(hr)){ MessageBox(NULL, L"�e�̒��_�o�b�t�@�̍쐬�Ɏ��s", L"", MB_OK); }


		//�V�F�[�_�ݒ�
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
	

	g_pImmediateContext->PSSetShaderResources	//���\�[�X��ݒ�
		(
		0,				//�ݒ肷��ŏ��̃X���b�g�ԍ�
		1,				//�ݒ肷��V�F�[�_���\�[�X�r���[��
		&g_pTextureRV	//�ݒ肷��V�F�[�_���\�[�X�r���[�z��
		);
	XMFLOAT4 LightPos = XMFLOAT4(lx, ly, 0.0f, 1.0f);//���C�g����



	zmove(100);

	XMMATRIX mSpin1 = XMMatrixRotationZ(2*D3DX_PI);								//z����]
	XMMATRIX mOrbit1 = XMMatrixRotationY(2*D3DX_PI);						//t�� y����] x�ړ����x	����
	XMMATRIX mTranslate1 = XMMatrixTranslation(0, -500.0f, z);		//���s�ړ� xyz
	XMMATRIX mScale1 = XMMatrixScaling(10, 10.01f, 10.0f);				//xyz �� �g��
	XMMATRIX g_World = mScale1 * mSpin1 * mTranslate1 * mOrbit1;

	//�L���[�u�̍\���̂�ݒ� �萔�o�b�t�@���V�F�[�_�ɑ���
	ConstantBuffer cb;
	cb.mWorld = XMMatrixTranspose(g_World);
	cb.mView = XMMatrixTranspose(g_View);												//�J����
	cb.mProjection = XMMatrixTranspose(g_Projection);
	cb.mLight = LightPos;																//���C�g

	

	g_pImmediateContext->UpdateSubresource(g_pConstantBuffer, 0, NULL, &cb, 0, 0);
	g_pImmediateContext->Draw(yousosu, 0);

}

void OBJC::taiyoumove()
{
	//lightios();

	g_pImmediateContext->PSSetShaderResources	//���\�[�X��ݒ�
		(
		0,				//�ݒ肷��ŏ��̃X���b�g�ԍ�
		1,				//�ݒ肷��V�F�[�_���\�[�X�r���[��
		&g_pTextureRV	//�ݒ肷��V�F�[�_���\�[�X�r���[�z��
		);


	// Set vertex buffer
	UINT stride = sizeof(SimpleVertex); //���_�o�b�t�@�Ɋ܂܂�钸�_�f�[�^�̃T�C�Y
	UINT offset = 0;					//���_�o�b�t�@1�̃I�t�Z�b�g

	g_pImmediateContext->IASetVertexBuffers//���̓A�Z���u���ɒ��_�o�b�t�@��ݒ�
		(
		0,								//�ݒ肷��ŏ��̓��̓X���b�g�ԍ�
		1, 								//�ݒ肷�钸�_�o�b�t�@�̐�
		&g_pVertexBufferc,				//�ݒ肷�钸�_�o�b�t�@�̔z��
		&stride, 						//�e���_�o�b�t�@�̗v�f�T�C�Y
		&offset							//�e���_�o�b�t�@�̃I�t�Z�b�g
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

	XMFLOAT4 LightPos = XMFLOAT4(lx, ly, 0.0f, 1.0f);//���C�g����

	XMMATRIX mSpin1 = XMMatrixRotationY(kait * D3DX_PI);								//z����]
	XMMATRIX mOrbit1 = XMMatrixRotationX(2 * D3DX_PI);						//t�� y����] x�ړ����x	����
	XMMATRIX mTranslate1 = XMMatrixTranslation(lx, ly, 0.0f);		//���s�ړ� xyz
	XMMATRIX mScale1 = XMMatrixScaling(1.0f, 1.0f, 1.0f);				//xyz �� �g��

	XMMATRIX g_World = mScale1 * mSpin1 * mTranslate1 * mOrbit1;

	//�L���[�u�̍\���̂�ݒ� �萔�o�b�t�@���V�F�[�_�ɑ���
	ConstantBuffer cb;
	cb.mWorld = XMMatrixTranspose(g_World);
	cb.mView = XMMatrixTranspose(g_View);												//�J����
	cb.mProjection = XMMatrixTranspose(g_Projection);
	cb.mLight = LightPos;																//���C�g

	g_pImmediateContext->UpdateSubresource(g_pConstantBuffer, 0, NULL, &cb, 0, 0);//�萔�o�b�t�@���X�V
	g_pImmediateContext->Draw(yousosu, 0);

}


void OBJC::hakom()
{



	// Set vertex buffer
	UINT stride = sizeof(SimpleVertex); //���_�o�b�t�@�Ɋ܂܂�钸�_�f�[�^�̃T�C�Y
	UINT offset = 0;					//���_�o�b�t�@1�̃I�t�Z�b�g

	g_pImmediateContext->IASetVertexBuffers//���̓A�Z���u���ɒ��_�o�b�t�@��ݒ�
		(
		0,								//�ݒ肷��ŏ��̓��̓X���b�g�ԍ�
		1, 								//�ݒ肷�钸�_�o�b�t�@�̐�
		&g_pVertexBufferc,				//�ݒ肷�钸�_�o�b�t�@�̔z��
		&stride, 						//�e���_�o�b�t�@�̗v�f�T�C�Y
		&offset							//�e���_�o�b�t�@�̃I�t�Z�b�g
		);

	
	LightPos = XMFLOAT4(lx, ly, 100.0f, 1.0f);//���C�g����

	XMMATRIX mSpin1 = XMMatrixRotationY(2 * D3DX_PI);								//z����]
	XMMATRIX mOrbit1 = XMMatrixRotationX(2 * D3DX_PI);						//t�� y����] x�ړ����x	����
	XMMATRIX mTranslate1 = XMMatrixTranslation(x, y, z);		//���s�ړ� xyz
	XMMATRIX mScale1 = XMMatrixScaling(xsc, ysc, 1.0f);				//xyz �� �g��
	XMMATRIX g_World = mScale1 * mSpin1 * mTranslate1 * mOrbit1;

	//�L���[�u�̍\���̂�ݒ� �萔�o�b�t�@���V�F�[�_�ɑ���
	ConstantBuffer cb;
	cb.mWorld = XMMatrixTranspose(g_World);
	cb.mView = XMMatrixTranspose(g_View);												//�J����
	cb.mProjection = XMMatrixTranspose(g_Projection);
	cb.mLight = LightPos;																//���C�g

	g_pImmediateContext->UpdateSubresource(g_pConstantBuffer, 0, NULL, &cb, 0, 0);//�萔�o�b�t�@���X�V
	g_pImmediateContext->Draw(yousosu, 0);


}

void OBJC::shetecs()
{

	D3DX11CreateShaderResourceViewFromFile//���\�[�X�r���[�쐬			�摜�t�@�C������e�N�X�`�����\�[�X�ƃr���[�쐬
		(
		g_pd3dDevice,								//D3D11�f�o�C�X
		L"Resource\\texture\\sora.jpg",			//�摜�t�@�C����
		NULL,										//�쐬����e�N�X�e��
		NULL,										//�񓯊����s�̐ݒ�
		&g_pTextureRVc,								//�r���[���󂯎��ϐ�
		&hr											//�߂�l
		);
	if (FAILED(hr)){ MessageBox(NULL, L"D3DX11CreateShaderResourceViewFromFile���s", 0, MB_OK); }//�G���[�`�F�b�N


}

void OBJC::sheset()
{
	/*
	ID3D11Resource* g_pTexture1 = NULL;//�e�N�X�`�����\�[�X�̕ϐ�
	WCHAR* texturenama = L"Resource\\texture\\Cover1.jpg";

	D3DX11CreateTextureFromFile			//�摜�t�@�C������e�N�X�`�����\�[�X���쐬
		(
		g_pd3dDevice,				//���\�[�X���쐬����f�o�C�X
		texturenama,				//�摜�̃t�@�C���p�X
		NULL,						//�f�t�H���g�ݒ�			�쐬����e�N�X�`���̐ݒ��D3DX11_IMAGE_LOAD_INFO�\���̂ɃZ�b�g���ăA�h���X��n��
		NULL,						//�񓯊��Ŏ��s���Ȃ�
		&g_pTexture1,				//�e�N�X�`�����擾����ϐ�
		&hr							//�߂�l���擾����ϐ�
		);
	if (FAILED(hr)){ MessageBox(NULL, L"D3DX11CreateTextureFromFile���s", 0, MB_OK); }//�G���[�`�F�b�N
	*/

	g_pImmediateContext->PSSetShaderResources	//���\�[�X��ݒ�
		(
		0,				//�ݒ肷��ŏ��̃X���b�g�ԍ�
		1,				//�ݒ肷��V�F�[�_���\�[�X�r���[��
		&g_pTextureRVc	//�ݒ肷��V�F�[�_���\�[�X�r���[�z��
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
