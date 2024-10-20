#include "BloomUtils.hlsli"
#include "Functions.hlsli"

Texture2D gOriginalTex : register(t0);
Texture2D gBlurOri : register(t1);
Texture2D gBlurHalf : register(t2);
Texture2D gBlurQuarter : register(t3);
Texture2D gBlurEighth : register(t4);


SamplerState gSampler : register(s0);

cbuffer worldProperty : register(b0)
{
    float gBloomIntensity;
    float gBloomThreshold;
    float gTonemapThreshold;
};

struct VertexOut
{
    float4 pos : SV_POSITION;
    float2 Tex : TEXCOORD;
};

float4 main(VertexOut input) : SV_TARGET
{
    // ���� ����
    float4 oriColor = gOriginalTex.Sample(gSampler, input.Tex);
    
    float4 BlurOri = gBlurOri.Sample(gSampler, input.Tex);
    float4 Half = gBlurHalf.Sample(gSampler, input.Tex);
    float4 Quarter = gBlurQuarter.Sample(gSampler, input.Tex);
    float4 Eighth = gBlurEighth.Sample(gSampler, input.Tex);
    
    float4 bloomColor = (0.1 * BlurOri) + (0.1 * Half) + (0.3 * Quarter) + (0.5 *Eighth);
    //float4 bloomColor = 0.25 * (BlurOri + Half + Quarter + Eighth);
    
    // ��� ���
    float3 lit = bloomColor.rgb * gBloomIntensity;
    lit = oriColor.rgb + lit;
    
    // ������ ���Ͼ� �÷��� ��ȯ
    float3 linearColor = GammaToLinear(lit);
    
    // fxaa�� ���� �縶 ���
    float4 final;
    float ruma = rgb2luma(lit); // �縶���� �����÷��� ���ȴ�.
    final = float4(linearColor, ruma); // fxaa���� ���Ͼ��÷��� ���ȴ�.
    return final;
}