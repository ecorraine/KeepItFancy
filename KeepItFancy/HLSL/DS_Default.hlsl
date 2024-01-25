#include "BUF_WVPMatrix.hlsli"

#define MAX_CP_COUNT (4)

struct DS_OUT
{
	float4 pos		: SV_POSITION;
	float2 uv		: TEXCOORD0;
	float4 color	: COLOR0;
	float3 normal	: NORMAL0;
	float3 worldPos : POSITION0;
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

[domain("quad")]
DS_OUT main(
	HS_CONSTANT_DATA_OUTPUT input,
	float2 domain : SV_DomainLocation,
	const OutputPatch<HS_CONTROL_POINT_OUTPUT, MAX_CP_COUNT> patch
	)
{
	DS_OUT dout = (DS_OUT)0;

	// Barycentric interpolation
    /*
    // Triangle
	float4 position = (patch[0].pos.xyz * domain.x
                    + patch[1].pos.xyz * domain.y
                    + patch[2].pos.xyz * domain.z,
                    1);
    */
    // Quad
	float4 position = (patch[0].pos.xyz * (1 - domain.x) * (1 - domain.y)
                    + patch[1].pos.xyz * domain.x * (1 - domain.y)
                    + patch[2].pos.xyz * (1 - domain.x) * domain.y
                    + patch[3].pos.xyz * domain.x * domain.y,
                    1);
	dout.pos = mul(position, world);
    dout.worldPos = dout.pos.xyz;
    dout.pos = mul(dout.pos, view);
	dout.pos = mul(dout.pos, proj);

    dout.uv = patch[0].uv * (1 - domain.x) * (1 - domain.y)
            + patch[1].uv * domain.x * (1 - domain.y)
            + patch[2].uv * (1 - domain.x) * domain.y
            + patch[3].uv * domain.x * domain.y;

    dout.color = patch[0].color * (1 - domain.x) * (1 - domain.y)
                + patch[1].color * domain.x * (1 - domain.y)
                + patch[2].color * (1 - domain.x) * domain.y
                + patch[3].color * domain.x * domain.y;

	dout.normal = patch[0].normal * (1 - domain.x) * (1 - domain.y)
                + patch[1].normal * domain.x * (1 - domain.y)
                + patch[2].normal * (1 - domain.x) * domain.y
                + patch[3].normal * domain.x * domain.y;

	return dout;
}