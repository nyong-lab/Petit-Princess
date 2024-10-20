cbuffer UIMove : register(b0)
{
    float2 UIcenter;
    float2 UIsize;
    int meshCount;
    float3 pad;
}

struct VertexIn
{
    float3 position : POSITION;
    float2 tex : TEXCOORD0;
};

struct VertexOut
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};

VertexOut main(VertexIn input)
{
    VertexOut output;    
    output.tex = input.tex;

    int screenWidth = 1920;
    int screenHeight = 1080;
    float2 centerpos, size;

    if (meshCount == 1)  // Background
    {
        centerpos = float2(screenWidth / 2.f, screenHeight / 2.f);
        size = float2(1500.f, 840.f);
    }
    else if (meshCount == 2)  // Food
    {   
        centerpos = float2(screenWidth/2, (screenHeight/2 -70.f));
        size = float2(1300/1.5, 700/1.5);
    }
    else if (meshCount == 3)
    {
        centerpos = UIcenter;
        size = UIsize;
    }
    else
    {
        int keyChainStride = 130;
        centerpos = UIcenter;
        centerpos.x += ((meshCount - 3) * keyChainStride);
        size = UIsize;
    }

    float2 UIpos = float2(centerpos.x + (input.position.x * (size.x / 2)), centerpos.y + (input.position.y * (size.y / 2)));

    // 화면 좌표를 NDC로 변환
    float normalizedPosX = (2.0 * UIpos.x) / screenWidth - 1.0;
    float normalizedPosY = 1.0 - (2.0 * UIpos.y) / screenHeight;

    output.position = float4(normalizedPosX, normalizedPosY, 0.f, 1.f);
    
    return output;
}