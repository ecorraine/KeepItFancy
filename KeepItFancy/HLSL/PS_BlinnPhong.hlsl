#include "BUF_Light.hlsli"

Texture2D		tex			: register(t0);
SamplerState	g_Sampler	: register(s0);

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
	float4 outColor = pin.color;

	float3 normal = normalize(pin.normal.xyz);
	float3 light = normalize(lightDir.xyz);
	light = -light;
	float diffuse = saturate(dot(normal, light));

	float3 viewDir = normalize(cameraPos.xyz - pin.worldPos.xyz);
	float3 halfDir = normalize(viewDir + light);

	float NdotH = saturate(dot(normal, halfDir));
	float3 specular = pow(NdotH, 16);
	
	outColor.rgb *= (diffuse * lightDiffuse.rgb) + lightAmbient.rgb + specular;

	return outColor;
}