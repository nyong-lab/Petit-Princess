//--------------------------------------------------------------------------------
//                         가우시안 필터링
//--------------------------------------------------------------------------------





//--------------------------------------------------------------------------------
//                         가우시안 블러
//--------------------------------------------------------------------------------

#define POSTEFFECT_BLUR_SAMPLING_COUNT 9

// X 방향 Gaussian Blur
// .x = width, .y = height, .z = 1/width, .w = 1/height
float4 psBlur_X(float2 texCoord, Texture2D tex, SamplerState SamplerSrc0, float4 vScreenSize)
{
    float weights[POSTEFFECT_BLUR_SAMPLING_COUNT] =
    {
        0.013519569015984728,
        0.047662179108871855,
        0.11723004402070096,
        0.20116755999375591,
        0.240841295721373,
        0.20116755999375591,
        0.11723004402070096,
        0.047662179108871855,
        0.013519569015984728
    };

    float indices[POSTEFFECT_BLUR_SAMPLING_COUNT] = { -4, -3, -2, -1, 0, +1, +2, +3, +4 };
    float2 dir = float2(1.0, 0.0);
    
    float2 step = float2(dir.x * vScreenSize.z, dir.y * vScreenSize.w);
    float4 Result = 0.0f;
    
    for (int i = 0; i < POSTEFFECT_BLUR_SAMPLING_COUNT; i++)
    {
        Result += tex.Sample(SamplerSrc0, float2(texCoord + indices[i] * step)) * weights[i];
    }
    
    return float4(Result.rgb, 1);
}

// Y 방향 Gaussian Blur
float4 psBlur_Y(float2 texCoord, Texture2D tex, SamplerState SamplerSrc0, float4 vScreenSize)
{
    float weights[POSTEFFECT_BLUR_SAMPLING_COUNT] =
    {
        0.013519569015984728,
        0.047662179108871855,
        0.11723004402070096,
        0.20116755999375591,
        0.240841295721373,
        0.20116755999375591,
        0.11723004402070096,
        0.047662179108871855,
        0.013519569015984728
    };

    float indices[POSTEFFECT_BLUR_SAMPLING_COUNT] = { -4, -3, -2, -1, 0, +1, +2, +3, +4 };
    float2 dir = float2(0.0, 1.0);
    
    float2 step = float2(dir.x * vScreenSize.z, dir.y * vScreenSize.w);
    float4 Result = 0.0f;
    
    for (int i = 0; i < POSTEFFECT_BLUR_SAMPLING_COUNT; i++)
    {
        Result += tex.Sample(SamplerSrc0, float2(texCoord + indices[i] * step)) * weights[i];
    }
    
    return float4(Result.rgb, 1);
}


//--------------------------------------------------------------------------------
//                               블룸 커브
//--------------------------------------------------------------------------------
//#define BLOOMCURVE_METHOD_1
//#define BLOOMCURVE_METHOD_2
#define BLOOMCURVE_METHOD_3

float GetBloomCurve(float x, float threshold)
{
    float result = x;
    x *= 2.0f;
    
#ifdef BLOOMCURVE_METHOD_1
    result = x*0.05+max(0,x-threshold)*0.5; // default threshold = 1.26
#endif
    
#ifdef BLOOMCURVE_METHOD_2
    result = x*x/3.2;
#endif
    
#ifdef BLOOMCURVE_METHOD_3
    result = max(0,x-threshold); // default threshold = 1.0
    result *= result;
#endif 
    
    return result * 0.5f;
}
