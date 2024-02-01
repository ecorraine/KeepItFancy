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

[domain("quad")]
DS_OUT main(
	QUAD_OUTPUT input,
	float2 domain : SV_DomainLocation,
	const OutputPatch<HS_OUT, QUAD_CONTROLPOINT> patch
	)
{
	DS_OUT dout = (DS_OUT)0;

	// Barycentric Quad interpolation
	float2 uv1 = lerp(patch[0].uv, patch[1].uv, domain.x);
	float2 uv2 = lerp(patch[2].uv, patch[3].uv, domain.x);
	dout.uv = lerp(uv1, uv2, domain.y);

	float4 color1 = lerp(patch[0].color, patch[1].color, domain.x);
	float4 color2 = lerp(patch[2].color, patch[3].color, domain.x);
	dout.color = lerp(color1, color2, domain.y);

	float3 pos1 = lerp(patch[1].pos.xyz, patch[2].pos.xyz, domain.x);
	float3 pos2 = lerp(patch[0].pos.xyz, patch[3].pos.xyz, domain.x);
	dout.pos = float4(lerp(pos1, pos2, domain.y), 1.0f);

	dout.pos = mul(dout.pos, world);
	dout.worldPos = dout.pos.xyz;
	dout.pos = mul(dout.pos, view);
	dout.pos = mul(dout.pos, proj);

	float3 normal1 = lerp(patch[0].normal, patch[1].normal, domain.x);
	float3 normal2 = lerp(patch[2].normal, patch[3].normal, domain.x);
	dout.normal = lerp(normal1, normal2, domain.y);
	dout.normal = normalize(mul(dout.normal, (float3x3) world));

	return dout;
}