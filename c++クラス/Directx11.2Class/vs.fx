//���͗p
struct vertexIn
{
	float4 pos : POSITION0;//���[�J�����W �ʒu
	float4 col : COLOR0;
};

//�o�͗p
struct vertexOut
{
	float4 pos : SV_POSITION;
	float4 col : COLOR;
};

//�ϊ��p�s��
cbuffer ConstantBuffer : register( b0 )
{
	matrix World;		//���[���h�ϊ��s��
	matrix View;		//�r���[�ϊ��s��
	matrix Projection;	//�����ˉe�ϊ��s��
}

vertexOut vs_main(vertexIn IN)
{
	vertexOut OUT;//���^�[������\����

	OUT.pos = mul(IN.pos, World);		//���[�J�����W�ƃ��[���h�ϊ���������
	OUT.pos = mul(OUT.pos, View);		//���[���h�ϊ��ƃr���[�ϊ���������
	OUT.pos = mul(OUT.pos, Projection);	//�r���[�ϊ��Ɠ����ˉe�ϊ���������
	OUT.col = IN.col;

	return OUT;
}