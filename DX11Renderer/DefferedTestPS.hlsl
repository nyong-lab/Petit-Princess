cbuffer Material : register(b0)
{
    bool useBaseColorMap;
    bool useNormalMap;
    bool useMetallicMap;
    bool useRoughnessMap;

    float4 cBaseColor;
    float cMetallic;
    float cRoughness;
}

struct PixelIn
{
    float4 position : SV_Position;
    float2 tex : TEXCOORD0;
};

Texture2D diffuseTex : register(t0);

SamplerState SampleTypeWrap : register(s0);

float4 main(PixelIn input) : SV_TARGET
{
    float4 color    = diffuseTex.Sample(SampleTypeWrap, input.tex);
//return float4(0,1,0,1);    
return color;
}