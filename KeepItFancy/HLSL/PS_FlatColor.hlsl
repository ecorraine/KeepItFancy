Texture2D tex : register(t0);
SamplerState samp : register(s0);

struct PS_IN
{
	float4 pos		: SV_POSITION;
	float2 uv		: TEXCOORD0;
	float4 color	: COLOR0;
	float3 normal	: NORMAL0;
};

float4 main(PS_IN pin) : SV_TARGET
{
	float4 outColor = tex.Sample(samp, pin.uv);
	outColor *= pin.color;

	return outColor;
}