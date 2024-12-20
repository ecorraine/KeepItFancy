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
	float4 outColor = baseColor * pin.color;
	float4 sampledColor = texBase.Sample(g_Sampler, pin.uv);
	if (bool(g_useTexture))
		outColor *= sampledColor;

	float3 normal = normalize(pin.normal);
	float3 light = normalize(lightDir.xyz);
	light = -light;
	float shadow = saturate(dot(normal, light));

	float3 viewDir = normalize(cameraPos.xyz - pin.worldPos);
	float3 halfDir = normalize(viewDir + light);

	float NdotH = saturate(dot(normal, halfDir));
	float3 specular = pow(NdotH, 16);
	
	outColor.rgb *= (shadow * lightDiffuse.rgb) + lightAmbient.rgb + specular;

	return outColor;
}