#include "BUF_Light.hlsli"

cbuffer Data : register(b1)
{
	float g_time;
	float bloomThreshold;
	float2 dummy;
};

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

	// Caustics effect
	float caustics = 0.5 + 0.5 * sin(g_time * 5.0); // Adjust frequency and amplitude
	color.rgb *= caustics;

	float bloom = 1.5f;
	// Bloom effect
	float bloomFactor = max(0, color.r - bloom);
	color.rgb += bloomFactor;

	return color;
}