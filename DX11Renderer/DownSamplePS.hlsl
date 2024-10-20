Texture2D InputTexture : register(t0);
SamplerState SampleType : register(s0);

//x = width   y = height   z = 1 / width   w = 1 / height;
cbuffer Frame : register(b0)
{
    float4 gFrame;
};  

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float2 texCoord : TEXCOORD;
};

float4 main(VS_OUTPUT input) : SV_TARGET
{
    float2 texSize = float2(gFrame.z, gFrame.w);
    
    // °¢ ÇÈ¼¿ÀÇ Áß½É ÁÂÇ¥
    float2 pixelCenter = input.texCoord * 2.f;
    pixelCenter = input.texCoord + texSize * 0.5f;
    
    // ÀÎÁ¢ÇÑ 4°³ÀÇ ÇÈ¼¿ »ùÇÃ¸µ
    float4 sample1 = InputTexture.Sample(SampleType, pixelCenter + float2(-texSize.x, -texSize.y));
    float4 sample2 = InputTexture.Sample(SampleType, pixelCenter + float2(texSize.x, -texSize.y));
    float4 sample3 = InputTexture.Sample(SampleType, pixelCenter + float2(-texSize.x, texSize.y));
    float4 sample4 = InputTexture.Sample(SampleType, pixelCenter + float2(texSize.x, texSize.y));

    // »ùÇÃµÈ °ªµéÀÇ Æò±Õ °è»ê
    float4 result = (sample1 + sample2 + sample3 + sample4) * 0.25f;
    
    return result;
}