#include "BUF_WVPMatrix.hlsli"

#define MAX_CP_COUNT (4)

cbuffer Tessellation : register(b1)
{
	float g_TessellationFactor : packoffset(c0);
}

struct VS_OUT
{
	float4 pos		: SV_POSITION;
	float2 uv		: TEXCOORD0;
	float4 color	: COLOR0;
	float3 normal	: NORMAL0;
};

struct HS_CONTROL_POINT_OUTPUT
{
	float4 pos		: SV_POSITION;
	float2 uv		: TEXCOORD0;
	float4 color	: COLOR0;
	float3 normal	: NORMAL0;
};

struct HS_CONSTANT_DATA_OUTPUT
{
	float edges[4] 	: SV_TessFactor;
	float inside[2]	: SV_InsideTessFactor;
};

HS_CONSTANT_DATA_OUTPUT HSPatchConstants(
	InputPatch<VS_OUT, MAX_CP_COUNT> patch,
	uint patchId : SV_PrimitiveID)
{
	HS_CONSTANT_DATA_OUTPUT output = (HS_CONSTANT_DATA_OUTPUT)0;

	// Set tessellation factors based on application-defined logic
	output.edges[0]	= 4;
	output.edges[1]	= 4;
	output.edges[2]	= 4;
	output.edges[3]	= 4;
	output.inside[0] = 4;
	output.inside[1] = 4;

	return output;
}

[domain("quad")]
[partitioning("fractional_even")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(MAX_CP_COUNT)]
[patchconstantfunc("HSPatchConstants")]
// [maxtessfactor(8.0)]
HS_CONTROL_POINT_OUTPUT main(
	InputPatch<VS_OUT, MAX_CP_COUNT> patch,
	uint pointId : SV_OutputControlPointID,
	uint patchId : SV_PrimitiveID
	)
{
	HS_CONTROL_POINT_OUTPUT hout = (HS_CONTROL_POINT_OUTPUT)0;

	hout.pos		= patch[pointId].pos;

	hout.uv			= patch[pointId].uv;
	hout.color		= patch[pointId].color;
	hout.normal		= patch[pointId].normal;

	return hout;
}