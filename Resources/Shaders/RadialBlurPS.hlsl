#include "PostEffect.hlsli"

Texture2D<float4> tex : register(t0); // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

float Gaussian(float2 draw_uv, float2 pick_uv, float sigma) 
{
	float d = distance(draw_uv, pick_uv);
	return exp(-(d * d) / (2 * sigma * sigma));
}

float4 main(VSOutput input) : SV_TARGET0
 {
    float4 color = float4(0, 0, 0, 0);
    float total_weight = 0;
	
	float power = 30.0f;

	float2 center = float2(0.5f, 0.5f);
	float2 dir = center - input.uv;
	float len = length(dir);
	float2 offset = normalize(dir) * center / float2(1280, 720);	
	offset *= (power * len);

	float weight = 0.19f;
	for(int i = 1; i <= 10; i++)
	{
		float2 pick_uv = offset * i;
		pick_uv += input.uv;

		color += tex.Sample(smp, pick_uv) * weight;

		total_weight += weight;
		weight -= 0.02f;
	}

    return color /= total_weight;
}
