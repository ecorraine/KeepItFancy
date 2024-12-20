#include "AF_Noise.hlsli"

Texture2D		texBase		: register(t0);
Texture2D		texHeight	: register(t1);
Texture2D		texNormal	: register(t2);
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
	float4 outColor = baseColor * pin.color;
	float4 sampledColor = texBase.Sample(g_Sampler, pin.uv);
	if (bool(g_useTexture))
		outColor *= sampledColor;

	float3 normal = normalize(pin.normal);
	float3 light = normalize(lightDir.xyz);
	light = -light;
	float shadow = saturate(dot(normal, light));

	outColor.rgb *= (shadow * lightDiffuse.rgb) + lightAmbient.rgb;

	float3 viewDir = normalize(cameraPos.xyz - pin.worldPos);
	float3 halfDir = normalize(viewDir + light);
	float reflection = saturate(dot(halfDir, normal));
	// float reverse = saturate(dot(-halfDir, normal));

	float time = g_time;

	float4 heightMap = texHeight.Sample(g_Sampler, pin.uv);
	float4 normalMap1 = texNormal.Sample(g_Sampler, pin.uv - time * 0.001f);
	float4 normalMap2 = texNormal.Sample(g_Sampler, pin.uv - time * 0.003f);
	float4 normalFinal = normalize(lerp(normalMap1, normalMap2, 0.5f));
	// normalFinal = normalFinal;

	outColor.rgb += normalFinal.r;
	
	// tile
	float2 uvCoord = float2(pin.uv.x * g_UVTiling.x, pin.uv.y * g_UVTiling.y);
	// generate voronoi cell noise
	float caustics = CellNoiseTilable(uvCoord, time * 0.3f);

	outColor.rgb = outColor.rgb + caustics * reflection;

	return outColor;
}