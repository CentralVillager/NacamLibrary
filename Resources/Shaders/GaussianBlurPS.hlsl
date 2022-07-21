#include "PostEffect.hlsli"

Texture2D<float4> tex : register(t0); // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

float Gaussian(float2 draw_uv, float2 pick_uv, float sigma) {

	float d = distance(draw_uv, pick_uv);
	return exp(-(d * d) / (2 * sigma * sigma));
}

float4 main(VSOutput input) : SV_TARGET0 {

    float4 color = float4(0, 0, 0, 0);
    float total_weight = 0, _Sigma = 0.005, _StepWidth = 0.005;

	for (float py = -_Sigma * 2; py <= _Sigma * 2; py += _StepWidth) {
		for (float px = -_Sigma * 2; px <= _Sigma * 2; px += _StepWidth) {

			float2 pick_uv = input.uv + float2(px, py);
			float weight = Gaussian(input.uv, pick_uv, _Sigma);
			color += tex.Sample(smp, pick_uv) * weight;
			total_weight += weight;
		}
	}

	color.rgb = color.rgb / total_weight;
    return color;
}
