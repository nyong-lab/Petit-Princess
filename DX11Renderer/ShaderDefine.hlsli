/// ----------------------------------
///           VERTEX IN/OUT
/// ----------------------------------

struct VertexIn
{
    float3 PosL             : POSITION;
    float3 NormalL          : NORMAL;
    float3 Binormal         : BINORMAL;
    float3 Tangent          : TANGENT;
    float2 Tex              : TEXCOORD;
};

struct SkinnedVertexIn
{
    float3 PosL             : POSITION;
    float3 NormalL          : NORMAL;
    float3 Binormal         : BINORMAL;
    float3 Tangent          : TANGENT;
    float2 Tex              : TEXCOORD;
    float4 Weights          : WEIGHTS;
    int4 Indices            : INDICES;
};

struct VertexOut
{
    float4 PosH             : SV_POSITION;
    float3 PosW             : POSITION;
    float3 NormalW          : NORMAL;
    float3 Tangent          : TANGENT;
    float3 Binormal         : BINORMAL;
    float2 Tex              : TEXCOORD;
};

struct ShadowVertexOut
{
    float4 position : SV_POSITION;
};


/// ----------------------------------
///            PIXEL IN/OUT
/// ----------------------------------

struct PixelIn
{
    float4 PosH             : SV_POSITION;
    float3 PosW             : POSITION;
    float3 NormalW          : NORMAL;
    float3 Tangent          : TANGENT;
    float3 Binormal         : BINORMAL;
    float2 Tex              : TEXCOORD;
};

struct PixelOut
{
    float4 texBaseColor         : SV_Target0;
    float4 texNormal            : SV_Target1;
    float4 worldPosition        : SV_Target2;
    float4 roughnessMetallicAO  : SV_Target3;
};



