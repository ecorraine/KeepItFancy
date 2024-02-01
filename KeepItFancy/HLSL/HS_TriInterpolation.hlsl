#include "AS_HullDomainCommon.hlsli"

Texture2D		texHeight	: register(t0);
SamplerState	g_Sampler	: register(s0);

struct VS_OUT
{
	float4 pos		: SV_POSITION;
	float2 uv		: TEXCOORD0;
	float4 color	: COLOR0;
	float3 normal	: NORMAL0;
	float3 worldPos : POSITION0;
};

struct HS_OUT
{
	float4 pos		: SV_POSITION;
	float2 uv		: TEXCOORD0;
	float4 color	: COLOR0;
	float3 normal	: NORMAL0;
};

cbuffer TessellationData : register(b0)
{
	float	g_useTesselate;
	float	g_TessFactor;
	float	g_NoiseScale;
	float	padding;
};

TRI_OUTPUT PatchTri(
	InputPatch<VS_OUT, TRI_CONTROLPOINT> patch,
	uint patchId : SV_PrimitiveID)
{
	TRI_OUTPUT output = (TRI_OUTPUT)0;

	float noiseFactor = texHeight.SampleLevel(g_Sampler, patch[0].uv, 0).r;

	output.edgeTess[0]	= g_TessFactor * (1.0 + noiseFactor * g_NoiseScale);
	output.edgeTess[1]	= g_TessFactor * (1.0 + noiseFactor * g_NoiseScale);
	output.edgeTess[2]	= g_TessFactor * (1.0 + noiseFactor * g_NoiseScale);

	output.insideTess = g_TessFactor;

	return output;
}

[domain("tri")]
[partitioning("fractional_even")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(TRI_CONTROLPOINT)]
[patchconstantfunc("PatchTri")]
[maxtessfactor(8.0)]
HS_OUT main(
	InputPatch<VS_OUT, TRI_CONTROLPOINT> patch,
	uint pointId : SV_OutputControlPointID,
	uint patchId : SV_PrimitiveID)
{
	HS_OUT hout = (HS_OUT)0;

	hout.pos		= patch[pointId].pos;
	hout.uv			= patch[pointId].uv;
	hout.color		= patch[pointId].color;
	hout.normal		= patch[pointId].normal;

	return hout;
}