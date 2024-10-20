#include "BRDF.hlsli"
#include "Functions.hlsli"
#include "BloomUtils.hlsli"
#define SHADOW_BIAS 0.01f


struct PointLight
{
    float4 color;
    float3 position;
    float intensity;
    float3 attenuation;
    float range;
};

cbuffer DLightMaterial : register(b0)
{
    float4 gDirection;
    float3 gLightColor;
    float gDLightIntensity;
    float4 gEyePosW;
    float4 gAmbient;
    float4x4 gShadowTransform;
};

cbuffer PLightMaterial : register(b1)
{
    PointLight gPointLights[64];
    unsigned int gPCount;
}

cbuffer worldProperty : register(b2)
{
    float gBloomIntensity;
    float gBloomThreshold;
    float gTonemapThreshold;
};

Texture2D BaseColorTex      	: register(t0);
Texture2D normalTex		        : register(t1);
Texture2D worldPosDepth         : register(t2);
Texture2D RoughnessMetallicAO   : register(t3);
Texture2D shadowMap             : register(t4);

SamplerState gSampler : SAMPLER : register(s0);

struct PixelIn
{
    float4 pos : SV_POSITION;
    float2 Tex : TEXCOORD;
};

struct PixelOutt
{
    float4 original         : SV_Target0;
    float4 bloomSat         : SV_Target1;
};

PixelOutt main(PixelIn input)
{    
    PixelOutt output;

    float4 baseColor = BaseColorTex.Sample(gSampler, input.Tex);
    float4 normalOri = normalTex.Sample(gSampler, input.Tex);
    float roughness = RoughnessMetallicAO.Sample(gSampler, input.Tex).r;
    float metallic = RoughnessMetallicAO.Sample(gSampler, input.Tex).g;
    float AO = RoughnessMetallicAO.Sample(gSampler, input.Tex).b;
 
    if (length(normalOri) == 0.0f)
    {
        float ruma = rgb2luma(baseColor.rgb);
        output.original = float4(baseColor.x, baseColor.y, baseColor.z, ruma);
        output.bloomSat = float4(0,0,0,1);
        return output;
    }
        
    // 베이스 컬러를 리니어 공간으로 변환
    float3 linearcolor = GammaToLinear(baseColor.xyz);
    baseColor = float4(linearcolor, baseColor.w);    

    float3 normalW = normalOri.xyz;
    //normalW.y=1-normalW.y;
    normalW = (normalW * 2.0f) - 1.0f;
    normalW = normalize(normalW);
    float3 N = normalW;
    float3 worldPos = worldPosDepth.Sample(gSampler, input.Tex).xyz;
    // float3 depth = gDepth.Sample(mySamplerState, pin.Tex).xyz;
    float4 f4White = float4(1.0f, 1.0f, 1.0f, 1.0f);
    
    float4 litColor = f4White;
    
    float3 eyePosTemp = gEyePosW.xyz;
    // Light calculations

    float3 L = normalize(-gDirection.xyz);
    float3 R = reflect(-L, N);
    float3 toEye =  normalize(eyePosTemp - worldPos);

    float distToEye = length(toEye);
	// Normalize.
    toEye /= distToEye;
    float3 V = toEye;

    float NdotL = saturate(dot(normalW, L));
    float NdotV = saturate(dot(normalW, V));
    
    // 디렉셔널 라이트
    float3 color = LightSurface2(V, N, gLightColor, L, baseColor.rgb, gDLightIntensity, roughness, metallic, AO);
    
    // 포인트라이트
    for (int i = 0; i < gPCount; i++)
    {
        float3 lightVec = gPointLights[i].position - worldPos;
        float distance = length(lightVec);
        
        if (distance < gPointLights[i].range)
        {
            float3 pointl = LightSurface2(V, N, gPointLights[i].color.rgb, lightVec, baseColor.rgb, gPointLights[i].intensity, roughness, metallic, AO);
            float att = 1.0f / (gPointLights[i].attenuation.x + gPointLights[i].attenuation.y * distance + gPointLights[i].attenuation.z * distance * distance);
        
            pointl *= att;
            color += pointl;
        }
    }
    //aaaa
 // 그림자 맵에서 그림자를 확인
    float4 shadowMapPos = mul(float4(worldPos, 1.0f), gShadowTransform);
    float2 projectTexCoord;
    projectTexCoord.x =  shadowMapPos.x / shadowMapPos.w / 2.0f + 0.5f;
    projectTexCoord.y = -shadowMapPos.y / shadowMapPos.w / 2.0f + 0.5f;

    if (saturate(projectTexCoord.x) == projectTexCoord.x && saturate(projectTexCoord.y) == projectTexCoord.y)
    {
        float depthValue = shadowMap.Sample(gSampler, projectTexCoord).r;
        float lightDepthValue = shadowMapPos.z / shadowMapPos.w;
        lightDepthValue = lightDepthValue - SHADOW_BIAS;

        if (lightDepthValue > depthValue)
        {
            // 그림자가 있는 경우 그림자 색을 반영
            float4 shadowColor = float4(0.8f, 0.8f, 0.8f, 1.0f);
            shadowColor *= gDLightIntensity;
            color *= shadowColor;
            color = saturate(color);
        }
        else
        {
            color = saturate(color);
        }
    }
   
    // 환경광 이게 맞니? ...
     float3 amb = gAmbient.rgb * baseColor.rgb;
     amb *= gAmbient.w;
     color += amb;       
    
    //HDR -> LDR 톤맵핑
    float3 toneMappedGamma = LinearToGamma(color);
    //toneMappedGamma = ToneMap::ACES(toneMappedGamma);
    //toneMappedGamma = ToneMap::Reinhard_Jodie(toneMappedGamma);
    //toneMappedGamma = ToneMap::ExtendedReinhard(toneMappedGamma, gTonemapThreshold);
    //toneMappedGamma = ToneMap::FilmicALU(toneMappedGamma);
    //toneMappedGamma = ToneMap::Hejl2015(toneMappedGamma, gTonemapThreshold);
    
    // bloom 을 위한 밝기 추출
//    float3 toneMappedLinear = GammaToLinear(toneMappedGamma);
//    float brightness = 0.299 * toneMappedLinear.r
//                         +0.587 * toneMappedLinear.g
//                         +0.114 * toneMappedLinear.b;
    float brightness = 0.299 * color.r
                        +0.587 * color.g
                        +0.114 * color.b;    

    float blooms = GetBloomCurve(brightness, gBloomThreshold); //threshold
    float3 bloomColor = (color) * blooms;

    output.original = float4(toneMappedGamma, 1.f);
    output.bloomSat = float4(bloomColor,1);
    
    return output;
}