// ライト情報を保持する定数バッファ
cbuffer Light : register(b0)
{
	float4 viewDir;
	float4 lightDir;
	float4 lightDiffuse;
	float4 lightAmbient;
};
