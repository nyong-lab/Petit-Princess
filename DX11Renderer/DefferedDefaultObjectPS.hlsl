#include "ShaderDefine.hlsli"

cbuffer Material : register(b0)
{
    bool useBaseColorMap;
    bool useNormalMap;
    bool useMetallicMap;
    bool useRoughnessMap;
    bool useAOMap;
    float3 pad3;
    float4 cBaseColor;
    float cMetallic;
    float cRoughness;
    float cAO;
    float pad;
}

SamplerState SampleType : register(s0);

Texture2D BaseColorMap : register(t0);
Texture2D normalMap : register(t1);
Texture2D metallicMap : register(t2);
Texture2D roughnessMap : register(t3);
Texture2D AOMap : register(t4);

///TODO:: 메탈릭, 러프니스, AO 타겟 하나로 합치기
PixelOut main(PixelIn pin)
{
    PixelOut output;

	// diffuse------------------------------------------------------------------------
	//output.texBaseColor = (useBaseColorMap == true) ? BaseColorMap.Sample(SampleType, pin.Tex) : cBaseColor;

    if (useBaseColorMap == true)
    {
        output.texBaseColor = BaseColorMap.Sample(SampleType, pin.Tex);
        if (output.texBaseColor.a < 0.01)
            discard;
    }
    else
    {
        output.texBaseColor = cBaseColor;
    }

	// normal--------------------------------------------------------------------------
    output.texNormal = float4(0, 0, 0, 0);
	
	// roughness ------------------------------------------------------------------------
    if (useRoughnessMap == true)
    {
        output.roughnessMetallicAO.r = roughnessMap.Sample(SampleType, pin.Tex).x;
    }
    else
    {
        output.roughnessMetallicAO.r = cRoughness;
    }

	// metalic--------------------------------------------------------------------------
    if (useMetallicMap == true)
    {
        output.roughnessMetallicAO.g = metallicMap.Sample(SampleType, pin.Tex).x;
    }
    else
    {
        output.roughnessMetallicAO.g = cMetallic;
    }
	
	// AO --------------------------------------------------------------------------
    if (useAOMap == true)
    {
        output.roughnessMetallicAO.b = AOMap.Sample(SampleType, pin.Tex).x;
    }
    else
    {
        output.roughnessMetallicAO.b = cAO;
    }
	
	
	// NDC -----------------------------------------------------------------------------
    float4 worldPos = float4(pin.PosW.xyz, 1.0f);
    output.worldPosition = worldPos;

	// depth ----------------------------------------------------------------------------
    float d = pin.PosH.z / pin.PosH.w;
    d *= 10;
    output.worldPosition.w = 1 - d;

    return output;
}