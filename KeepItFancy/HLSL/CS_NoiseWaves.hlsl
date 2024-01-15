#include "F_Noise.hlsli"

#define Thread_X (1)
#define Thread_Y (1)
#define Thread_Z (1)

#define XM_PI 3.141592653589793238
#define XM_TWOPI (2 * XM_PI)

struct VERTEX
{
	float3 pos;
	float2 uv;
	float4 color;
	float3 normal;
};

cbuffer MeshData : register(b0)
{
	float4 offsetSize;
	float4 count;
	float4 g_time;
};

RWByteAddressBuffer outputVertices : register(u0);

[numthreads(Thread_X, Thread_Y, Thread_Z)]
void main( uint3 DTid : SV_DispatchThreadID )
{
	uint index = DTid.x;
	const uint offset = index * 48;

	float3 pos = asfloat(outputVertices.Load3(offset + offsetSize.x));
	float2 uv = asfloat(outputVertices.Load2(offset + offsetSize.y));
	float4 color = asfloat(outputVertices.Load4(offset + offsetSize.z));
	float3 normal = asfloat(outputVertices.Load3(offset + offsetSize.w));

	float time = g_time.x;
	float amplitude = 0.0005f;
	float frequency = 1.0f;
	float k = XM_TWOPI * frequency;

	// compute radial wave displacement
	float radialDistance = sqrt(pos.x * pos.x + pos.z * pos.z);

	// add random offset to wave
	float randomOffset = XM_TWOPI * frac(sin(radialDistance) * 143758.5453);

	// compute displacement
	float waveHeight = amplitude * sin(k * radialDistance + time + randomOffset);
	// Apply wave displacement to the Y-coordinate of the vertex position
	pos.y += waveHeight;
	
	// Write the updated vertex back to the buffer
	outputVertices.Store3(offset + offsetSize.x, asuint(pos));
	outputVertices.Store2(offset + offsetSize.y, asuint(uv));
	outputVertices.Store4(offset + offsetSize.z, asuint(color));
	outputVertices.Store3(offset + offsetSize.w, asuint(normal));
}
