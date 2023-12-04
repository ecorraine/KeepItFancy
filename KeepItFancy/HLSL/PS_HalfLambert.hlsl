#include "BUF_Light.hlsli"

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

	//float4 view = viewDir - pin.pos;
	//view = normalize(view);

	float4 light = normalize(-lightDir);
	float4 normal = normalize(float4(pin.normal.xyz, 0.0f));
	float diffuse = saturate(dot(normal, light));

	color.rgb *= diffuse * lightDiffuse.rgb + lightAmbient.rgb;

	return color;
}