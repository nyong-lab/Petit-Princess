// 점 A를 기준으로 점 B를 중심으로 각도 angle만큼 회전시키는 함수
float2 rotatePoint(float2 target, float2 pivot, double angle)
{
    float2 result;
    double rad = angle * (3.141592 / 180.0); // 각도를 라디안으로 변환

    // B를 원점으로 이동
    double translatedX = target.x - pivot.x;
    double translatedY = target.y - pivot.y;

    // 회전 변환 수행
    result.x = translatedX * cos(rad) - translatedY * sin(rad);
    result.y = translatedX * sin(rad) + translatedY * cos(rad);

    // 다시 B의 위치로 이동
    result.x += pivot.x;
    result.y += pivot.y;

    return result;
}

cbuffer UIMove : register(b0)
{
    float2 UIcenter;
    float2 UIsize;
    float rotation;  // 태양의 회전
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
    
    // 화면 좌표를 NDC로 변환
    float normalizedPosX = (2.0 * UIpos.x) / screenWidth - 1.0;
    float normalizedPosY = 1.0 - (2.0 * UIpos.y) / screenHeight;
    
    output.position = float4(normalizedPosX, normalizedPosY, 0.f, 1.f);
    output.tex = input.tex;
    
    // 태양은 이미지를 회전 시켜야 한다.
    float2 mathUV = float2(input.tex.x, -input.tex.y);  // uv 공간을 수학적 공간으로 변환
    float2 rotMatUV = rotatePoint(mathUV, float2(0.5,-0.5), rotation);
    output.suntex = float2(rotMatUV.x, -rotMatUV.y);    // 수학적 공간을 다시 uv 공간으로 변환;

    return output;
}