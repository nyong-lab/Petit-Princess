struct VertexIn
{
    float3 PosL     : POSITION;
    float2 Tex      : TEXCOORD;
};

struct VertexOut
{
    float4 pos      : SV_POSITION;
    float2 Tex      : TEXCOORD;
};

VertexOut main(VertexIn _vin)
{
    VertexOut vout;
    vout.pos = float4(_vin.PosL, 1.0f);
    vout.Tex = _vin.Tex;

    return vout;
}