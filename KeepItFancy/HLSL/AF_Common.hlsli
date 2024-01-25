#ifndef F_COMMON_HLSLI
#define F_COMMON_HLSLI

float3 RGBtoFloat(int red, int green, int blue)
{
	return float3(red/255, green/255, blue/255);
}

#endif // F_COMMON_HLSLI