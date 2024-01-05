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

const float3 causticsColor = float3(0.2f, 0.5f, 0.8f);
const float3 reflectionColor = float3(0.8f, 0.9f, 1.0f);
const float3 shadowColor = float3(0.1f, 0.1f, 0.1f);

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
	
	float shadowFactor = saturate(dot(-lightDir.xyz, pin.normal));

	// Calculate caustics using a mathematical function
	float causticsIntensity = abs(cos(pin.worldPos.x * 10.0f + time) * sin(pin.worldPos.z * 10.0f + time));
	// Simulate reflections based on the view direction
	float reflectionIntensity = saturate(dot(viewDir, normal));
	
	// Blend colors for a more water-like appearance
	float3 waterColor = lerp(causticsColor, reflectionColor, reflectionIntensity);
	waterColor = lerp(waterColor, shadowColor, shadowFactor);
	
	// Combine the caustics color with the original color
	outColor.rgb = outColor.rgb * (1.0f - causticsIntensity) + (waterColor.rgb * causticsIntensity);

	return outColor;
}