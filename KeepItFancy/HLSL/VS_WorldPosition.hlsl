#include "BUF_WVPMatrix.hlsli"

struct VS_IN
{
	float3 pos		: POSITION0;
	float2 uv		: TEXCOORD0;
	float4 color	: COLOR0;
	float3 normal	: NORMAL0;
};

struct VS_OUT
{
	float4 pos		: SV_POSITION;
	float2 uv		: TEXCOORD0;
	float4 color	: COLOR0;
	float3 normal	: NORMAL0;
	float3 worldPos : POSITION0;
};

cbuffer VSCommonData : register(b1)
{
	float	g_useTesselate;
	float	g_TessFactor;
	float2	padding;
};

VS_OUT main(VS_IN vin)
{
	VS_OUT vout = (VS_OUT)0;

	vout.pos		= float4(vin.pos, 1.0f);

	if (bool(!g_useTesselate))
	{
		// transform position || 座標変換
		vout.pos = mul(vout.pos, world);
		vout.worldPos = vout.pos.xyz;
		vout.pos = mul(vout.pos, view);
		vout.pos = mul(vout.pos, proj);

		vout.normal = normalize(mul(vin.normal, (float3x3) world));
	}
	else
	{
		// pass through || 変換なし
		vout.worldPos = vin.pos;
		vout.normal = vin.normal;
	}

	vout.uv			= vin.uv;
	vout.color		= vin.color;

	return vout;
}