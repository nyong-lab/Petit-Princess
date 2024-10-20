SamplerState samplerState;

Texture2D texBG : register(t0); 
Texture2D texIn     : register(t1);


cbuffer TextureTranslation : register(b0)
{
    float textureTranslation;
    int UItype;
    float2 pad;
}; 

struct VertexOut
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};

float4 main(VertexOut input) : SV_TARGET
{
     float4 bg = texBG.Sample(samplerState, input.tex);
     float4 inGauge  = texIn.Sample(samplerState, input.tex);
    
     float3 output = inGauge.rgb * inGauge.a + bg.rgb * (1 - inGauge.a);       
     return float4(output, inGauge.a + bg.a);
}