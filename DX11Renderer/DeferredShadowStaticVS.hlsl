#include "ShaderDefine.hlsli"
cbuffer MatrixBuffer : register(b0)
{
    matrix g_world;
    matrix g_view;
    matrix g_proj;
    matrix g_wvp;
    matrix g_worldInv;
}

ShadowVertexOut main(VertexIn vin)
{
    // 출력
	ShadowVertexOut vout;

    // 정점의 뷰포트 좌표
    vout.position = mul(float4(vin.PosL, 1.f),  g_world);
    vout.position = mul(vout.position, g_view);
    vout.position = mul(vout.position, g_proj);

	return vout;
}