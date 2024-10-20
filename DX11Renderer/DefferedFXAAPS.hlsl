#define FXAA_HLSL_5 1
#define FXAA_PC 1
#define FXAA_QUALITY__PRESET 12

#include "FXAA3_11.hlsli"
#include "Functions.hlsli"

cbuffer Texel : register(b0)
{
    float4 fxaaQualityRcpFrame; //텍셀의?크기.xy,?x에는?1.0f/pixelWidth,?y에는?1.0/pixelHeight??

//     FxaaFloat fxaaQualitySubpix;
//     FxaaFloat fxaaQualityEdgeThreshold;
//     FxaaFloat fxaaQualityEdgeThresholdMin;
};    

struct VertexOut
{
    float4 pos : SV_POSITION;
    float2 Tex : TEXCOORD;
};

Texture2D gTexture : register(t0);

//Insure thetexture sampler(s) used by FXAA are set to bilinear filtering.
SamplerState mySamplerState
{
    Filter = MIN_MAG_LINEAR_MIP_POINT; // MIN_MAG_LINEAR_MIP_POINT는 bilinear filtering을 설정합니다.
    AddressU = CLAMP; // U 방향의 텍스처 주소 모드를 CLAMP로 설정합니다.
    AddressV = CLAMP; // V 방향의 텍스처 주소 모드를 CLAMP로 설정합니다.
    AddressW = CLAMP; // W 방향의 텍스처 주소 모드를 CLAMP로 설정합니다.
};


// FxaaVS_Output FxaaVS(uint id : SV_VertexID)
// {
//     FxaaVS_Output Output;
//     Output.Tex = float2((id << 1) & 2, id & 2);
//     Output.Pos = float4(Output.Tex * float2(2.0f, -2.0f) + float2(-1.0f, 1.0f), 0.0f, 1.0f);
//     return Output;
// }


float4 main(VertexOut input) : SV_TARGET
{
    FxaaTex InputFXAATex = { mySamplerState, gTexture };
    
    float4 fxaaColor = FxaaPixelShader(
        input.Tex.xy, // FxaaFloat2 pos,
        FxaaFloat4(0.0f, 0.0f, 0.0f, 0.0f), // FxaaFloat4 fxaaConsolePosPos,
        InputFXAATex, // FxaaTex tex,
        InputFXAATex, // FxaaTex fxaaConsole360TexExpBiasNegOne,
        InputFXAATex, // FxaaTex fxaaConsole360TexExpBiasNegTwo,
        fxaaQualityRcpFrame.zw, // FxaaFloat2 fxaaQualityRcpFrame, 
        FxaaFloat4(0.0f, 0.0f, 0.0f, 0.0f), // FxaaFloat4 fxaaConsoleRcpFrameOpt,
        FxaaFloat4(0.0f, 0.0f, 0.0f, 0.0f), // FxaaFloat4 fxaaConsoleRcpFrameOpt2,
        FxaaFloat4(0.0f, 0.0f, 0.0f, 0.0f), // FxaaFloat4 fxaaConsole360RcpFrameOpt2,
        0.75f, // FxaaFloat fxaaQualitySubpix,
        0.125f, // FxaaFloat fxaaQualityEdgeThreshold,
        0.0833f, // FxaaFloat fxaaQualityEdgeThresholdMin,
        0.0f, // FxaaFloat fxaaConsoleEdgeSharpness,
        0.0f, // FxaaFloat fxaaConsoleEdgeThreshold,
        0.0f, // FxaaFloat fxaaConsoleEdgeThresholdMin,
        FxaaFloat4(0.0f, 0.0f, 0.0f, 0.0f) // FxaaFloat fxaaConsole360ConstDir,
        );

    // 리니어공간의 색을 다시 감마 공간으로 변환해준다.
    float3 color = fxaaColor.xyz;
    color = LinearToGamma(color);
    return float4(color, fxaaColor.w);
}


    //   1.00 - upper limit (softer)
    //   0.75 - default amount of filtering
    //   0.50 - lower limit (sharper, less sub-pixel aliasing removal)
    //   0.25 - almost off
    //   0.00 - completely off
    // FxaaFloat fxaaQualitySubpix, 


    // The minimum amount of local contrast required to apply algorithm.
    //   0.333 - too little (faster)
    //   0.250 - low quality
    //   0.166 - default
    //   0.125 - high quality 
    //   0.063 - overkill (slower)
    // FxaaFloat fxaaQualityEdgeThreshold, 

   
    // Trims the algorithm from processing darks.
    //   0.0833 - upper limit (default, the start of visible unfiltered edges)
    //   0.0625 - high quality (faster)
    //   0.0312 - visible limit (slower)
    // FxaaFloat fxaaQualityEdgeThresholdMin, 