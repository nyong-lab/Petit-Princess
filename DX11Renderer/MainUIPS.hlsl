cbuffer window
{
	bool isExit;
	bool isCredit;
    float fade;
    float pad;
}

SamplerState samplerState;

Texture2D texBG			: register(t0); 
Texture2D texExit		: register(t1); 
Texture2D texCredit     : register(t2); 

struct PixelIn
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};

float4 main(PixelIn input) : SV_TARGET
{
	if(isExit) return texExit.Sample(samplerState, input.tex);
	if(isCredit) return texCredit.Sample(samplerState, input.tex);
	float4 color = texBG.Sample(samplerState, input.tex);
	color.a = fade;
	return color;
	//return texBG.Sample(samplerState, input.tex);

}