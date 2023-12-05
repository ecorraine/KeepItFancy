// Constants
#define DIV_X 32
#define DIV_Y 32

// Buffers
cbuffer MeshData : register(b0)
{
	float divX;
	float divY;
	float g_time;
	float dummy;
};

RWByteAddressBuffer g_Vertices : register(u0);

[numthreads(DIV_X, 1, DIV_Y)]
void main(uint3 DTid : SV_DispatchThreadID)
{
	uint index = DTid.z * divX + DTid.x + DTid.z;
	const uint vbInIndex = index * 40;

	float3 pos = asfloat(g_Vertices.Load3(vbInIndex + 0));
	float4 color = asfloat(g_Vertices.Load4(vbInIndex + 12));
	float3 normal = asfloat(g_Vertices.Load3(vbInIndex + 28));

	float time = 0.0f;
	time = g_time;
	float frequency = 1.0f;
	float amplitude = 0.1f;

	float waveHeight = amplitude * sin(time * frequency + pos.x);
	// Update the Y component of the position
	pos.y += waveHeight;

	const uint vbOutIndex = vbInIndex;
	g_Vertices.Store3(vbOutIndex + 0, asuint(pos));
	g_Vertices.Store4(vbOutIndex + 12, asuint(color));
	g_Vertices.Store3(vbOutIndex + 28, asuint(normal));
}
