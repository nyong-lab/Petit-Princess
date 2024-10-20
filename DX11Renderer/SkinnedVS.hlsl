cbuffer MatrixBuffer : register(b0)
{
    matrix g_world;
    matrix g_view;
    matrix g_proj;
    matrix g_worldInv;
};

cbuffer cbBonePalette : register(b1)
{
    matrix g_boneMat[128];
}

struct VertexIn
{
    float3 PosL : POSITION;
    float3 Normal : NORMAL;
    float2 UV : TEXCOORD0;
    float4 Weights : WEIGHTS;
    int4 Indices : INDICES;
};

struct VertexOut
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITION;
    float2 UV : TEXCOORD0;
};

VertexOut main(VertexIn input)
{
    VertexOut Output;

    matrix combindedMatrix = mul(input.Weights.x, g_boneMat[input.Indices.x]);;
    for (int i = 0; i < 4; ++i)
    {
        if (input.Indices[i] == -1)
        {
            break;
        }

        //combindedMatrix += mul(input.Weights[i], g_boneMat[input.Indices[i]]);
    }
	
    Output.PosW = mul(float4(input.PosL, 1.f), mul(combindedMatrix, g_world)).xyz;
	
    Output.PosH = mul(float4(input.PosL, 1.f), mul(combindedMatrix, g_world));
    //Output.PosH = mul(Output.PosH, g_view);
    //Output.PosH = mul(Output.PosH, g_proj);
    Output.UV = input.UV;

    return Output;
}


