#include "Common_HLSL.hlsli"

float4 main(PS_IN input) : SV_Target
{
    return input.color;
}