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

	float power = 20.0f;

	float2 center = float2(0.5f, 0.5f);
	float2 dir = center - input.uv;
	float len = length(dir);
	float offset = normalize(dir) * center / float2(1280, 720);	
	offset *= (power * len);

	for(int i = 1; i <= 10; i++)
	{
		float2 pick_uv = offset * i;
		float weight = Gaussian(input.uv, pick_uv, sigma);

		color += tex.Sample(smp, pick_uv) * weight;

		total_weight += weight;
	}

	// for (float py = -sigma * 2; py <= sigma * 2; py += step_width) 
	// {
	// 	for (float px = -sigma * 2; px <= sigma * 2; px += step_width) 
	// 	{
	// 		float2 pick_uv = input.uv + float2(px, py);
	// 		float weight = Gaussian(input.uv, pick_uv, sigma);
	// 		color += tex.Sample(smp, pick_uv) * weight;
	// 		total_weight += weight;
	// 	}
	// }

    return color /= total_weight;
}
