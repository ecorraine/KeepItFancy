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
			cell = 0.5f + 0.5f * sin(time + 6.2831 * cell);		// Smoothly animate the cell centers
			
			float2 difference = offset + cell - frac(uv);
			
			float distance = length(difference);				// Distance to the nearest cell center
			
			minDistance = min(minDistance, distance);			// Keep the shortest distance
		}
	}
	return (minDistance);
}

// Perlin noise functions
float fade(float t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

float gradient(int hash, float x) {
    int h = hash & 15;
    float grad = 1.0 + (h & 7);		// Gradient value 1-8
    if (h & 8) grad = -grad;		// Randomly invert half of them
    return (grad * x);
}

float perlin(float2 p) {
    // Determine grid cell coordinates
    float2 pi = floor(p);
    // Relative position in the cell
    float2 pf = p - pi;
    
    // Compute fade curves for each of Pf's components
    float2 fadePf = float2(fade(pf.x), fade(pf.y));

    // Hash coordinates of the 4 cube corners
    int2 hashP = (int2(pi.x, pi.y) + int2(0, 0)) & 255;

    // And add blended results from 2 corners of the cube
    float2 gradP = float2(gradient(hashP.x, pf.x), gradient(hashP.y, pf.y));
    float2 dotPfGrad = float2(dot(gradP, pf - float2(0, 0)), dot(gradP, pf - float2(1, 0)));

    // Interpolate the results along each axis
    return lerp(dotPfGrad.x, dotPfGrad.y, fadePf.x);
}

#endif // F_NOISE_HLSLI