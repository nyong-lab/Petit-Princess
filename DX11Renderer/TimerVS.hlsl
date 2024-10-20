// �� A�� �������� �� B�� �߽����� ���� angle��ŭ ȸ����Ű�� �Լ�
float2 rotatePoint(float2 target, float2 pivot, double angle)
{
    float2 result;
    double rad = angle * (3.141592 / 180.0); // ������ �������� ��ȯ

    // B�� �������� �̵�
    double translatedX = target.x - pivot.x;
    double translatedY = target.y - pivot.y;

    // ȸ�� ��ȯ ����
    result.x = translatedX * cos(rad) - translatedY * sin(rad);
    result.y = translatedX * sin(rad) + translatedY * cos(rad);

    // �ٽ� B�� ��ġ�� �̵�
    result.x += pivot.x;
    result.y += pivot.y;

    return result;
}

cbuffer UIMove : register(b0)
{
    float2 UIcenter;
    float2 UIsize;
    float rotation;  // �¾��� ȸ��
}

struct VertexIn
{
    float3 position : POSITION;
    float2 tex : TEXCOORD0;
};

struct PixelIn
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float2 suntex : TEXCOORD1;
};

PixelIn main(VertexIn input)
{
    PixelIn output;
   
    int screenWidth = 1920;
    int screenHeight = 1080;

    float2 UIpos = float2(UIcenter.x + (input.position.x * (UIsize.x / 2)),
                          UIcenter.y + (input.position.y * (UIsize.y / 2)));
    
    // ȭ�� ��ǥ�� NDC�� ��ȯ
    float normalizedPosX = (2.0 * UIpos.x) / screenWidth - 1.0;
    float normalizedPosY = 1.0 - (2.0 * UIpos.y) / screenHeight;
    
    output.position = float4(normalizedPosX, normalizedPosY, 0.f, 1.f);
    output.tex = input.tex;
    
    // �¾��� �̹����� ȸ�� ���Ѿ� �Ѵ�.
    float2 mathUV = float2(input.tex.x, -input.tex.y);  // uv ������ ������ �������� ��ȯ
    float2 rotMatUV = rotatePoint(mathUV, float2(0.5,-0.5), rotation);
    output.suntex = float2(rotMatUV.x, -rotMatUV.y);    // ������ ������ �ٽ� uv �������� ��ȯ;

    return output;
}