#include "AS_HullDomainCommon.hlsli"

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
	float2	padding;
};

QUAD_OUTPUT PatchQuad(
	InputPatch<VS_OUT, QUAD_CONTROLPOINT> patch,
	uint patchId : SV_PrimitiveID)
{
	QUAD_OUTPUT output = (QUAD_OUTPUT)0;

	output.edgeTess[0]	= g_TessFactor;
	output.edgeTess[1]	= g_TessFactor;
	output.edgeTess[2]	= g_TessFactor;
	output.edgeTess[3]	= g_TessFactor;

	output.insideTess[0] = g_TessFactor;
	output.insideTess[1] = g_TessFactor;

	return output;
}

[domain("quad")]
[partitioning("fractional_even")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(QUAD_CONTROLPOINT)]
[patchconstantfunc("PatchQuad")]
[maxtessfactor(8.0)]
HS_OUT main(
	InputPatch<VS_OUT, QUAD_CONTROLPOINT> patch,
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