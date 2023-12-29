#include "F_Noise.hlsli"
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
	float4 pos		: SV_POSITION;
	float2 uv		: TEXCOORD0;
	float4 color	: COLOR0;
	float3 normal	: NORMAL0;
	float3 worldPos : POSITION0;
};

float3 CausticsEffect(float2 uv, float time)
{
	float causticsScale = 0.5f;
	float causticsSpeed = 1.0f;
	float voronoi = VoronoiNoise(uv * causticsScale + time * causticsSpeed, time);

	float shadowIntensity = 0.2f;
	float shadow = 1.0f - saturate(voronoi * shadowIntensity);
	
	// Adjust the intensity of the caustics effect
	float intensity = 0.2f;
	
	// Blend the caustics color with the original color
	float3 causticsColor = float3(0.5294f, 0.8078f, 0.9804f);
	return lerp(float3(1.0f, 1.0f, 1.0f), causticsColor, voronoi * intensity) * shadow;
}

float4 main(PS_IN pin) : SV_TARGET
{
	float4 outColor = pin.color;

	float3 normal = normalize(pin.normal.xyz);
	float3 light = normalize(lightDir.xyz);
	light = -light;

	float diffuse = saturate(dot(normal, light));
	outColor.rgb *= (diffuse * lightDiffuse.rgb) + lightAmbient.rgb;

	float3 viewDir = normalize(cameraPos.xyz - pin.worldPos.xyz);

	//float3 caustics = CausticsEffect(pin.uv, g_time);
	
	float shadowFactor = saturate(dot(-lightDir.xyz, pin.normal));

	// Calculate caustics using a mathematical function
	float causticsIntensity = abs(cos(pin.worldPos.x * 1.0f) * sin(pin.worldPos.z * 1.0f));
	// Simulate reflections based on the view direction
	float reflectionIntensity = saturate(dot(viewDir, normal));
	
	// Blend colors for a more water-like appearance
	float3 causticsColor = float3(0.2f, 0.5f, 0.8f);
	float3 reflectionColor = float3(0.8f, 0.9f, 1.0f);
	float3 shadowColor = float3(0.1f, 0.1f, 0.1f);
	// Combine the caustics color and reflection color
	float3 waterColor = lerp(causticsColor, reflectionColor, reflectionIntensity);
	waterColor = lerp(waterColor, shadowColor, shadowFactor);
	
	// Combine the caustics color with the original color
	outColor.rgb = outColor.rgb * (1.0f - causticsIntensity) + (waterColor.rgb * causticsIntensity);

	return outColor;
}