SamplerState SampleType;

Texture2D call : register(t0);
Texture2D chase : register(t1);
Texture2D skill : register(t2);
Texture2D susp : register(t3);
Texture2D bg : register(t4);

cbuffer Infos
{
    int state; // 무엇을 띄울지? 
    // 0 : 
    // 1 : 
    // 2 : chase
    // 3 : call
    // 4 : susp
    // 5 : skill
}

struct PixelIn
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};

float4 main(PixelIn input) : SV_TARGET
{
    float4 backg = bg.Sample(SampleType, input.tex);
    float4 color;
    
    switch (state)
    {
        case 2:
            color = chase.Sample(SampleType, input.tex);
            break;
        case 3:
            color = call.Sample(SampleType, input.tex);
            break;
        case 4:
            color = susp.Sample(SampleType, input.tex);
            break;
        case 5:
            color = skill.Sample(SampleType, input.tex);
            break;
        default:
            return float4(0, 0, 0, 0);
    }
    
    return float4(color.rgb * color.a + backg.rgb * (1 - color.a), backg.a + color.a);
}