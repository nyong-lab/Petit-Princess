
struct VertexIn
{
    float3 position : POSITION;
    float2 tex : TEXCOORD0;
};

struct PixelIn
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};

PixelIn main(VertexIn input)
{
    PixelIn output;
    
    float2 UIcenter = float2(1920 / 2, 1080 / 2);
    float2 UIsize = float2(1920, 1080);
    int screenWidth = 1920;
    int screenHeight = 1080;
    
    float2 UIpos = float2(UIcenter.x + (input.position.x * (UIsize.x / 2)),
                          UIcenter.y + (input.position.y * (UIsize.y / 2)));
    
    // ȭ�� ��ǥ�� NDC�� ��ȯ
    float normalizedPosX = (2.0 * UIpos.x) / screenWidth - 1.0;
    float normalizedPosY = 1.0 - (2.0 * UIpos.y) / screenHeight;
    
    output.position = float4(normalizedPosX, normalizedPosY, 0.f, 1.f);
    output.tex = input.tex;
    
    return output;
}