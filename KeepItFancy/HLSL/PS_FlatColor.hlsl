
Texture2D		tex		: register(t0);
SamplerState	samp	: register(s0);

struct PS_IN
{
	float4	pos		: SV_POSITION;
	float4	color	: COLOR0;
	float3	normal	: NORMAL0;
};

float4 main(PS_IN pin) : SV_TARGET
{
	float4 color = pin.color;

	return color;
}