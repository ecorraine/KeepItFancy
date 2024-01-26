#include "AS_HullDomainCommon.hlsli"
#include "BUF_WVPMatrix.hlsli"

struct DS_OUT
{
	float4 pos		: SV_POSITION;
	float2 uv		: TEXCOORD0;
	float4 color	: COLOR0;
	float3 normal	: NORMAL0;
	float3 worldPos	: POSITION0;
};

struct HS_OUT
{
	float4 pos		: SV_POSITION;
	float2 uv		: TEXCOORD0;
	float4 color	: COLOR0;
	float3 normal	: NORMAL0;
};

[domain("tri")]
DS_OUT main(
	TRI_OUTPUT input,
	float3 domain : SV_DomainLocation,
	const OutputPatch<HS_OUT, TRI_CONTROLPOINT> patch
	)
{
	DS_OUT dout = (DS_OUT)0;

	// Barycentric Triangle interpolation
	float3 position = patch[0].pos.xyz * domain.x
					+ patch[1].pos.xyz * domain.y
					+ patch[2].pos.xyz * domain.z;

	dout.pos = mul(float4(position, 1.0f), world);
	dout.worldPos = dout.pos.xyz;
	dout.pos = mul(dout.pos, view);
	dout.pos = mul(dout.pos, proj);

	dout.uv = patch[0].uv * domain.x
			+ patch[1].uv * domain.y
			+ patch[2].uv * domain.z;

	dout.color = patch[0].color * domain.x
				+ patch[1].color * domain.y
				+ patch[2].color * domain.z;

	dout.normal = patch[0].normal * domain.x
				+ patch[1].normal * domain.y
				+ patch[2].normal * domain.z;

	return dout;
}