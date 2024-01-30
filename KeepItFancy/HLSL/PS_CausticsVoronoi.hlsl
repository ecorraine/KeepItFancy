#include "AF_Noise.hlsli"

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

cbuffer PSCommonData : register(b0)
{
	float4	baseColor;
	float	g_time;
	float	g_useTexture;
	float2	g_UVTiling;
};

cbuffer LightData : register(b1)
{
	float4 cameraPos;
	float4 lightDir;
	float4 lightDiffuse;
	float4 lightAmbient;
};

float4 main(PS_IN pin) : SV_TARGET
{
	float time = g_time;

	float4 outColor = baseColor * pin.color;
	float4 sampledColor = texBase.Sample(g_Sampler, pin.uv);
	if (bool(g_useTexture))
		outColor *= sampledColor;

	float3 normal = normalize(pin.normal.xyz);
	float3 light = normalize(lightDir.xyz);
	light = -light;

	float diffuse = saturate(dot(normal, light));
	outColor.rgb *= (diffuse * lightDiffuse) + lightAmbient;

	float3 viewDir = normalize(cameraPos.xyz - pin.worldPos.xyz);
	
	float shadow = saturate(dot(-lightDir.xyz, pin.normal));
	float reflection = saturate(dot(viewDir, normal));

	float2 uvCoord = pin.uv * 3;
	float caustics = CellNoiseTilable(uvCoord, time * 0.3f);
	caustics = caustics * shadow * reflection;
	outColor.rgb = outColor.rgb * (1 - caustics) + caustics * 1.5f;

	return outColor;
}