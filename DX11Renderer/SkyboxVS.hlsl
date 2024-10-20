cbuffer MatrixBuffer
{
    matrix g_world;
    matrix g_wvp; //월드 x 뷰 x 프로젝션
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
    
    // z/w = 1 이 되도록(즉 하늘 돔이 항상 먼 평면에 있도록) z = w로 설정한다. 
    // Set Pos to xyww instead of xyzw, so that z will always be 1 (furthest from camera)
    //output.position = mul(g_wvp, float4(pos, 1.0f)).xyww;
    output.tex = pos;
    output.position = mul(float4(pos, 1.0f), g_wvp).xyww;
    
    return output;
}