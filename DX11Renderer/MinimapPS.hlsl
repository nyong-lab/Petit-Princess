SamplerState samplerState;

Texture2D texMinimap : register(t0);

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
    /// ��� ������ (������ �� ���� �ڵ�)
    //float3 output;
    
    //float2 textureOffset = float2(0.0f, 0.0f); // �������� (0, 0)���� ����
    //float2 textureSize = float2(1.0f, 1 - textureTranslation); // ���� / ���� ����� �κ�(�ִ� 1.0f)
    //float4 result;
    //float4 tex;
    //float4 outline = texOutline.Sample(samplerState, input.tex);
    
           
    //float3 color = float3(1, 0, 0);
  
    //output = outline.rgb * outline.a + (tex.rgb * color) * (1 - outline.a);
    //float alpha = tex.a + outline.a;
    //return float4(output, alpha);
    
    float3 output;
    
    float4 tex = texMinimap.Sample(samplerState, input.tex);
    
    return float4(tex);
}