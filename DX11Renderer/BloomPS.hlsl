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
    // 색상 추출
    float4 oriColor = gOriginalTex.Sample(gSampler, input.Tex);
    
    float4 BlurOri = gBlurOri.Sample(gSampler, input.Tex);
    float4 Half = gBlurHalf.Sample(gSampler, input.Tex);
    float4 Quarter = gBlurQuarter.Sample(gSampler, input.Tex);
    float4 Eighth = gBlurEighth.Sample(gSampler, input.Tex);
    
    float4 bloomColor = (0.1 * BlurOri) + (0.1 * Half) + (0.3 * Quarter) + (0.5 *Eighth);
    //float4 bloomColor = 0.25 * (BlurOri + Half + Quarter + Eighth);
    
    // 블룸 계산
    float3 lit = bloomColor.rgb * gBloomIntensity;
    lit = oriColor.rgb + lit;
    
    // 감마를 리니어 컬러로 변환
    float3 linearColor = GammaToLinear(lit);
    
    // fxaa를 위한 루마 계산
    float4 final;
    float ruma = rgb2luma(lit); // 루마에는 감마컬러가 사용된다.
    final = float4(linearColor, ruma); // fxaa에는 리니어컬러가 사용된다.
    return final;
}