Texture2D texOutline : register(t0);
Texture2D texBG      : register(t1);
Texture2D texIn      : register(t2);


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
    //pin.Tex.x+=textureTranslation;
     if(UItype == 1)
     {
     // 텍스처 좌표가 0과 1을 벗어나면 frac 함수를 사용하여 해당 범위 내로 되돌려주어 반복
         input.tex.x = frac(input.tex.x);
         input.tex.y = frac(input.tex.y);
         texColor = texOutline.Sample(SampleType, input.tex);
         if (texColor.a < 0.01)
             discard;
 
     }
     else if(UItype == 2)
     {
         float2 textureOffset = float2(0.0f, 0.0f);  // 시작점을 (0, 0)으로 설정
         float2 textureSize = float2(1.0f, 1-textureTranslation);    // 가로 / 세로 사용할 부분(최대 1.0f)


        // 텍스처 좌표가 특정 영역을 벗어나면 투명하게 처리
        if (input.tex.x < textureOffset.x || input.tex.x > textureOffset.x + textureSize.x ||
            1-input.tex.y < textureOffset.y || 1-input.tex.y > textureOffset.y + textureSize.y)
        {
            discard; // 투명하게 처리
        }
        else
        {
            // Sample texture.
            texColor = texOutline.Sample(SampleType, input.tex);
        }
    }
    else if (UItype == 3)
    {
        float3  output = float3(0.0f, 0.0f, 0.0f);;
        float2 textureOffset = float2(0.0f, 0.0f); // 시작점을 (0, 0)으로 설정
        float2 textureSize = float2(textureTranslation, 1.0f); // 가로 / 세로 사용할 부분(최대 1.0f)
        float4 outLine = texOutline.Sample(SampleType, input.tex);
        float4 inGauge = float4(0,0,0,0);
        float4 bg=float4(0,0,0,0);
        float alpha=0.0f;
        // 텍스처 좌표가 특정 영역을 벗어나면 투명하게 처리
        if (input.tex.x < textureOffset.x || input.tex.x > textureOffset.x + textureSize.x ||
            1-input.tex.y < textureOffset.y || 1-input.tex.y > textureOffset.y + textureSize.y)
        {
            inGauge.a = 0.f;
        }
        else
        {
            // Sample texture.          
            inGauge = texIn.Sample(SampleType, input.tex);
        }
            bg = texBG.Sample(SampleType, input.tex);
            output = inGauge.rgb * inGauge.a + bg.rgb * (1 - inGauge.a);
            output = outLine.rgb * outLine.a + output.rgb * (1 - outLine.a);
            if(isFirst)
            {
                alpha=0.0f;
                
            }else
            {
                alpha=1.0f;
                alpha=inGauge.a+bg.a+outLine.a;
            }
            
            texColor=float4(output, alpha);
    }
    else
    {
         texColor = texOutline.Sample(SampleType, input.tex);
         if (texColor.a < 0.01)
             discard;
    }
    return texColor;
    //return float4(1.0f, 0.f, 0.f, 1.f);
}