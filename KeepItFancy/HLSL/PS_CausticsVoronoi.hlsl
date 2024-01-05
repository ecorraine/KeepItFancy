#include "F_Common.hlsli"
#include "F_Noise.hlsli"
#include "BUF_Light.hlsli"

cbuffer Data : register(b1)
{
	float4 g_time;
};

Texture2D		tex		: register(t0);
SamplerState	samp	: register(s0);

struct PS_IN
{
	float4 pos		: SV_POSITION;
	float2 uv		: TEXCOORD0;
	float4 color	: COLOR0;
	float3 normal	: NORMAL0;
	float3 worldPos : POSITION0;
};

float4 main(PS_IN pin) : SV_TARGET
{
	float time = g_time.x;
	
	float4 outColor = pin.color;

	float3 normal = normalize(pin.normal.xyz);
	float3 light = normalize(lightDir.xyz);
	light = -light;

	float diffuse = saturate(dot(normal, light));
	outColor.rgb *= (diffuse * lightDiffuse.rgb) + lightAmbient.rgb;

	float3 viewDir = normalize(cameraPos.xyz - pin.worldPos.xyz);
	
	float shadow = saturate(dot(-lightDir.xyz, pin.normal));
	float reflection = saturate(dot(viewDir, normal));

	float2 uvCoord = pin.uv * 8;
	float caustics = CellNoiseTilable(uvCoord, time * 0.1f);
	outColor *= caustics * 0.5f;
	outColor *= reflection;
	
	return outColor;
}