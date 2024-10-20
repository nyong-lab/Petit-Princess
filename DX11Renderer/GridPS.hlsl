

cbuffer CBuf
{
    float4 face_colors[6];
};


float4 main(uint tid : SV_PrimitiveID) : SV_TARGET //이렇게 하면 각 삼각형 마다 고유의 id를 가진다 ?
{
    return float4(1, 1, 1, 1);
}