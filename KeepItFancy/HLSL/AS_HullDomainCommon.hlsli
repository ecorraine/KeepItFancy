#ifndef S_HULLDOMAIN_COMMON_HLSLI
#define S_HULLDOMAIN_COMMON_HLSLI



#define TRI_CONTROLPOINT 3
struct TRI_OUTPUT
{
    float edgeTess[3]	: SV_TessFactor;
    float insideTess	: SV_InsideTessFactor;
};


#define QUAD_CONTROLPOINT 4
struct QUAD_OUTPUT
{
    float edgeTess[4]	: SV_TessFactor;
    float insideTess[2]	: SV_InsideTessFactor;
};


struct ISOLINE_OUT
{
    float density[2]	: SV_TessFactor;
};

#endif // S_HULLDOMAIN_COMMON_HLSLI