#include "ShaderDefine.hlsli"

cbuffer MatrixBuffer : register(b0)
{
    matrix g_world;
    matrix g_view;
    matrix g_proj;
    matrix g_wvp;
    matrix g_worldInv;
}

cbuffer cbBonePalette : register(b1)
{
    matrix g_boneMat[512];
}

VertexOut main(SkinnedVertexIn vin)
{
    // 출력
	VertexOut vout;
    float3 normalL = float3(0.0f, 0.0f, 0.0f);

	matrix combindedMatrix = mul(vin.Weights.x, g_boneMat[vin.Indices.x]);;
	for (int i = 0; i < 4; ++i)
	{
		if (vin.Indices[i] == -1)
		{
			break;
		}

		combindedMatrix += mul(vin.Weights[i], g_boneMat[vin.Indices[i]]);
	}

	// 정점의 월드공간 좌표
	vout.PosW = mul(float4(vin.PosL, 1.0f), mul(combindedMatrix, g_world)).xyz;
	
    // 정점의 뷰포트 좌표
	vout.PosH = mul(float4(vin.PosL, 1.f), mul(combindedMatrix, g_world));
	vout.PosH = mul(vout.PosH, g_view);
	vout.PosH = mul(vout.PosH, g_proj);
	
	normalL = mul(vin.NormalL,(float3x3) combindedMatrix);
	
	// 텍스쳐의 좌표 (texcoord를 그대로 픽셀쉐이더로 넘겨 준다)
	vout.Tex = vin.Tex;
	
	//정점의 월드 공간에서 법선 벡터
	vout.NormalW = (mul(normalL, (float3x3) g_worldInv));
	vout.Tangent = (mul(vin.Tangent, (float3x3) g_world));
	vout.Binormal = (mul(vin.Binormal, (float3x3) g_worldInv));
    
	return vout;
}