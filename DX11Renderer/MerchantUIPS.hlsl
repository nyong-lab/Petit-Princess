Texture2D meat : register(t0); // 맛집인 경우 띄울 음식들
Texture2D apple : register(t1);
Texture2D bagguett : register(t2);
Texture2D crab : register(t3);
Texture2D cupcake : register(t4);

Texture2D cooltimeBG : register(t5); // 재방문 쿨타임
Texture2D cooltimeGauge : register(t6);

Texture2D senseBG : register(t7); // 눈치게이지
Texture2D senseGauge : register(t8);
Texture2D senceDot : register(t9);
Texture2D senceSusp : register(t10);
Texture2D senceDetect : register(t11);

Texture2D bubbleBG : register(t12); // 맛집일 경우 띄울 배경
Texture2D bubbleDisabled : register(t13); // 맛집일 경우 재방문 쿨타임 대신 이걸 띄운다

Texture2D maskTex : register(t14); // 마스크

SamplerState samplerState;

struct PixelIn
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};

cbuffer Infos
{
    int state; // 무엇을 띄울지? 
    // 0 : 아무것도 띄우지 않겠다 
    // 1 : 맛집 준비 
    // 2 : 다먹은 맛집
    // 3 : 재방문 쿨타임
    // 4 : 눈치게이지
    // 5 : 적발
    
    int foodtype; // 맛집 종류
    // 0 : 고기
    // 1 : 사과(과일)
    // 2 : 바게트(빵)
    // 3 : 게(해산물)
    // 4 : 컵케익(디저트)
    
    float ratio; // 게이지 환산 ( 0 ~ 1 )
}


float4 main(PixelIn input) : SV_TARGET
{
    float4 mask;
    float4 gauge;
    float4 bg;
    
    switch (state)
    {
  //-------------------------------------------------------------------------------------        
        case 0: // 아무것도 띄우지 않겠다 
            return float4(1.f, 1.f, 1.f, 0.f);
            
 //-------------------------------------------------------------------------------------         
        case 1: // 맛집 준비     
            bg = bubbleBG.Sample(samplerState, input.tex);
            float4 food;
            switch (foodtype)
            {
                case 0: //고기
                    food = meat.Sample(samplerState, input.tex);
                    break;
                case 1: //과일
                    food = apple.Sample(samplerState, input.tex);
                    break;
                case 2: // 빵
                    food = bagguett.Sample(samplerState, input.tex);
                    break;
                case 3: // 해산물
                    food = crab.Sample(samplerState, input.tex);
                    break;
                case 4: // 컵케익
                    food = cupcake.Sample(samplerState, input.tex);
                    break;
            }       
            float3 yammy = food.rgb * food.a + bg.rgb * (1 - food.a);
            
            return float4(yammy, food.a + bg.a);
            
 //-------------------------------------------------------------------------------------       
        case 2: // 다먹은 맛집
            return bubbleDisabled.Sample(samplerState, input.tex);

 //-------------------------------------------------------------------------------------         
        case 3: // 재방문 쿨타임
            bg = cooltimeBG.Sample(samplerState, input.tex);
            gauge = cooltimeGauge.Sample(samplerState, input.tex);
            mask = maskTex.Sample(samplerState, input.tex);
        
            if( ratio > mask.a ) gauge.a = 0.f;
            float3 vistColor = gauge.rgb * gauge.a + bg.rgb * (1 - gauge.a);
            
            return float4(vistColor, bg.a + gauge.a);
 //-------------------------------------------------------------------------------------         
        case 4: // 눈치게이지
            bg = senseBG.Sample(samplerState, input.tex);
            //bg = float4(1,1,1,0);
            gauge = senseGauge.Sample(samplerState, input.tex);
            mask = maskTex.Sample(samplerState, input.tex);
            
            float4 center;
            if (ratio > 0.5)
                center = senceDot.Sample(samplerState, input.tex);
            else
                center = senceSusp.Sample(samplerState, input.tex);     
        
            if( ratio > mask.a ) gauge.a = 0.f;
            float3 senceColor = gauge.rgb * gauge.a + center.rgb * (1-gauge.a);
            float tmpa = gauge.a + center.a;
            senceColor = senceColor.rgb * tmpa + bg.rgb * (1-tmpa);         

            return float4(senceColor, bg.a + gauge.a + center.a);
 //-------------------------------------------------------------------------------------        
        case 5: //적발
            return senceDetect.Sample(samplerState, input.tex);
    }
 //-------------------------------------------------------------------------------------     
    return float4(1, 1, 0, 1); // 에러
}