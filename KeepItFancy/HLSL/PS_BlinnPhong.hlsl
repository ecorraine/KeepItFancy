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

cbuffer CommonData : register(b0)
{
	float4	newColor;
	float	g_time;
	float	g_isUsingTexture;
	float	g_Tiling;
	float	padding;
};

cbuffer Light : register(b1)
{
	float4 cameraPos;
	float4 lightDir;
	float4 lightDiffuse;
	float4 lightAmbient;
};

float4 main(PS_IN pin) : SV_TARGET
{
	float4 outColor = newColor * pin.color;
	float4 sampledColor = texBase.Sample(g_Sampler, pin.uv);
	if (bool(g_isUsingTexture))
		outColor *= sampledColor;

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