

SamplerState samplerState;
Texture2D tex : register(t0); 

struct PixelIn
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};


float4 main(PixelIn input) : SV_TARGET
{
	return tex.Sample(samplerState, input.tex);
}