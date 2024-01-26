#include "AS_HullDomainCommon.hlsli"

struct HS_OUT
{
	float4 pos		: SV_POSITION;
	float2 uv		: TEXCOORD0;
	float4 color	: COLOR0;
	float3 normal	: NORMAL0;
};

[domain("tri")]
[partitioning("fractional_even")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(TRI_CONTROLPOINT)]
[patchconstantfunc("PatchTri")]
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