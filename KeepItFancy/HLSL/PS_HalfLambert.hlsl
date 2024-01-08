#include "BUF_Light.hlsli"

Texture2D		texBase		: register(t0);
SamplerState	g_Sampler	: register(s0);

struct PS_IN
{
	float4 pos		: SV_POSITION;
	float2 uv		: TEXCOORD0;
	float4 color	: COLOR0;
	float3 normal	: NORMAL0;
};

float4 main(PS_IN pin) : SV_TARGET
{
	float4 outColor = pin.color;
	float4 tex = texBase.Sample(g_Sampler, pin.uv);
	outColor.rgb *= tex.rgb;

	float3 normal = normalize(pin.normal.xyz);
	float3 light = normalize(lightDir.xyz);
	light = -light;
	float diffuse = saturate(dot(normal, light));

	outColor.rgb *= (diffuse * lightDiffuse.rgb) + lightAmbient.rgb;

	return outColor;
}