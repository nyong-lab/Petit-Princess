#include "ShaderDefine.hlsli"

float4 main(ShadowVertexOut pin) : SV_TARGET
{
	float depthValue;
	float4 color;

	depthValue = pin.position.z/pin.position.w;
	color = float4(depthValue, depthValue, depthValue, 1.0f);

	return color;
}