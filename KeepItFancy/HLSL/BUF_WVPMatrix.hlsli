#ifndef BUF_WVPMATRIX_HLSLI
#define BUF_WVPMATRIX_HLSLI

cbuffer WVP : register(b0)
{
	float4x4 world;
	float4x4 view;
	float4x4 proj;
};

#endif // BUF_WVPMATRIX_HLSLI