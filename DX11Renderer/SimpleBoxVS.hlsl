
cbuffer MatrixBuffer
{
    matrix g_world;
    matrix g_wvp; //월드 x 뷰 x 프로젝션
    matrix g_worldInv;
};

struct VertexIn
{
    float3 position : POSITION;
    float2 tex : TEXCOORD0;
};

struct VertexOut
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};


VertexOut main(VertexIn input)
{
    VertexOut output;
    output.position = float4(input.position, 1.0f);
    output.position = mul(output.position, g_wvp);
    output.tex = input.tex;
    
    return output;
}