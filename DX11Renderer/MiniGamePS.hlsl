cbuffer MiniGameProgress : register(b0)
{
    int foodtype;
    int gametype;
    int stage;
    int progress;
    int meshCount;
    int direction;
    float2 pad;
};

// 진짜 도대체 배열 왜안돼.........?
// 너무 맘에 안들지만 어쩔 수 없다...
cbuffer MiniGameKeyChain : register(b1)
{
    int key1;
    int key2;
    int key3;
    int key4;
    int key5;
    float3 pad2;
}

struct PixelIn
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};

SamplerState samplerState;

Texture2D firstTex : register(t0);      // Point
Texture2D secondTex : register(t1);     // Drag
Texture2D thirdTex : register(t2);      // KeyChain
Texture2D fourthTex : register(t3);     // Wheel

float4 main(PixelIn input) : SV_TARGET
{
    float4 texColor;
    float spriteWidth, spriteHeight;

    if(meshCount == 1)  /// Background
    {
        spriteWidth = 1.0f / 4.0f;
        spriteHeight = 1.0f;

        // 텍스처 UV 범위 계산
        float2 spriteUV;
        spriteUV.x = (spriteWidth * input.tex.x) + (spriteWidth * gametype);
        spriteUV.y = (spriteHeight * input.tex.y) + spriteHeight;
            
        texColor = firstTex.Sample(samplerState, spriteUV);
    }
    else if(meshCount == 2)  /// Food
    {
        // 스프라이트에 맞춰 UV 나누기
        spriteWidth = 1.0f / 5.0f;
        spriteHeight = (foodtype < 8) ? 1.0f / 8.0f : 1.0f / 7.0f;
        
        // 텍스처 UV 범위 계산
        float2 spriteUV;
        spriteUV.x = (spriteWidth * input.tex.x) + (spriteWidth * (stage - 1));
        spriteUV.y = foodtype < 8 ? (spriteHeight * input.tex.y) + (spriteHeight * foodtype) : (spriteHeight * input.tex.y) + (spriteHeight * (foodtype - 8));
        
        // 텍스처 샘플링
        texColor = (foodtype < 8) ? firstTex.Sample(samplerState, spriteUV) : secondTex.Sample(samplerState, spriteUV);
    }
    else if(meshCount == 3)
    {
        /// Point ------------------------------------------------------------------------
        if(gametype == 0) 
        {
            spriteWidth = 1.0f / 4.0f;
            spriteHeight = 1.0f;
            
            // 텍스처 UV 범위 계산
            float2 spriteUV;
            spriteUV.x = (spriteWidth * input.tex.x) + (spriteWidth * (progress - 1));
            spriteUV.y = (spriteHeight * input.tex.y) + spriteHeight;
            
            texColor = firstTex.Sample(samplerState, spriteUV);
        }
        /// ------------------------------------------------------------------------------
        
        /// Drag -------------------------------------------------------------------------
        else if(gametype == 1)
        {
            spriteWidth = 1.0f / 8.0f;
            spriteHeight = 1.0f;
            
            // 텍스처 UV 범위 계산
            float2 spriteUV;
            spriteUV.x = (spriteWidth * input.tex.x) + (spriteWidth * direction);
            spriteUV.y = (spriteHeight * input.tex.y) + spriteHeight;
            
            texColor = secondTex.Sample(samplerState, spriteUV);
        }
        /// ------------------------------------------------------------------------------

        /// KeyChain ---------------------------------------------------------------------
        else if(gametype == 2)
        {
            if (key1 == 2)
                discard;
            
            spriteWidth = 1.0f / 2.0f;
            spriteHeight = 1.0f;
           
            // 텍스처 UV 범위 계산
            float2 spriteUV;
            spriteUV.x = (spriteWidth * input.tex.x) + (spriteWidth * key1);
            spriteUV.y = (spriteHeight * input.tex.y) + spriteHeight;
            
            texColor = thirdTex.Sample(samplerState, spriteUV);
        }
        /// ------------------------------------------------------------------------------

        /// Wheel ------------------------------------------------------------------------
        else if(gametype == 3)
        {
            spriteWidth = 1.0f / 2.0f;
            spriteHeight = 1.0f;
                    
            // 텍스처 UV 범위 계산
            float2 spriteUV;
            spriteUV.x = (spriteWidth * input.tex.x) + spriteWidth;
            spriteUV.y = (spriteHeight * input.tex.y) + spriteHeight;
            
            texColor = fourthTex.Sample(samplerState, spriteUV);
        }
        /// ------------------------------------------------------------------------------

    }
    else if(meshCount > 3)
    {
        /// KeyChain ---------------------------------------------------------------------
        if (gametype == 2)
        {
            spriteWidth = 1.0f / 2.0f;
            spriteHeight = 1.0f;
           
            int value = 2;
            switch (meshCount)
            {
                case 4:
                    value = key2;
                    break;
                case 5:
                    value = key3;
                    break;
                case 6:
                    value = key4;
                    break;
                case 7:
                    value = key5;
                    break;
                default:
                    break;
            }

            if (value == 2)
                discard;
            
            // 텍스처 UV 범위 계산
            float2 spriteUV;
            spriteUV.x = (spriteWidth * input.tex.x) + (spriteWidth * value);
            spriteUV.y = (spriteHeight * input.tex.y) + spriteHeight;
            
            texColor = thirdTex.Sample(samplerState, spriteUV);
        }
        else
        {
            discard;
        }
        /// ------------------------------------------------------------------------------
    }

    return texColor;
}