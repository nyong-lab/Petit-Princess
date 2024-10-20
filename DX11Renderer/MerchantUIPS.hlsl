Texture2D meat : register(t0); // ������ ��� ��� ���ĵ�
Texture2D apple : register(t1);
Texture2D bagguett : register(t2);
Texture2D crab : register(t3);
Texture2D cupcake : register(t4);

Texture2D cooltimeBG : register(t5); // ��湮 ��Ÿ��
Texture2D cooltimeGauge : register(t6);

Texture2D senseBG : register(t7); // ��ġ������
Texture2D senseGauge : register(t8);
Texture2D senceDot : register(t9);
Texture2D senceSusp : register(t10);
Texture2D senceDetect : register(t11);

Texture2D bubbleBG : register(t12); // ������ ��� ��� ���
Texture2D bubbleDisabled : register(t13); // ������ ��� ��湮 ��Ÿ�� ��� �̰� ����

Texture2D maskTex : register(t14); // ����ũ

SamplerState samplerState;

struct PixelIn
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};

cbuffer Infos
{
    int state; // ������ �����? 
    // 0 : �ƹ��͵� ����� �ʰڴ� 
    // 1 : ���� �غ� 
    // 2 : �ٸ��� ����
    // 3 : ��湮 ��Ÿ��
    // 4 : ��ġ������
    // 5 : ����
    
    int foodtype; // ���� ����
    // 0 : ���
    // 1 : ���(����)
    // 2 : �ٰ�Ʈ(��)
    // 3 : ��(�ػ깰)
    // 4 : ������(����Ʈ)
    
    float ratio; // ������ ȯ�� ( 0 ~ 1 )
}


float4 main(PixelIn input) : SV_TARGET
{
    float4 mask;
    float4 gauge;
    float4 bg;
    
    switch (state)
    {
  //-------------------------------------------------------------------------------------        
        case 0: // �ƹ��͵� ����� �ʰڴ� 
            return float4(1.f, 1.f, 1.f, 0.f);
            
 //-------------------------------------------------------------------------------------         
        case 1: // ���� �غ�     
            bg = bubbleBG.Sample(samplerState, input.tex);
            float4 food;
            switch (foodtype)
            {
                case 0: //���
                    food = meat.Sample(samplerState, input.tex);
                    break;
                case 1: //����
                    food = apple.Sample(samplerState, input.tex);
                    break;
                case 2: // ��
                    food = bagguett.Sample(samplerState, input.tex);
                    break;
                case 3: // �ػ깰
                    food = crab.Sample(samplerState, input.tex);
                    break;
                case 4: // ������
                    food = cupcake.Sample(samplerState, input.tex);
                    break;
            }       
            float3 yammy = food.rgb * food.a + bg.rgb * (1 - food.a);
            
            return float4(yammy, food.a + bg.a);
            
 //-------------------------------------------------------------------------------------       
        case 2: // �ٸ��� ����
            return bubbleDisabled.Sample(samplerState, input.tex);

 //-------------------------------------------------------------------------------------         
        case 3: // ��湮 ��Ÿ��
            bg = cooltimeBG.Sample(samplerState, input.tex);
            gauge = cooltimeGauge.Sample(samplerState, input.tex);
            mask = maskTex.Sample(samplerState, input.tex);
        
            if( ratio > mask.a ) gauge.a = 0.f;
            float3 vistColor = gauge.rgb * gauge.a + bg.rgb * (1 - gauge.a);
            
            return float4(vistColor, bg.a + gauge.a);
 //-------------------------------------------------------------------------------------         
        case 4: // ��ġ������
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
        case 5: //����
            return senceDetect.Sample(samplerState, input.tex);
    }
 //-------------------------------------------------------------------------------------     
    return float4(1, 1, 0, 1); // ����
}