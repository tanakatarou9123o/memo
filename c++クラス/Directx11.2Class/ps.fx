struct pixcelIn
{
    float4 pos : SV_POSITION;
	float4 col : COLOR;
};

float4 ps_main(pixcelIn IN) : SV_Target
{
	pixcelIn OUT;

	//�J���[�����̂܂ܕԂ�
	OUT.col = IN.col;

	return OUT.col;
}