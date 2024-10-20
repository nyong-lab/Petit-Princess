Texture2D texBG : register(t0);

SamplerState SampleType;

cbuffer TextureTranslation : register(b0)
{
    float textureTranslation;
    int UItype;
    bool isFirst;
    float pad;
}; 

struct PixelIn
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};

float4 main(PixelIn input) : SV_TARGET
{
    float4 texColor = float4(1, 1, 1, 0);
    texColor = texBG.Sample(SampleType, input.tex);
    texColor.a = textureTranslation;
    return texColor;
    //return float4(1.0f, 0.f, 0.f, 1.f);
}