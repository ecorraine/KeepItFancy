#include "F_Common.hlsli"
#include "F_Noise.hlsli"
#include "BUF_Light.hlsli"

Texture2D		texBase		: register(t0);
SamplerState	g_Sampler	: register(s0);

struct PS_IN
{
	float4 pos		: SV_POSITION;
	float2 uv		: TEXCOORD0;
	float4 color	: COLOR0;
	float3 normal	: NORMAL0;
	float3 worldPos : POSITION0;
};

cbuffer Data : register(b1)
{
	float4 g_time;
};

float4 main(PS_IN pin) : SV_TARGET
{
	float time = g_time.x;

	float4 outColor = pin.color;
	float4 tex = texBase.Sample(g_Sampler, pin.uv);
	outColor.rgb *= tex.rgb;

	float3 normal = normalize(pin.normal.xyz);
	float3 light = normalize(lightDir.xyz);
	light = -light;

	float diffuse = saturate(dot(normal, light));
	outColor.rgb *= (diffuse * lightDiffuse.rgb) + lightAmbient.rgb;

	float3 viewDir = normalize(cameraPos.xyz - pin.worldPos.xyz);
	
	float shadow = saturate(dot(-lightDir.xyz, pin.normal));
	float reflection = saturate(dot(viewDir, normal));

	float2 uvCoord = pin.uv * 10;
	float caustics = CellNoiseTilable(uvCoord, time * 0.5f);
	outColor *= caustics * 1.5f;
	outColor *= shadow * reflection;
	
	return outColor;
}