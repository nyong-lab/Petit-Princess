Texture2D shaderTexture;
SamplerState SampleType;

struct PixelIn
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};


float4 main(PixelIn input) : SV_TARGET
{
    float4 textureColor;
    //input.tex.y *= -1;
    textureColor = shaderTexture.Sample(SampleType, input.tex);
    
    if (textureColor.a < 0.01)
        discard;
    
    return textureColor;
}