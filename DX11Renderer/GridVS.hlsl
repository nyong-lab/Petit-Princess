cbuffer MatrixBuffer
{
    matrix g_world;
    matrix g_wvp; //���� x �� x ��������
    matrix g_worldInv;
    //Material g_material;
};

float4 main(float4 pos : POSITION) : SV_POSITION
{
    float4 outputPos;
    outputPos = mul(pos, g_wvp);
    return outputPos;
}