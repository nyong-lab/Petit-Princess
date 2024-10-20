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

    float screenWidth = 1920.f;
    float screenHeight = 1080.f;
    
    // 그냥 수동으로 좌표 넣기
    float2 centerpos = float2(1200.f, 1000.f); 
    float stride = 105.f; 
    centerpos.x += stride * (meshCount-1);

    float2 UIsize = (90.f, 90.f);
    float2 UIpos = float2(centerpos.x + (input.position.x * (UIsize.x / 2)), centerpos.y + (input.position.y * (UIsize.y / 2)));

    // 화면 좌표를 NDC로 변환
    float normalizedPosX = (2.0 * UIpos.x) / screenWidth - 1.0;
    float normalizedPosY = 1.0 - (2.0 * UIpos.y) / screenHeight;    
    
    output.position = float4(normalizedPosX, normalizedPosY, 0.f, 1.f);

	return output;
}