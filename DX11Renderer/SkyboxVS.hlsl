cbuffer MatrixBuffer
{
    matrix g_world;
    matrix g_wvp; //���� x �� x ��������
    matrix g_worldInv;
};

struct VertexOut
{
    float4 position : SV_POSITION;
    float3 tex : TEXCOORD0;
};


VertexOut main(float3 pos : POSITION)
{
    VertexOut output;
    
    // z/w = 1 �� �ǵ���(�� �ϴ� ���� �׻� �� ��鿡 �ֵ���) z = w�� �����Ѵ�. 
    // Set Pos to xyww instead of xyzw, so that z will always be 1 (furthest from camera)
    //output.position = mul(g_wvp, float4(pos, 1.0f)).xyww;
    output.tex = pos;
    output.position = mul(float4(pos, 1.0f), g_wvp).xyww;
    
    return output;
}