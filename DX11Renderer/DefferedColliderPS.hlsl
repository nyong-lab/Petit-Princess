#include "ShaderDefine.hlsli"

cbuffer Physics : register(b0)
{
    int isCollision;
    float3 pad;
}

PixelOut main(PixelIn pin)
{
    PixelOut output;


	// diffuse------------------------------------------------------------------------

    if (isCollision == 1)
    {
        output.texBaseColor = float4(1.f, 0.f, 0.f, 1.f);

    }
    else if (isCollision == 0)
    {
        output.texBaseColor = float4(0.f, 1.f, 0.f, 1.f);
    }
    else
    {
        output.texBaseColor = float4(1.f, 1.f, 0.f, 1.f);
    }

	// NDC -----------------------------------------------------------------------------
    float4 worldPos = float4(pin.PosW.xyz, 1.0f);
    output.worldPosition = worldPos;

	// depth ----------------------------------------------------------------------------
    float d = pin.PosH.z / pin.PosH.w;
    d *= 10;
    output.worldPosition.w = 1 - d;

    output.roughnessMetallicAO = float4(1, 0, 1, 1);
    output.texNormal = float4(0, 0, 0, 0);
    
    return output;
}