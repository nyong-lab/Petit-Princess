

//FXAA를 처리하는 곳에서 픽셀의 RGB는 Linear한 공간에 있어야 하지만,
//Luminance가 저장된 Alpha값은 감마 공간에 있어야 합니다.
// 감마 컬렉션이 적용된 luminance
float rgb2luma(float3 rgb)
{
    return sqrt(dot(rgb, float3(0.299, 0.587, 0.114)));
}

static const float INV_GAMMA = 1.0 / 2.2;
static const float GAMMA = 2.2;

float3 GammaToLinear(float3 color)
{
    return pow(color, float3(INV_GAMMA, INV_GAMMA, INV_GAMMA));
}

float3 LinearToGamma(float3 color)
{
    return pow(color, float3(GAMMA, GAMMA, GAMMA));
}

//--------------------------------------------------------------------------------------------------------------------------
//                           톤 맵핑
//--------------------------------------------------------------------------------------------------------------------------
namespace ToneMap
{
    float3 Multiply(const float3 m[3], const float3 v)
    {
        float x = m[0][0] * v[0] + m[0][1] * v[1] + m[0][2] * v[2];
        float y = m[1][0] * v[1] + m[1][1] * v[1] + m[1][2] * v[2];
        float z = m[2][0] * v[1] + m[2][1] * v[1] + m[2][2] * v[2];
        return float3(x, y, z);
    }

    float3 rtt_and_odt_fit(float3 v)
    {
        float3 a = v * (v + 0.0245786f) - 0.000090537f;
        float3 b = v * (0.983729f * v + 0.4329510f) + 0.238081f;
        return a / b;
    }

// sRGB => XYZ => D65_2_D60 => AP1 => RRT_SAT
    static float3 aces_input_matrix[3] = { float3(0.59719f, 0.35458f, 0.04823f), float3(0.07600f, 0.90834f, 0.01566f), float3(0.02840f, 0.13383f, 0.83777f) };
// ODT_SAT => XYZ => D60_2_D65 => sRGB
    static float3 aces_output_matrix[3] = { float3(1.60475f, -0.53108f, -0.07367f), float3(-0.10208f, 1.10813f, -0.00605f), float3(-0.00327f, -0.07276f, 1.07602f) };

    float luminance(float3 v)
    {
        return dot(v, float3(0.2126f, 0.7152f, 0.0722f));
    }

    float3 change_luminance(float3 c_in, float l_out)
    {
        float l_in = luminance(c_in);
        return c_in * (l_out / l_in);
    }
    
    float3 LinearTosRGB(in float3 color)
    {
        float3 x = color * 12.92f;
        float3 y = 1.055f * pow(saturate(color), 1.0f / 2.4f) - 0.055f;

        float3 clr = color;
        clr.r = color.r < 0.0031308f ? x.r : y.r;
        clr.g = color.g < 0.0031308f ? x.g : y.g;
        clr.b = color.b < 0.0031308f ? x.b : y.b;

        return clr;
    }

    float3 SRGBToLinear(in float3 color)
    {
        float3 x = color / 12.92f;
        float3 y = pow(max((color + 0.055f) / 1.055f, 0.0f), 2.4f);

        float3 clr = color;
        clr.r = color.r <= 0.04045f ? x.r : y.r;
        clr.g = color.g <= 0.04045f ? x.g : y.g;
        clr.b = color.b <= 0.04045f ? x.b : y.b;

        return clr;
    }
    
    // ---------------------------------------------------------------------------------
    
    float3 ACES(float3 colorIn)
    {
        colorIn = Multiply(aces_input_matrix, colorIn);
        colorIn = rtt_and_odt_fit(colorIn);
        colorIn = Multiply(aces_output_matrix, colorIn);
    
        return saturate(colorIn);
    }
    
    float3 ExtendedReinhard(float3 v, float max_white_l)
    {
        float l_old = luminance(v);
        float numerator = l_old * (1.0f + (l_old / (max_white_l * max_white_l)));
        float l_new = numerator / (1.0f + l_old);
        return change_luminance(v, l_new);
    }
    
    float3 Reinhard_Jodie(float3 v)
    {
        float l = luminance(v);
        float3 tv = v / (1.0f + v);
        return lerp(v / (1.0f + l), tv, tv);
    }
    
    // Applies the filmic curve from John Hable's presentation
    float3 FilmicALU(in float3 color)
    {
        color = max(0, color - 0.004f);
        color = (color * (6.2f * color + 0.5f)) / (color * (6.2f * color + 1.7f) + 0.06f);
        return color;
    }

    float3 Hejl2015(in float3 hdr, float WhitePoint_Hejl)
    {
        float4 vh = float4(hdr, WhitePoint_Hejl);
        float4 va = (1.435f * vh) + 0.05;
        float4 vf = ((vh * va + 0.004f) / ((vh * (va + 0.55f) + 0.0491f))) - 0.0821f;
        return LinearTosRGB(vf.xyz / vf.www);
    }
}
