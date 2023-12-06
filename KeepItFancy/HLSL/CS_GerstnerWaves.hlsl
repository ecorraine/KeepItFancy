// Constants
#define DIV_X 16
#define DIV_Y 16
#define DIV_Z 2

#define XM_PI 3.141592654f

// Buffers
cbuffer MeshData : register(b0)
{
	float divX;
	float divY;
	float g_time;
	float dummy;
};

RWByteAddressBuffer g_Vertices : register(u0);

[numthreads(DIV_X, DIV_Y, DIV_Z)]
void main(uint3 DTid : SV_DispatchThreadID)
{
	uint index = DTid.z * divX + DTid.x + DTid.z;
	const uint vbInIndex = index * 40;

	float3 pos = asfloat(g_Vertices.Load3(vbInIndex + 0));
	float4 color = asfloat(g_Vertices.Load4(vbInIndex + 12));
	float3 normal = asfloat(g_Vertices.Load3(vbInIndex + 28));

	float time = g_time;
	float frequency = 1.0f;
	float amplitude = 0.1f;

	// Compute wave displacement
	float waveHeight = amplitude * sin(2 * XM_PI * frequency * pos.x + time);

	// Apply wave displacement to the Y-coordinate of the position
	pos.y += waveHeight;

	const uint vbOutIndex = vbInIndex;
	g_Vertices.Store3(vbOutIndex + 0, asuint(pos));
	g_Vertices.Store4(vbOutIndex + 12, asuint(color));
	g_Vertices.Store3(vbOutIndex + 28, asuint(normal));
}
