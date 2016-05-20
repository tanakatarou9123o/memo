//入力用
struct vertexIn
{
	float4 pos : POSITION0;//ローカル座標 位置
	float4 col : COLOR0;
};

//出力用
struct vertexOut
{
	float4 pos : SV_POSITION;
	float4 col : COLOR;
};

//変換用行列
cbuffer ConstantBuffer : register( b0 )
{
	matrix World;		//ワールド変換行列
	matrix View;		//ビュー変換行列
	matrix Projection;	//透視射影変換行列
}

vertexOut vs_main(vertexIn IN)
{
	vertexOut OUT;//リターンする構造体

	OUT.pos = mul(IN.pos, World);		//ローカル座標とワールド変換をかける
	OUT.pos = mul(OUT.pos, View);		//ワールド変換とビュー変換をかける
	OUT.pos = mul(OUT.pos, Projection);	//ビュー変換と透視射影変換をかける
	OUT.col = IN.col;

	return OUT;
}