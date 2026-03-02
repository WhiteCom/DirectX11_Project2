struct VS_IN
{
    float3 pos : POSITION;
    float4 color : COLOR;
};

struct PS_IN
{
    float4 pos : SV_Position;
    float4 color : COLOR;
};
