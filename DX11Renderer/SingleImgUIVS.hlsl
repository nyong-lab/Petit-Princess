cbuffer UIInfo : register(b0)
{
    float2 UIcenter;
    float2 UIsize;
    float4 pad; //귀찮으니 다른 상수버퍼 재활용..
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
};

PixelIn main(VertexIn input)
{
    PixelIn output;
   
    int screenWidth = 1920;
    int screenHeight = 1080;

    float2 UIpos = float2(UIcenter.x + (input.position.x * (UIsize.x / 2)),
                          UIcenter.y + (input.position.y * (UIsize.y / 2)));
    
    // 화면 좌표를 NDC로 변환
    float normalizedPosX = (2.0 * UIpos.x) / screenWidth - 1.0;
    float normalizedPosY = 1.0 - (2.0 * UIpos.y) / screenHeight;
    
    output.position = float4(normalizedPosX, normalizedPosY, 0.f, 1.f);
    output.tex = input.tex;

    return output;
}