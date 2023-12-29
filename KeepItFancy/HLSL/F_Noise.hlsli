#ifndef F_NOISE_HLSLI
#define F_NOISE_HLSLI

float rand(float2 uv)
{
	return frac(sin(dot(uv, float2(12.9898f, 78.233f))) * 43758.5453f);
}

float VoronoiNoise(float2 uv, float time)
{
	// Number of Voronoi points
	const int numPoints = 8;

	// Seed values for the Voronoi points
	float2 points[numPoints];
	for (int i = 0; i < numPoints; ++i)
	{
		points[i] = float2(rand(float2(i, time)), rand(float2(i, time + 1)));
	}

	float minDistance = length(uv - points[0]);
	for (int i = 1; i < numPoints; ++i)
	{
		float distance = length(uv - points[i]);
		minDistance = min(minDistance, distance);
	}

	// Normalize the distance to [0, 1] range
	return 1.0f - saturate(minDistance);
}

#endif // F_NOISE_HLSLI