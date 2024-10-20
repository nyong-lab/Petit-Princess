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
    matrix g_boneMat[128];
}

ShadowVertexOut main(SkinnedVertexIn vin)
{
    // ���
	ShadowVertexOut vout;

	matrix combindedMatrix = mul(vin.Weights.x, g_boneMat[vin.Indices.x]);;
	for (int i = 0; i < 4; ++i)
	{
		if (vin.Indices[i] == -1)
		{
			break;
		}

		combindedMatrix += mul(vin.Weights[i], g_boneMat[vin.Indices[i]]);
	}

	// ������ ������� ��ǥ
	//vout.PosW = mul(float4(vin.PosL, 1.0f), mul(combindedMatrix, g_world)).xyz;
	
    // ������ ����Ʈ ��ǥ
    vout.position = mul(float4(vin.PosL, 1.f), mul(combindedMatrix, g_world));
    vout.position = mul(vout.position, g_view);
    vout.position = mul(vout.position, g_proj);
	
	return vout;
}