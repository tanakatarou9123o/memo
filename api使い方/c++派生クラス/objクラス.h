#include "stdafx.h" 


//�v���g�^�C�v�錾


#ifndef OBJCL
#define OBJCL


class OBJC
{
public:
	OBJC();
	OBJC(CHAR *objname);
	~OBJC();
	void INIT(char*);	//����������
	void xmove(float);	//x�ړ�
	void ymove(float);	//y�ړ�
	void zmove(float);	//z�ړ�
	void ammomove();	//�`�揈��
	void taiyoumove();	//�`�揈��
	void hakom();		//�`�揈��
	void scl(float sx, float sy, float sz);//�X�P�[���ύX
	void shetecs();		//�摜����V�F�[�_���\�[�X�쐬
	void sheset();		//�V�F�[�_�Ƀr���[�Z�b�g

	static void lightios();//���C�g�ʒu����

private:
protected:
	HRESULT hr = S_OK;

	float x;//���W�ʒu
	float y;//���W�ʒu
	float z;//���W�ʒu
	float xsc;//�X�P�[��
	float ysc;//�X�P�[��
	float zsc;//�X�P�[��

	ID3D11ShaderResourceView* g_pTextureRVc = NULL;						//���\�[�X�r���[

	D3D11_BUFFER_DESC bd;						////���_�o�b�t�@�̒�`�Ɏg��
	D3D11_SUBRESOURCE_DATA InitData;			//�o�b�t�@�f�[�^�̏����l���i�[���� ���_�o�b�t�@�̒�`�Ɏg��
	ID3D11Buffer*           g_pVertexBufferc;//�ݒ肷�钸�_�o�b�t�@�̔z��	���_�o�b�t�@�̒�`�Ɏg��

	int yousosu;							//obj�v�f��
	tyouten tyou;							//obj���
	SimpleVertex *vertices;					//���_�f�[�^�|�C���^
	WORD *indices;							//�C���f�b�N�X�o�b�t�@�|�C���^

	XMFLOAT4 LightPos;//���C�g����
	static float ly, lx, lz;//���C�g�ʒu �X�^�e�B�b�N

};

extern OBJC hako;
extern OBJC billbodo;

#endif