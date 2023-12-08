#define Thread_X (1)
#define Thread_Y (1)
#define Thread_Z (1)

#define XM_PI 3.141592653589793238

struct VERTEX
{
	float3 pos;
	float4 color;
	float3 normal;
};

cbuffer MeshData : register(b0)
{
	float4 size;
	float4 count;
	float4 g_time;
};

//StructuredBuffer<VERTEX> inputVertices : register(t0);
RWByteAddressBuffer outputVertices : register(u0);

[numthreads(Thread_X, Thread_Y, Thread_Z)]
void main(uint3 DTid : SV_DispatchThreadID)
{
	//uint index = DTid.z * count.x + DTid.x + DTid.z;
	uint index = DTid.x;
	const uint offset = index * 40;

	float time = g_time.x;
	float frequency = 1.0f;
	float amplitude = 0.1f;

	float3 pos = asfloat(outputVertices.Load3(offset + 0));
	float4 color = asfloat(outputVertices.Load4(offset + 12));
	float3 normal = asfloat(outputVertices.Load3(offset + 28));

	// Compute wave displacement
	float waveHeight = amplitude * sin(2 * XM_PI * frequency * pos.x + time);

	// Apply wave displacement to the Y-coordinate of the position
	pos.y += waveHeight;

	// Write the updated vertex back to the buffer
	outputVertices.Store3(offset + 0, asuint(pos));
	outputVertices.Store4(offset + 12, asuint(color));
	outputVertices.Store3(offset + 28, asuint(normal));
}
