
SamplerState samplerState;

Texture2D texOutline : register(t0); 
Texture2D texSun     : register(t1); 
Texture2D texBG      : register(t2); 
Texture2D texMask    : register(t3); 

struct PixelIn
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float2 suntex : TEXCOORD1;
};

float4 main(PixelIn input) : SV_TARGET
{
    float3 output;
    
    float4 outline = texOutline.Sample(samplerState, input.tex);
    float4 sun = texSun.Sample(samplerState, input.suntex);
    float4 bg = texBG.Sample(samplerState, input.tex);
    float4 mask = texMask.Sample(samplerState, input.tex);
    
    //return outline;

    // 해와 배경
    sun.a *= (1 - mask.a);
    output = sun.rgb * sun.a + bg.rgb * (1 - sun.a);
    
    // 아웃라인
    output = outline.rgb * outline.a + output.rgb * (1 - outline.a);
    
    // 알파값 계산
    float alpha = sun.a + bg.a + outline.a;
    //if(alpha < 0.01) discard;
    return float4(output, alpha);
}