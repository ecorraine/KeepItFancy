#ifndef F_NOISE_HLSLI
#define F_NOISE_HLSLI

float randomizer(float2 uv)
{
	return frac(sin(dot(uv, float2(12.9898, 78.233))) * 43758.5453);
}

float hash(float2 p)
{
	float h = dot(p, float2(127.1, 311.7));
	return -1.0f + 2.0f * frac(sin(h) * 43758.5453);
}
float CellNoise(float2 uv)
{
	// Number of Voronoi points
	const int numPoints = 64;

	// Seed values for the Voronoi points
	float2 cellCoord[numPoints];
	for (int i = 0; i < numPoints; ++i)
	{
		cellCoord[i] = float2(randomizer(float2(i, i)), randomizer(float2(i, i + 1)));
	}

	float minDistance = length(uv - cellCoord[0]);
	for (int j = 1; j < numPoints; ++j)
	{
		float distance = length(uv - cellCoord[j]);
		minDistance = min(minDistance, distance);
	}

	// Normalize the distance to [0, 1] range
	return (1.0f - saturate(minDistance));
}

float CellNoiseTilable(float2 uv, float time)
{
	float minDistance = 1.0f;

	for (int y = -1; y <= 1; ++y)
	{
		for (int x = -1; x <= 1; ++x)
		{
			float2 offset = float2((float) x, (float)y);
			float2 cell = hash(floor(uv) + offset);
			cell = 0.5f + 0.5f * sin(time + 6.2831 * cell);
			
			float2 difference = offset + cell - frac(uv);
			
			float distance = length(difference);
			
			minDistance = min(minDistance, distance);
		}
	}
	return (minDistance);
}

#endif // F_NOISE_HLSLI