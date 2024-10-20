TextureCube skyMap;
SamplerState Sampler;

struct PixelIn
{
    float4 position : SV_POSITION;
    float3 tex : TEXCOORD0;
};

float4 main(PixelIn input) : SV_TARGET
{
    //return float4(1, 1, 1, 1);
    return skyMap.Sample(Sampler, input.tex);
}