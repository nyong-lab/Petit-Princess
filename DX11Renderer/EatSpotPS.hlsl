cbuffer EatSpotList : register(b0)
{
    bool meat;
    bool crab;
    bool apple;
    bool cupcake;
    bool baguette;
    int meshCount;
    float2 pad2;
}

struct PixelIn
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};

SamplerState samplerState;
Texture2D emptyTex : register(t0);
Texture2D fillTex : register(t1);

float4 main(PixelIn input) : SV_TARGET
{
    float4 texColor;
	float spriteWidth = 1.0f / 5.0f;
    float spriteHeight = 1.0f;

    // 상수버퍼 배열 대체 왜안되는거야
    // 진짜 짜증남...
    int foodtype;
    switch(meshCount)
    {
        case 1: foodtype = 0; break;
        case 2: foodtype = 1; break;
        case 3: foodtype = 2; break;
        case 4: foodtype = 3; break;
        case 5: foodtype = 4; break;
    }

    bool isEat;
    switch (foodtype)
    {
        case 0:
            isEat = (meat == true) ? true : false;
        break;
        case 1:
            isEat = (crab == true) ? true : false;
        break;
        case 2:
            isEat = (apple == true) ? true : false;
        break;
        case 3:
            isEat = (cupcake == true) ? true : false;            
        break;
        case 4:
            isEat = (baguette == true) ? true : false;            
        break;
    }

    // 텍스처 UV 범위 계산
    float2 spriteUV;
    spriteUV.x = (spriteWidth * input.tex.x) + (spriteWidth * foodtype);
    spriteUV.y = (spriteHeight * input.tex.y) + spriteHeight;

    if(isEat == false) texColor = emptyTex.Sample(samplerState, spriteUV);
    else texColor = fillTex.Sample(samplerState, spriteUV);
    
	return texColor;
}