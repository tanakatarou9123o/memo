#include "stdafx.h"
#include "obj�h���N���X.h" 


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

	LightPos = XMFLOAT4(lx, ly, 0.0f, 1.0f);//���C�g����

	XMMATRIX mSpin1 = XMMatrixRotationY(kait * D3DX_PI);								//z����]
	XMMATRIX mOrbit1 = XMMatrixRotationX(2 * D3DX_PI);						//t�� y����] x�ړ����x	����
	XMMATRIX mTranslate1 = XMMatrixTranslation(0, 0.0f, 50);		//���s�ړ� xyz
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
