Texture2D shaderTexture;
SamplerState SampleType;

struct PixelIn
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITION;
    float2 UV : TEXCOORD0;
};

float4 main(PixelIn input) : SV_TARGET
{
    float4 textureColor;
    textureColor = shaderTexture.Sample(SampleType, input.UV);
    
    if (textureColor.a < 0.01)
        discard;
    
    return textureColor;
}