#include "Common_HLSL.hlsli"

cbuffer ProgressBuffer : register(b0)
{
    float progress; // 0.0 ~ 1.0
    float3 padding;
};

cbuffer MatrixBuffer : register(b1)
{
    float4x4 projection;
};

PS_IN main(VS_IN input)
{
    PS_IN output;
    //output.pos = float4(input.pos, 1.0f);
    output.pos = mul(float4(input.pos, 1.0f), projection);
    output.color = input.color;
    
    return output;
}