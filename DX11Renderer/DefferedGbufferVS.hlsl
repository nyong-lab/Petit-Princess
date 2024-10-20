#include "ShaderDefine.hlsli"
cbuffer MatrixBuffer : register(b0)
{
    matrix g_world;
    matrix g_view;
    matrix g_proj;
    matrix g_wvp;
    matrix g_worldInv;
}

VertexOut main(VertexIn vin)
{
    // 출력
	VertexOut vout;

	// 정점의 월드공간 좌표
	vout.PosW = mul(float4(vin.PosL, 1.0f), g_world).xyz;
	
    // 정점의 뷰포트 좌표
	vout.PosH = mul(float4(vin.PosL, 1.0f), g_wvp);

	// 텍스쳐의 좌표 (texcoord를 그대로 픽셀쉐이더로 넘겨 준다)
	vout.Tex = vin.Tex;

	//정점의 월드 공간에서 법선 벡터
	//vout.NormalW = normalize(mul(vin.NormalL, (float3x3)g_worldInv));
	//vout.Tangent = normalize(mul(vin.Tangent, (float3x3)g_worldInv));
	//vout.Binormal = normalize(mul(vin.Binormal, (float3x3)g_worldInv));
    
	vout.Tangent = mul(vin.Tangent, (float3x3) g_world);	
	vout.Binormal = mul(vin.Binormal, (float3x3)g_world);
	vout.NormalW = (mul(vin.NormalL, (float3x3)g_worldInv));


	return vout;
}