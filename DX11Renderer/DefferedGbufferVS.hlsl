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
    // ���
	VertexOut vout;

	// ������ ������� ��ǥ
	vout.PosW = mul(float4(vin.PosL, 1.0f), g_world).xyz;
	
    // ������ ����Ʈ ��ǥ
	vout.PosH = mul(float4(vin.PosL, 1.0f), g_wvp);

	// �ؽ����� ��ǥ (texcoord�� �״�� �ȼ����̴��� �Ѱ� �ش�)
	vout.Tex = vin.Tex;

	//������ ���� �������� ���� ����
	//vout.NormalW = normalize(mul(vin.NormalL, (float3x3)g_worldInv));
	//vout.Tangent = normalize(mul(vin.Tangent, (float3x3)g_worldInv));
	//vout.Binormal = normalize(mul(vin.Binormal, (float3x3)g_worldInv));
    
	vout.Tangent = mul(vin.Tangent, (float3x3) g_world);	
	vout.Binormal = mul(vin.Binormal, (float3x3)g_world);
	vout.NormalW = (mul(vin.NormalL, (float3x3)g_worldInv));


	return vout;
}