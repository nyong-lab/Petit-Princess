Texture2D Texture;
SamplerState SampleType;

cbuffer MiniGameButton : register(b0)
{
    bool isActive;
    float3 pad;
};

struct PixelIn
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};

float4 main(PixelIn input) : SV_TARGET
{
    if(isActive == false) discard;

	float4 texColor = Texture.Sample(SampleType, input.tex);
    if(texColor.a < 0.01) discard;

    return texColor;
}