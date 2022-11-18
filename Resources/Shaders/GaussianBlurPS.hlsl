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
	float sigma = 0.005;
	float step_width = 0.005;

	for (float py = -sigma * 2; py <= sigma * 2; py += step_width) 
	{
		for (float px = -sigma * 2; px <= sigma * 2; px += step_width) 
		{
			float2 pick_uv = input.uv + float2(px, py);
			float weight = Gaussian(input.uv, pick_uv, sigma);
			color += tex.Sample(smp, pick_uv) * weight;
			total_weight += weight;
		}
	}

    return color /= total_weight;
}
