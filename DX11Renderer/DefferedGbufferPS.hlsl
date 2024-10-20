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

Texture2D BaseColorMap	: register(t0);
Texture2D normalMap		: register(t1);
Texture2D OMR			: register(t2);

float3 NormalSampleToWorldSpace(float3 normalMapSample, float3 unitNormalW, float3 tangentW)
{
	// Uncompress each component from [0,1] to [-1,1].
    float3 normalT = 2.0f * normalMapSample - 1.0f;

	// Build orthonormal basis.
    float3 N = unitNormalW;
    float3 T = normalize(tangentW - dot(tangentW, N) * N);
    float3 B = cross(N, T);

    float3x3 TBN = float3x3(T, B, N);

	// Transform from tangent space to world space.
    float3 bumpedNormalW = mul(normalT, TBN);

    return bumpedNormalW;
}


PixelOut main(PixelIn pin)
{
	PixelOut output;

	// diffuse------------------------------------------------------------------------
	//output.texBaseColor = (useBaseColorMap == true) ? BaseColorMap.Sample(SampleType, pin.Tex) : cBaseColor;

	if(useBaseColorMap == true)
	{
		output.texBaseColor = BaseColorMap.Sample(SampleType, pin.Tex);
		if (output.texBaseColor.a < 0.01) discard;
	}
	else
	{
		output.texBaseColor = cBaseColor;
	}

	// normal--------------------------------------------------------------------------
	if (useNormalMap == true)
	{
		float3 mappedNormal = normalMap.Sample(SampleType, pin.Tex).xyz;
// 		mappedNormal = mappedNormal * 2.0f - 1.0f;
// 
// 		float3 finalNormal;
// 		float3x3 TBN = float3x3(pin.Tangent, pin.Binormal, pin.NormalW); // 법선맵에서 가져온 벡터를 접선공간으로의 법선벡터로 매핑할 변환행렬.
// 		finalNormal = normalize(mul(mappedNormal, TBN)); // 접선공간의 법선벡터로 변환.
// 		finalNormal = (finalNormal + 1.0f) * 0.5f;
// 		output.texNormal = float4(finalNormal, 1.0f);
		
		pin.NormalW = normalize(pin.NormalW);        
		float3 bumpedNormalW = NormalSampleToWorldSpace(mappedNormal, pin.NormalW, pin.Tangent);
        bumpedNormalW = (bumpedNormalW + 1.0f) * 0.5f;
		output.texNormal = float4(bumpedNormalW, 1.0f);
	}
	else
	{
		float3 tempNormal = pin.NormalW;
		tempNormal = (tempNormal + 1.0f) * 0.5f;
		output.texNormal = float4(tempNormal, 1.0f);
	}	
	
	// roughness ------------------------------------------------------------------------
	if(useRoughnessMap == true)
	{
        output.roughnessMetallicAO.r = OMR.Sample(SampleType, pin.Tex).b;
    }
	else
	{
        output.roughnessMetallicAO.r = cRoughness;
    }

	// metalic--------------------------------------------------------------------------
    if (useMetallicMap == true)
    {
        output.roughnessMetallicAO.g = OMR.Sample(SampleType, pin.Tex).g;
    }
    else
    {
        output.roughnessMetallicAO.g = cMetallic;
    }
	
	// AO --------------------------------------------------------------------------
    if (useAOMap == true)
    {
        output.roughnessMetallicAO.b = OMR.Sample(SampleType, pin.Tex).r;
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