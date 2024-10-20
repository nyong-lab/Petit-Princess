struct VertexIn
{
    float3 PosL : POSITION;
    float2 Tex : TEXCOORD;
};

struct VertexOut
{
    float4 pos : SV_POSITION;
    float2 Tex : TEXCOORD;
};

cbuffer downSampleSize
{
    int gScale;
};

VertexOut main(VertexIn _vin)
{
    VertexOut vout;
    
    _vin.PosL.x = (_vin.PosL.x - (gScale - 1)) / gScale;
    _vin.PosL.y = (_vin.PosL.y + (gScale - 1)) / gScale;
    
    vout.pos = float4(_vin.PosL, 1.0f);
    vout.Tex = _vin.Tex;

    return vout;
}