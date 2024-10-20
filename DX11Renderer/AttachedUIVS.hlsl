cbuffer MatrixBuffer : register(b0)
{
    matrix g_world;
    matrix g_wvp; 
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
    float3 pos = input.position + float3(0, 0, 1);
    output.position = mul(float4(pos, 1.0f), g_world);
    output.position = mul(float4(output.position), g_wvp);
    output.tex = input.tex;

    return output;
}