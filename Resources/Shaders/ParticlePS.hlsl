#include "Particle.hlsli"

Texture2D<float4> tex : register(t0); // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

float4 main(GSOutput input) : SV_TARGET
{
    float4 color = tex.Sample(smp, input.uv);
    // float4 source = float4(1.0f, 0.0f, 0.0f, 1.0f);
    color *= input.color;
    color *= input.alpha.r;
    // color *= input.color.r;
    return color;
}
