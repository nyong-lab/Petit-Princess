SamplerState samplerState;

Texture2D texOutline : register(t0); 
Texture2D texBG     : register(t1);
Texture2D texIn     : register(t2);


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

    float4 texColor = float4(1, 1, 1, 0);
     float3  output = float3(0.0f, 0.0f, 0.0f);;
     float2 textureOffset = float2(0.0f, 0.0f); // �������� (0, 0)���� ����
     float2 textureSize = float2(1.0f, textureTranslation-0.07); // ���� / ���� ����� �κ�(�ִ� 1.0f)
     float4 outLine = texOutline.Sample(samplerState, input.tex);
     float4 inGauge = float4(0,0,0,0);
     float4 bg=float4(0,0,0,0);
     float alpha=0.0f;
     // �ؽ�ó ��ǥ�� Ư�� ������ ����� �����ϰ� ó��
     if (input.tex.x < textureOffset.x || input.tex.x > textureOffset.x + textureSize.x ||
         1-input.tex.y < textureOffset.y || 1-input.tex.y > textureOffset.y + textureSize.y)
     {
         inGauge.a = 0.f;
     }
     else
     {
         // Sample texture.          
         inGauge = texIn.Sample(samplerState, input.tex);
     }
         bg = texBG.Sample(samplerState, input.tex);
         output = inGauge.rgb * inGauge.a + bg.rgb * (1 - inGauge.a);
         output = outLine.rgb * outLine.a + output.rgb * (1 - outLine.a);
    
         
         alpha=inGauge.a+bg.a+outLine.a;
      
         
         texColor=float4(output, alpha);
        return texColor;
}