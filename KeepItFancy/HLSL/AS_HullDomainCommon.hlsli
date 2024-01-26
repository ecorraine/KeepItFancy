#ifndef S_HULLDOMAIN_COMMON_HLSLI
#define S_HULLDOMAIN_COMMON_HLSLI

cbuffer TessellationFactors : register(b0)
{
	float g_TessFactor;
};

struct VS_OUT
{
	float4 pos		: SV_POSITION;
	float2 uv		: TEXCOORD0;
	float4 color	: COLOR0;
	float3 normal	: NORMAL0;
	float3 worldPos : POSITION0;
};



#define TRI_CONTROLPOINT 3

struct TRI_OUTPUT
{
    float edgeTess[3]	: SV_TessFactor;
    float insideTess	: SV_InsideTessFactor;
};

TRI_OUTPUT PatchTri(
	InputPatch<VS_OUT, TRI_CONTROLPOINT> patch,
	uint patchId : SV_PrimitiveID)
{
	TRI_OUTPUT output = (TRI_OUTPUT)0;

	output.edgeTess[0]	= g_TessFactor;
	output.edgeTess[1]	= g_TessFactor;
	output.edgeTess[2]	= g_TessFactor;

	output.insideTess = g_TessFactor;

	return output;
}



#define QUAD_CONTROLPOINT 4

struct QUAD_OUTPUT
{
    float edgeTess[4]	: SV_TessFactor;
    float insideTess[2]	: SV_InsideTessFactor;
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


struct ISOLINE_OUT
{
    float density[2]	: SV_TessFactor;
};

#endif // S_HULLDOMAIN_COMMON_HLSLI