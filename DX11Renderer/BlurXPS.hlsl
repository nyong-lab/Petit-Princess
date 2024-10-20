#include "BloomUtils.hlsli"


//x = width   y = height   z = 1 / width   w = 1 / height;
cbuffer Frame : register(b0)
{
    float4 gFrame;
};  

Texture2D gTexture : register(t0);

SamplerState mySamplerState
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = CLAMP;
    AddressV = CLAMP;
    AddressW = CLAMP;
};

struct VertexOut
{
    float4 pos : SV_POSITION;
    float2 Tex : TEXCOORD;
};

float4 main(VertexOut input) : SV_TARGET
{
    sampler mySampler = mySamplerState;
    return psBlur_X(input.Tex, gTexture, mySampler, gFrame);
}