

cbuffer CBuf
{
    float4 face_colors[6];
};


float4 main(uint tid : SV_PrimitiveID) : SV_TARGET //�̷��� �ϸ� �� �ﰢ�� ���� ������ id�� ������ ?
{
    return float4(1, 1, 1, 1);
}